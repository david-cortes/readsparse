/*    Read and write sparse matrices in text format:
*     <labels(s)> <column>:<value> <column>:<value> ...
* 
*     BSD 2-Clause License
*     Copyright (c) 2021, David Cortes
*     All rights reserved.
*     Redistribution and use in source and binary forms, with or without
*     modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this
*       list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice,
*       this list of conditions and the following disclaimer in the documentation
*       and/or other materials provided with the distribution.
*     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*     FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*     OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*     OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#include "readsparse_internal.hpp"
#include "utils.hpp"

/* https://stackoverflow.com/questions/16696297/ftell-at-a-position-past-2gb */
/* TODO: do CLANG and ICC have similar functionality? */
#if (defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)) && defined(PLATFORM_IS_64_OR_HIGHER)
#   ifdef _MSC_VER
#       include <limits> /* https://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of */
#       define fseek_ _fseeki64
#       define ftell_ _ftelli64
#       define fpos_t_ __int64
#   elif defined(__GNUG__) || defined(__GNUC__)
#       define fseek_ fseeko
#       define ftell_ ftello
#       define fpos_t_ off_t
#   else
#       define fseek_ fseek
#       define ftell_ ftell
#       define fpos_t_ long /* <- might overflow with large files */
#   endif
#else
#   define fseek_ fseek
#   define ftell_ ftell
#   define fpos_t_ long
#endif

#define WHITESPACE_CHARS " \t\v"

/* https://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int */
#define pow2(n) ( ((size_large) 1) << (n) )

/* https://stackoverflow.com/questions/9103294/c-how-to-inspect-file-byte-order-mark-in-order-to-get-if-it-is-utf-8 */
/* sequences to skip:
0x00, 0x00, 0xfe, 0xff
0xff, 0xfe, 0x00, 0x00
0xfe, 0xff
0xff, 0xfe
0xef, 0xbb, 0xbf
 */
static void skip_bom(FILE *input_file)
{
    fpos_t_ return_to = ftell_(input_file);
    int next_char = fgetc(input_file);
    char ch1, ch2, ch3, ch4;
    if (next_char == EOF) goto rewind;
    ch1 = next_char;
    if (ch1 == '\x00' || ch1 == '\xFF' ||
        ch1 == '\xFE' || ch1 == '\xEF')
    {
        ch1 = next_char;
        next_char = fgetc(input_file);
        if (next_char == EOF) goto rewind;
        ch2 = next_char;
        if (
            (ch1 == '\x00' && ch2 == '\x00') ||
            (ch1 == '\xFF' && ch2 == '\xFE') ||
            (ch1 == '\xFE' && ch2 == '\xFF') ||
            (ch1 == '\xFF' && ch2 == '\xFE') ||
            (ch1 == '\xEF' && ch2 == '\xBB')
        )
        {
            if (
                (ch1 == '\xFE' && ch2 == '\xFF') ||
                (ch1 == '\xFF' && ch2 == '\xFE')
            ) {
                return;
            }

            next_char = fgetc(input_file);
            if (next_char == EOF) goto rewind;
            ch3 = next_char;
            if (
                (ch1 == '\xEF' && ch2 == '\xBB' && ch3 == '\xBF')
            ) {
                return;
            }

            if (ch3 == '\xFE' || ch3 == '\x00')
            {
                next_char = fgetc(input_file);
                if (next_char == EOF) goto rewind;
                ch4 = next_char;
                if (
                    (ch1 == '\x00' && ch2 == '\x00' && ch3 == '\xFE' && ch4 == '\xFF') ||
                    (ch1 == '\xFF' && ch2 == '\xFE' && ch3 == '\x00' && ch4 == '\x00')
                ) {
                    return;
                }
            }
        }
    }

    rewind:
    {
        fseek_(input_file, return_to, SEEK_SET);
        return;
    }
}


template <class int_t>
void subtract_one_from_vec(std::vector<int_t> &vec)
{
    for (auto el : vec)
        if (el <= 0)
            return;
    for (size_t ix = 0; ix < vec.size(); ix++)
        vec[ix] -= (int_t)1;
}

template <class int_t>
size_t find_largest_val(std::vector<int_t> &vec, int_t missing_label)
{
    int_t max_val = 0;
    for (int_t el : vec)
        max_val = std::max(max_val, (el != missing_label)? el : max_val);
    return (size_t)max_val;
}

template <class int_t>
bool check_size_is_within_type(size_large nrows, size_large ncols, size_large nclasses)
{
    size_large max_allowed_int = SIZE_MAX;
    if (sizeof(int_t) < sizeof(size_large) || std::is_signed<int_t>::value)
    {
        #ifdef _MSC_VER
        #   pragma warning( push )
        #   pragma warning( disable : 4293 )
        #endif
        if (sizeof(int_t) < sizeof(size_large)) {
            max_allowed_int = pow2((size_large)8*(size_large)sizeof(int_t)) - (size_large)1;
        }
        #ifdef _MSC_VER
        #   pragma warning( pop )
        #endif
        if (std::is_signed<int_t>::value) {
            max_allowed_int /= (size_large)2;
        }
    }
    if (std::max(nrows, std::max(ncols, nclasses))
            >=
        max_allowed_int)
    {
        if (nrows >= max_allowed_int) {
            throw_err("Error: number of rows is too large for integer type of indices.\n");
        }
        else if (ncols >= max_allowed_int) {
            throw_err("Error: number of columns is too large for integer type of indices.\n");
        }
        else {
            throw_err("Error: number of classes is too large for integer type of indices.\n");
        }
        return false;
    }

    return true;
}

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#   pragma warning( disable : 4305 )
#endif

// template <class int_t=int64_t, class real_t=double>
template <class int_t, class real_t>
bool read_multi_label_template
(
    std::istream &input_file,
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    std::vector<int_t> &indptr_lab,
    std::vector<int_t> &indices_lab,
    std::vector<int_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
)
{
    if (input_file.fail())
    {
        throw_err("Error: cannot open input file path.\n");
        return false;
    }
    std::string ln;

    indptr.clear();
    indices.clear();
    values.clear();
    indptr_lab.clear();
    indices_lab.clear();

    indptr.push_back(0);
    indptr_lab.push_back(0);

    bool is_first_line = true;

    int_t curr_lab = 0;
    long bytes_advance = 0, adv1 = 0, adv2 = 0;
    char *ln_char;

    int_t curr_col = 0;
    real_t curr_val = 0;

    size_t lim_first;
    long long remainder;

    size_t header_nrows = 0, header_ncols = 0, header_nclasses = 0;

    const char *parse_code_labels, *parse_code_features;
    if (std::is_same<int_t, int>::value) {
        parse_code_labels = "%d%ln,%ln";
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lA%ln";
        else
            parse_code_features = "%d:%A%ln";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lg%ln";
        else
            parse_code_features = "%d:%g%ln";
        #endif
    }

    else if (std::is_same<int_t, size_t>::value) {
        #ifdef HAS_Z_SPECIFIER
        parse_code_labels = "%zu%ln,%ln";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_labels = "%" SCNu64 "%ln,%ln";
        #elif defined(COMPILER_IS_MINGW)
        parse_code_labels = "%u%ln,%ln";
        #else
        parse_code_labels = "%llu%ln,%ln";
        #endif
        
        if (std::is_same<real_t, double>::value)
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%lA%ln";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%lg%ln";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%lg%ln";
            #else
            parse_code_features = "%llu:%lA%ln";
            #endif
        else
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%A%ln";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%g%ln";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%g%ln";
            #else
            parse_code_features = "%llu:%A%ln";
            #endif
    }

    else if (std::is_same<int_t, int64_t>::value) {
        parse_code_labels = "%" SCNd64 "%ln,%ln";
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lA%ln";
        else
            parse_code_features = "%" SCNd64 ":%A%ln";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lg%ln";
        else
            parse_code_features = "%" SCNd64 ":%g%ln";
        #endif
    }

    else {
        parse_code_labels = "%ld%ln,%ln";
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lA%ln";
        else
            parse_code_features = "%ld:%A%ln";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lg%ln";
        else
            parse_code_features = "%ld:%g%ln";
        #endif
    }

    int_t missing_qid;
    #ifndef _FOR_R
    if (std::is_same<int_t, size_t>::value)
        missing_qid = SIZE_MAX;
    else
        missing_qid = -INT_MAX;
    #else
    missing_qid = NA_INTEGER;
    #endif
    const char *parse_code_qid;
    if (std::is_same<int_t, int>::value)
        parse_code_qid = "qid:%d%ln";
    else if (std::is_same<int_t, size_t>::value)
        #ifdef HAS_Z_SPECIFIER
        parse_code_qid = "qid:%zu%ln";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_qid = "qid:%" SCNu64 "%ln";
        #elif defined(COMPILER_IS_MINGW)
        parse_code_qid = "qid:%u%ln";
        #else
        parse_code_qid = "qid:%llu%ln";
        #endif
    else if (std::is_same<int_t, int64_t>::value)
        parse_code_qid = "qid:%" SCNd64 "%ln";
    else
        parse_code_qid = "qid:%ld%ln";

    while (std::getline(input_file, ln))
    {
        if (is_first_line && std::regex_search(ln, std::regex("^\\s*\\d+\\s+\\d+\\s+\\d+")))
        {
            #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
            std::sscanf(ln.c_str(), "%zu %zu %zu",
                        &header_nrows, &header_ncols, &header_nclasses);
            if (limit_nrows)
                header_nrows = std::min(header_nrows, limit_nrows);
            bool size_is_within_type
                =
            check_size_is_within_type<int_t>(header_nrows, header_ncols, header_nclasses);
            #else
            uint64_t temp1, temp2, temp3;
            std::sscanf(ln.c_str(), "%" SCNu64 " %" SCNu64 " %" SCNu64,
                        &temp1, &temp2, &temp3);
            if (limit_nrows)
                temp1 = std::min(temp1, (uint64_t)limit_nrows);
            bool size_is_within_type
                =
            check_size_is_within_type<int_t>(temp1, temp2, temp3);
            header_nrows = temp1;
            header_ncols = temp2;
            header_nclasses = temp3;
            #endif
            if (!size_is_within_type)
                return false;
            indptr.reserve(header_nrows);
            indptr_lab.reserve(header_nrows);
            if (!assume_no_qid) qid.reserve(header_nrows);
            is_first_line = false;
            continue;
        }
        is_first_line = false;

        if ((ln.size() <= 1 && (ln.size() == 0 || ln[0] != '#')) ||
            (ln.size() == 2 && ln[0] == '\r'))
        {
            indptr_lab.push_back(indices_lab.size());
            indptr.push_back(indices.size());
            if (!assume_no_qid)
                qid.push_back(missing_qid);
            continue;
        }

        ln_char = (char*)ln.c_str();
        if (ln_char[0] == '#')
            continue;
        lim_first = ln.find_first_of(WHITESPACE_CHARS, 0);
        if (lim_first == std::string::npos) {
            if (ln[0] == '\r')
                continue;
            else {
                char err_msg[200];
                #ifdef HAS_Z_SPECIFIER
                snprintf(err_msg, 200, "Invalid line encountered at row %zu.\n", indptr.size());
                #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
                snprintf(err_msg, 200, "Invalid line encountered at row %" PRIu64 ".\n", (uint64_t)indptr.size());
                #elif defined(COMPILER_IS_MINGW)
                snprintf(err_msg, 200, "Invalid line encountered at row %u.\n", (unsigned int)indptr.size());
                #else
                snprintf(err_msg, 200, "Invalid line encountered at row %llu.\n", (unsigned long long)indptr.size());
                #endif
                throw_err(err_msg);
                return false;
            }
        }
        remainder = lim_first;
        if (remainder == 0)
            goto get_features;
        
        adv2 = 0;
        while (sscanf(ln_char, parse_code_labels, &curr_lab, &adv1, &adv2) == 1)
        {
            bytes_advance = adv1 + (bool)adv2;
            indices_lab.push_back(curr_lab);
            ln_char += bytes_advance;
            remainder -= bytes_advance;
            if (remainder <= 0)
                break;
        }

        get_features:
        indptr_lab.push_back(indices_lab.size());
        ln_char = (char*)ln.c_str() + lim_first + 1;

        if (!assume_no_qid)
        {
            auto lim_next = ln.find("qid:", lim_first + 1);
            if (lim_next == std::string::npos)
                qid.push_back(missing_qid);
            else {
                auto pos_comment = ln.find('#', lim_next + 1);
                auto n_matched = sscanf((char*)ln.c_str() + lim_next, parse_code_qid, &curr_col, &bytes_advance);
                if ((n_matched == 1) && (pos_comment == std::string::npos || pos_comment >= lim_next + bytes_advance)) {
                    qid.push_back(curr_col);
                    lim_first = ln.find_first_of(WHITESPACE_CHARS, lim_next + bytes_advance);
                    if (lim_first == std::string::npos)
                        goto next_line;
                    ln_char = (char*)ln.c_str() + lim_first + 1;
                }
                else
                    qid.push_back(missing_qid);
            }

        }

        remainder = lim_first;
        lim_first = ln.find('#', lim_first);
        if (lim_first == std::string::npos)
            remainder = ln.size() - remainder;
        else
            remainder = lim_first - remainder;
        if (remainder == 0)
            goto next_line;

        while (sscanf(ln_char, parse_code_features, &curr_col, &curr_val, &bytes_advance) == 2)
        {
            if (!ignore_zero_valued || curr_val)
            {
                indices.push_back(curr_col);
                values.push_back(curr_val);
            }
            ln_char += bytes_advance;
            remainder -= bytes_advance;
            if (remainder <= 0)
                break;
        }


        next_line:
        indptr.push_back(indices.size());
        if (limit_nrows && indptr.size()-(size_t)1 == limit_nrows)
            break;
    }

    if (text_is_base1) {
        subtract_one_from_vec(indices);
        subtract_one_from_vec(indices_lab);
    }

    nrows = indptr.size() - 1;
    int_t missing_ind;
    if (std::is_same<int_t, size_t>::value)
        missing_ind = SIZE_MAX;
    else
        missing_ind = -1;
    ncols = std::max(header_ncols, find_largest_val<int_t>(indices, missing_ind)+1);
    nclasses = std::max(header_nclasses, find_largest_val(indices_lab, missing_ind)+1);

    sort_sparse_indices_known_ncol(indptr, indices, values, ncols);
    std::vector<real_t> unused_vec;
    sort_sparse_indices_known_ncol(indptr_lab, indices_lab, unused_vec, nclasses);

    return true;
}


// template <class int_t=int64_t, class real_t=double>
template <class int_t, class real_t>
bool read_multi_label_template
(
    FILE *input_file,
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    std::vector<int_t> &indptr_lab,
    std::vector<int_t> &indices_lab,
    std::vector<int_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
)
{
    if (input_file == NULL)
    {
        throw_err("Error: cannot open input file path.\n");
        return false;
    }

    skip_bom(input_file);

    indptr.clear();
    indices.clear();
    values.clear();
    indptr_lab.clear();
    indices_lab.clear();

    indptr.push_back(0);
    indptr_lab.push_back(0);

    bool is_first_line = true;

    int_t curr_lab = 0;
    int_t curr_col = 0;
    real_t curr_val = 0;

    fpos_t_ return_to = ftell_(input_file);
    int n_matched;
    int next_char = (int)'a';

    size_t header_nrows = 0, header_ncols = 0, header_nclasses = 0;

    const char *parse_code_labels, *parse_code_features;
    if (std::is_same<int_t, int>::value) {
        parse_code_labels = "%d[^:],";
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lA[^#]";
        else
            parse_code_features = "%d:%A[^#]";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lg[^#]";
        else
            parse_code_features = "%d:%g[^#]";
        #endif
    }

    else if (std::is_same<int_t, size_t>::value) {
        #ifdef HAS_Z_SPECIFIER
        parse_code_labels = "%zu[^:],";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_labels = "%" SCNu64 "[^:],";
        #elif defined(COMPILER_IS_MINGW)
        parse_code_labels = "%u[^:],";
        #else
        parse_code_labels = "%llu[^:],";
        #endif
        if (std::is_same<real_t, double>::value)
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%lA[^#]";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%lg[^#]";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%lg[^#]";
            #else
            parse_code_features = "%llu:%lA[^#]";
            #endif
        else
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%A[^#]";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%" SCNu64 ":%g[^#]";
            #else
            parse_code_features = "%llu:%A[^#]";
            #endif
    }

    else if (std::is_same<int_t, int64_t>::value) {
        parse_code_labels = "%" SCNd64 "[^:],";
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lA[^#]";
        else
            parse_code_features = "%" SCNd64 ":%A[^#]";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lg[^#]";
        else
            parse_code_features = "%" SCNd64 ":%g[^#]";
        #endif
    }

    else {
        parse_code_labels = "%ld[^:],";
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lA[^#]";
        else
            parse_code_features = "%ld:%A[^#]";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lg[^#]";
        else
            parse_code_features = "%ld:%g[^#]";
        #endif
    }

    int_t missing_qid;
    #ifndef _FOR_R
    if (std::is_same<int_t, size_t>::value)
        missing_qid = SIZE_MAX;
    else
        missing_qid = -INT_MAX;
    #else
    missing_qid = NA_INTEGER;
    #endif
    const char *parse_code_qid;
    if (std::is_same<int_t, int>::value)
        parse_code_qid = "qid:%d";
    else if (std::is_same<int_t, size_t>::value)
        #ifdef HAS_Z_SPECIFIER
        parse_code_qid = "qid:%zu";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_qid = "qid:%" SCNu64;
        #elif defined(COMPILER_IS_MINGW)
        parse_code_qid = "qid:%u";
        #else
        parse_code_qid = "qid:%llu";
        #endif
    else if (std::is_same<int_t, int64_t>::value)
        parse_code_qid = "qid:%" SCNd64;
    else
        parse_code_qid = "qid:%ld";

    while (true)
    {
        if (is_first_line)
        {
            char buffer_first_line[1000];
            n_matched = fscanf(input_file, "%999[^\n]", buffer_first_line);
            if (n_matched == EOF)
                break;
            #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
            n_matched = sscanf(buffer_first_line, "%zu %zu %zu",
                               &header_nrows, &header_ncols, &header_nclasses);
            #else
            uint64_t temp1, temp2, temp3;
            n_matched = sscanf(buffer_first_line, "%" SCNu64 " %" SCNu64 " %" SCNu64,
                               &temp1, &temp2, &temp3);
            header_nrows = temp1;
            header_ncols = temp2;
            header_nclasses = temp3;
            #endif
            

            if (n_matched != 3)
            {
                header_nrows = 0;
                header_ncols = 0;
                header_nclasses = 0;
                fseek_(input_file, return_to, SEEK_SET);
                is_first_line = false;
                continue;
            }

            else if (limit_nrows)
            {
                #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
                header_nrows = std::min(header_nrows, limit_nrows);
                #else
                temp1 = std::min(temp1, (uint64_t)limit_nrows);
                header_nrows = temp1;
                #endif
            }

            bool size_is_within_type
                =
            #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
            check_size_is_within_type<int_t>(header_nrows, header_ncols, header_nclasses);
            #else
            check_size_is_within_type<int_t>(temp1, temp2, temp3);
            #endif
            if (!size_is_within_type)
                return false;
            indptr.reserve(header_nrows);
            indptr_lab.reserve(header_nrows);
            if (!assume_no_qid) qid.reserve(header_nrows);
            is_first_line = false;
            do { next_char = fgetc(input_file); }
            while ((char)next_char != '\n' && next_char != EOF);
            if (next_char == EOF)
                break;
            continue;
        }
        is_first_line = false;

        
        return_to = ftell_(input_file);
        
        /* check for empty line */
        next_char = fgetc(input_file);
        if (isspace((char)next_char) || (char)next_char == '#' || next_char == EOF)
        {
            if (next_char == EOF) {
                break;
            }
            else if ((char)next_char == '\n' || (char)next_char == '\r') {
                indptr_lab.push_back(indices_lab.size());
                indptr.push_back(indices.size());
                if (!assume_no_qid) qid.push_back(missing_qid);
                while ((char)next_char != '\n' && next_char != EOF)
                { next_char = fgetc(input_file); };
                if (next_char == EOF)
                    break;
                continue;
            }
            else if ((char)next_char == '#') {
                do { next_char = fgetc(input_file); }
                while ((char)next_char != '\n' && next_char != EOF);
                if (next_char == EOF)
                    break;
                continue;
            }
            else {
                next_char = fgetc(input_file);
                if ((next_char >= 48 && next_char <= 57) ||
                    (next_char >= 43 && next_char <= 46) ||
                    ((char)next_char == 'i' || (char)next_char == 'I') ||
                    ((char)next_char == 'n' || (char)next_char == 'N'))
                {
                    /* 0-9, -, ., +, i(nf), n(an) (expected case) */
                    fseek_(input_file, return_to, SEEK_SET);
                }

                else if (next_char == EOF) {
                    break;
                }

                else {
                    goto get_features;
                }
            }
        }

        else
        {
            fseek_(input_file, return_to, SEEK_SET);
        }

        while ((n_matched = fscanf(input_file, parse_code_labels, &curr_lab)) == 1)
        {
            next_char = fgetc(input_file);
            if ((char)next_char == ':')
            {
                fseek_(input_file, return_to, SEEK_SET);
                goto get_features;
            }
            indices_lab.push_back(curr_lab);
            return_to = ftell_(input_file);
            if ((char)next_char != ',')
                break;
        }


        get_features:
        indptr_lab.push_back(indices_lab.size());
        if (n_matched == EOF)
        {
            if (!assume_no_qid) qid.push_back(missing_qid);
            break;
        }
        if (indptr_lab.back() == indptr_lab[indptr_lab.size()-2])
        {
            fseek_(input_file, return_to, SEEK_SET);
        }

        if ((char)next_char == '\n' || (char)next_char == '\r' || (char)next_char == '#')
        {
            if (!assume_no_qid) qid.push_back(missing_qid);
            goto next_line;
        }
        return_to = ftell_(input_file);
        next_char = fgetc(input_file);
        if ((char)next_char == '\n' || (char)next_char == '\r' || (char)next_char == '#' || next_char == EOF) {
            if (!assume_no_qid) qid.push_back(missing_qid);
            goto next_line;
        }
        else
            fseek_(input_file, return_to, SEEK_SET);

        if (!assume_no_qid)
        {
            n_matched = fscanf(input_file, parse_code_qid, &curr_col);
            if (n_matched == EOF) {
                qid.push_back(missing_qid);
                break;
            }
            else if (n_matched == 0)
                qid.push_back(missing_qid);
            else
                qid.push_back(curr_col);
        }

        n_matched = 0;
        if (assume_trailing_ws)
        {
            while ((n_matched = fscanf(input_file, parse_code_features, &curr_col, &curr_val)) == 2)
            {
                if (!ignore_zero_valued || curr_val)
                {
                    indices.push_back(curr_col);
                    values.push_back(curr_val);
                }
                
                return_to = ftell_(input_file);
                do { next_char = fgetc(input_file); }
                while (next_char != EOF && isspace(next_char) && (char)next_char != '\n');
                if (next_char == EOF || (char)next_char == '\n' || (char)next_char == '#')
                    goto next_line;
                else
                    fseek_(input_file, return_to, SEEK_SET);
            }
            if (n_matched == 1) {
                fseek_(input_file, return_to, SEEK_SET);
                goto next_line;
            }
        }

        else
        {
            while ((n_matched = fscanf(input_file, parse_code_features, &curr_col, &curr_val)) == 2)
            {
                if (!ignore_zero_valued || curr_val)
                {
                    indices.push_back(curr_col);
                    values.push_back(curr_val);
                }
                
                next_char = fgetc(input_file);
                if ((char)next_char == '\n' || (char)next_char == '\r' || !isspace((char)next_char))
                    break;
            }
        }


        next_line:
        indptr.push_back(indices.size());
        if (indptr.back() == indptr[indptr.size()-2])
        {
            fseek_(input_file, return_to, SEEK_SET);
            do { next_char = fgetc(input_file); }
            while ((char)next_char != '\n' && next_char != EOF);
        }

        if (n_matched == EOF || next_char == EOF)
            break;
        while (next_char != EOF && (char)next_char != '\n')
        {
            next_char = fgetc(input_file);
        }

        if (limit_nrows && indptr.size()-1 == limit_nrows)
            break;
    }

    if (!feof(input_file) && ferror(input_file))
    {
        print_errno();
        return false;
    }

    if (text_is_base1) {
        subtract_one_from_vec(indices);
        subtract_one_from_vec(indices_lab);
    }

    nrows = indptr.size() - 1;
    int_t missing_ind;
    if (std::is_same<int_t, size_t>::value)
        missing_ind = SIZE_MAX;
    else
        missing_ind = -1;
    ncols = std::max(header_ncols, find_largest_val<int_t>(indices, missing_ind)+1);
    nclasses = std::max(header_nclasses, find_largest_val(indices_lab, missing_ind)+1);

    sort_sparse_indices_known_ncol(indptr, indices, values, ncols);
    std::vector<real_t> temp;
    sort_sparse_indices_known_ncol(indptr_lab, indices_lab, temp, ncols);

    return true;
}

// template <class int_t=int64_t, class real_t=double, class label_t=double>
template <class int_t, class real_t, class label_t>
bool read_single_label_template
(
    std::istream &input_file,
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    std::vector<label_t> &labels,
    std::vector<int_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
)
{
    if (input_file.fail())
    {
        throw_err("Error: cannot open input file path.\n");
        return false;
    }
    std::string ln;

    indptr.clear();
    indices.clear();
    values.clear();
    labels.clear();

    indptr.push_back(0);

    bool is_first_line = true;

    label_t curr_lab = 0;
    long bytes_advance = 0;
    char *ln_char;

    int_t curr_col = 0;
    real_t curr_val = 0;

    size_t lim_first;
    long long remainder;

    size_t header_nrows = 0, header_ncols = 0, header_nclasses = 0;

    label_t missing_label;

    const char *parse_code_labels, *parse_code_features;
    if (std::is_same<int_t, int>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lA%ln";
        else
            parse_code_features = "%d:%A%ln";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lg%ln";
        else
            parse_code_features = "%d:%g%ln";
        #endif
    }

    else if (std::is_same<int_t, size_t>::value) {
        if (std::is_same<real_t, double>::value)
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%lA%ln";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%lg%ln";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%lg%ln";
            #else
            parse_code_features = "%llu:%lA%ln";
            #endif
        else
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%A%ln";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%g%ln";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%g%ln";
            #else
            parse_code_features = "%llu:%A%ln";
            #endif
    }

    else if (std::is_same<int_t, int64_t>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lA%ln";
        else
            parse_code_features = "%" SCNd64 ":%A%ln";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lg%ln";
        else
            parse_code_features = "%" SCNd64 ":%g%ln";
        #endif
    }

    else {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lA%ln";
        else
            parse_code_features = "%ld:%A%ln";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lg%ln";
        else
            parse_code_features = "%ld:%g%ln";
        #endif
    }

    if (std::is_same<label_t, int>::value) {
        parse_code_labels = "%d%ln";
        #ifndef _FOR_R
        missing_label = -INT_MAX;
        #else
        missing_label = NA_INTEGER;
        #endif
    }
    else if (std::is_same<label_t, size_t>::value) {
        #ifdef HAS_Z_SPECIFIER
        parse_code_labels = "%zu%ln";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_labels = "%" SCNu64 "%ln";
        #elif defined(COMPILER_IS_MINGW)
        parse_code_labels = "%u%ln";
        #else
        parse_code_labels = "%llu%ln";
        #endif
        #ifndef _FOR_R
        missing_label = SIZE_MAX;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else if (std::is_same<label_t, int64_t>::value) {
        parse_code_labels = "%" SCNd64 "%ln";
        missing_label = -INT_MAX;
    }
    else if (std::is_same<label_t, double>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        parse_code_labels = "%lA%ln";
        #else
        parse_code_labels = "%lg%ln";
        #endif
        #ifndef _FOR_R
        missing_label = NAN;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else if (std::is_same<label_t, float>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        parse_code_labels = "%A%ln";
        #else
        parse_code_labels = "%g%ln";
        #endif
        #ifndef _FOR_R
        missing_label = NAN;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else if (std::is_same<label_t, long double>::value) {
        #ifdef HAS_FULL_STDIO
        parse_code_labels = "%llA%ln";
        #elif defined(_MSC_VER)
        parse_code_labels = "%LA%ln";
        #elif defined(HAS_Z_SPECIFIER) && !defined(COMPILER_IS_MINGW)
        parse_code_labels = "%lg%ln";
        #else
        parse_code_labels = "%lA%ln";
        #endif
        #ifndef _FOR_R
        missing_label = NAN;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else {
        parse_code_labels = "%ld%ln";
        #ifndef _FOR_R
        if (std::is_same<label_t, uint64_t>::value)
            missing_label = UINT64_MAX;
        else
            missing_label = -INT_MAX;
        #else
        missing_label = NA_INTEGER;
        #endif
    }

    int_t missing_qid;
    #ifndef _FOR_R
    if (std::is_same<int_t, size_t>::value)
        missing_qid = SIZE_MAX;
    else
        missing_qid = -INT_MAX;
    #else
    missing_qid = NA_INTEGER;
    #endif
    const char *parse_code_qid;
    if (std::is_same<int_t, int>::value)
        parse_code_qid = "qid:%d%ln";
    else if (std::is_same<int_t, size_t>::value)
        #ifdef HAS_Z_SPECIFIER
        parse_code_qid = "qid:%zu%ln";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_qid = "qid:%" SCNu64 "%ln";
        #elif defined(COMPILER_IS_MINGW)
        parse_code_qid = "qid:%u%ln";
        #else
        parse_code_qid = "qid:%llu%ln";
        #endif
    else if (std::is_same<int_t, int64_t>::value)
        parse_code_qid = "qid:%" SCNd64 "%ln";
    else
        parse_code_qid = "qid:%ld%ln";

    while (std::getline(input_file, ln))
    {
        if (is_first_line && std::regex_search(ln, std::regex("^\\s*\\d+\\s+\\d+\\s+\\d+")))
        {
            #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
            std::sscanf(ln.c_str(), "%zu %zu %zu",
                        &header_nrows, &header_ncols, &header_nclasses);
            if (limit_nrows)
                header_nrows = std::min(header_nrows, limit_nrows);
            bool size_is_within_type
                =
            check_size_is_within_type<int_t>(header_nrows, header_ncols, header_nclasses);
            #else
            uint64_t temp1, temp2, temp3;
            std::sscanf(ln.c_str(), "%" SCNu64 " %" SCNu64 " %" SCNu64,
                        &temp1, &temp2, &temp3);
            if (limit_nrows)
                temp1 = std::min(temp1, (uint64_t)limit_nrows);
            bool size_is_within_type
                =
            check_size_is_within_type<int_t>(temp1, temp2, temp3);
            header_nrows = temp1;
            header_ncols = temp2;
            header_nclasses = temp3;
            #endif
            if (!size_is_within_type)
                return false;
            indptr.reserve(header_nrows);
            labels.reserve(header_nrows);
            if (!assume_no_qid) qid.reserve(header_nrows);
            is_first_line = false;
            continue;
        }
        is_first_line = false;

        if ((ln.size() <= 1 && (ln.size() == 0 || ln[0] != '#')) ||
            (ln.size() == 2 && ln[0] == '\r'))
        {
            labels.push_back(missing_label);
            indptr.push_back(indices.size());
            if (!assume_no_qid)
                qid.push_back(missing_qid);
            continue;
        }

        ln_char = (char*)ln.c_str();
        if (ln_char[0] == '#')
            continue;
        lim_first = ln.find_first_of(WHITESPACE_CHARS, 0);
        if (lim_first == std::string::npos) {
            if (ln[0] == '\r')
                continue;
            else {
                char err_msg[200];
                #ifdef HAS_Z_SPECIFIER
                snprintf(err_msg, 200, "Invalid line encountered at row %zu.\n", indptr.size());
                #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
                snprintf(err_msg, 200, "Invalid line encountered at row %" PRIu64 ".\n", (uint64_t)indptr.size());
                #elif defined(COMPILER_IS_MINGW)
                snprintf(err_msg, 200, "Invalid line encountered at row %u.\n", (unsigned int)indptr.size());
                #else
                snprintf(err_msg, 200, "Invalid line encountered at row %llu.\n", (unsigned long long)indptr.size());
                #endif
                throw_err(err_msg);
                return false;
            }
        }
        remainder = lim_first;
        if (remainder == 0) {
            labels.push_back(missing_label);
            goto get_features;
        }
        
        if (sscanf(ln_char, parse_code_labels, &curr_lab, &bytes_advance) == 1)
            labels.push_back(curr_lab);
        else
            labels.push_back(missing_label);

        get_features:
        ln_char = (char*)ln.c_str() + lim_first + 1;

        if (!assume_no_qid)
        {
            auto lim_next = ln.find("qid:", lim_first + 1);
            if (lim_next == std::string::npos)
                qid.push_back(missing_qid);
            else {
                auto pos_comment = ln.find('#', lim_next + 1);
                auto n_matched = sscanf((char*)ln.c_str() + lim_next, parse_code_qid, &curr_col, &bytes_advance);
                if ((n_matched == 1) && (pos_comment == std::string::npos || pos_comment >= lim_next + bytes_advance)) {
                    qid.push_back(curr_col);
                    lim_first = ln.find_first_of(WHITESPACE_CHARS, lim_next + bytes_advance);
                    if (lim_first == std::string::npos)
                        goto next_line;
                    ln_char = (char*)ln.c_str() + lim_first + 1;
                }
                else
                    qid.push_back(missing_qid);
            }

        }

        remainder = lim_first;
        lim_first = ln.find('#', lim_first);
        if (lim_first == std::string::npos)
            remainder = ln.size() - remainder;
        else
            remainder = lim_first - remainder;
        if (remainder == 0)
            goto next_line;

        while (sscanf(ln_char, parse_code_features, &curr_col, &curr_val, &bytes_advance) == 2)
        {
            if (!ignore_zero_valued || curr_val)
            {
                indices.push_back(curr_col);
                values.push_back(curr_val);
            }
            ln_char += bytes_advance;
            remainder -= bytes_advance;
            if (remainder <= 0)
                break;
        }


        next_line:
        indptr.push_back(indices.size());
        if (limit_nrows && indptr.size()-1 == limit_nrows)
            break;
    }

    if (text_is_base1)
        subtract_one_from_vec(indices);

    nrows = indptr.size() - 1;
    int_t missing_ind;
    if (std::is_same<int_t, size_t>::value)
        missing_ind = SIZE_MAX;
    else
        missing_ind = -1;
    ncols = std::max(header_ncols, find_largest_val<int_t>(indices, missing_ind)+1);
    if (!(std::is_same<label_t, float>::value || std::is_same<label_t, double>::value || std::is_same<label_t, long double>::value))
        nclasses = std::max(header_nclasses, find_largest_val(labels, missing_label)+1);
    else
        nclasses = 0;

    sort_sparse_indices_known_ncol(indptr, indices, values, ncols);

    return true;
}

// template <class int_t=int64_t, class real_t=double, class label_t=double>
template <class int_t, class real_t, class label_t>
bool read_single_label_template
(
    FILE *input_file,
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    std::vector<label_t> &labels,
    std::vector<int_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
)
{
    if (input_file == NULL)
    {
        throw_err("Error: cannot open input file path.\n");
        return false;
    }

    skip_bom(input_file);

    indptr.clear();
    indices.clear();
    values.clear();
    labels.clear();

    indptr.push_back(0);

    bool is_first_line = true;

    size_t header_nrows = 0, header_ncols = 0, header_nclasses = 0;

    label_t curr_lab = 0;
    int_t curr_col = 0;
    real_t curr_val = 0;

    fpos_t_ return_to = ftell_(input_file);
    int n_matched;
    int next_char = (int)'a';


    const char *parse_code_labels, *parse_code_features;
    if (std::is_same<int_t, int>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lA[^#]";
        else
            parse_code_features = "%d:%A[^#]";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%d:%lg[^#]";
        else
            parse_code_features = "%d:%g[^#]";
        #endif
    }

    else if (std::is_same<int_t, size_t>::value) {
        if (std::is_same<real_t, double>::value)
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%lA[^#]";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%lg[^#]";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%lg[^#]";
            #else
            parse_code_features = "%llu:%lA[^#]";
            #endif
        else
            #ifdef HAS_Z_SPECIFIER
            parse_code_features = "%zu:%A[^#]";
            #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
            parse_code_features = "%" SCNu64 ":%g[^#]";
            #elif defined(COMPILER_IS_MINGW)
            parse_code_features = "%u:%g[^#]";
            #else
            parse_code_features = "%llu:%A[^#]";
            #endif
    }

    else if (std::is_same<int_t, int64_t>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lA[^#]";
        else
            parse_code_features = "%" SCNd64 ":%A[^#]";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%" SCNd64 ":%lg[^#]";
        else
            parse_code_features = "%" SCNd64 ":%g[^#]";
        #endif
    }

    else {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lA[^#]";
        else
            parse_code_features = "%ld:%A[^#]";
        #else
        if (std::is_same<real_t, double>::value)
            parse_code_features = "%ld:%lg[^#]";
        else
            parse_code_features = "%ld:%g[^#]";
        #endif
    }

    label_t missing_label;
    if (std::is_same<label_t, int>::value) {
        parse_code_labels = "%d[^:]";
        #ifndef _FOR_R
        missing_label = -INT_MAX;
        #else
        missing_label = NA_INTEGER;
        #endif
    }
    else if (std::is_same<label_t, size_t>::value) {
        #ifdef HAS_Z_SPECIFIER
        parse_code_labels = "%zu[^:]";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_labels = "%" SCNu64 "[^:]";
        #elif defined(COMPILER_IS_MINGW)
        parse_code_labels = "%u[^:]";
        #else
        parse_code_labels = "%llu[^:]";
        #endif
        #ifndef _FOR_R
        missing_label = SIZE_MAX;
        #else
        missing_label = NA_INTEGER;
        #endif
    }
    if (std::is_same<label_t, int64_t>::value) {
        parse_code_labels = "%" SCNd64 "[^:]";
        missing_label = -INT_MAX;
    }
    else if (std::is_same<label_t, double>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        parse_code_labels = "%lA[^:]";
        #else
        parse_code_labels = "%lg[^:]";
        #endif
        #ifndef _FOR_R
        missing_label = NAN;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else if (std::is_same<label_t, float>::value) {
        #if defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        parse_code_labels = "%A[^:]";
        #else
        parse_code_labels = "%g[^:]";
        #endif
        #ifndef _FOR_R
        missing_label = NAN;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else if (std::is_same<label_t, long double>::value) {
        #ifdef HAS_FULL_STDIO
        parse_code_labels = "%llA[^:]";
        #elif defined(HAS_Z_SPECIFIER) || !defined(COMPILER_IS_MINGW)
        parse_code_labels = "%llA[^:]";
        #else
        parse_code_labels = "%lg[^:]";
        #endif
        #ifndef _FOR_R
        missing_label = NAN;
        #else
        missing_label = NA_REAL;
        #endif
    }
    else {
        parse_code_labels = "%ld[^:]";
        #ifndef _FOR_R
        if (std::is_same<label_t, uint64_t>::value)
            missing_label = UINT64_MAX;
        else
            missing_label = -INT_MAX;
        #else
        missing_label = NA_INTEGER;
        #endif
    }

    int_t missing_qid;
    #ifndef _FOR_R
    if (std::is_same<int_t, size_t>::value)
        missing_qid = SIZE_MAX;
    else
        missing_qid = -INT_MAX;
    #else
    missing_qid = NA_INTEGER;
    #endif
    const char *parse_code_qid;
    if (std::is_same<int_t, int>::value)
        parse_code_qid = "qid:%d";
    else if (std::is_same<int_t, size_t>::value)
        #ifdef HAS_Z_SPECIFIER
        parse_code_qid = "qid:%zu";
        #elif defined(COMPILER_IS_MINGW) && defined(PLATFORM_IS_64_OR_HIGHER)
        parse_code_qid = "qid:%" SCNu64;
        #elif defined(COMPILER_IS_MINGW)
        parse_code_qid = "qid:%u";
        #else
        parse_code_qid = "qid:%llu";
        #endif
    else if (std::is_same<int_t, int64_t>::value)
        parse_code_qid = "qid:%" SCNd64;
    else
        parse_code_qid = "qid:%ld";

    while (true)
    {
        if (is_first_line)
        {
            char buffer_first_line[1000];
            n_matched = fscanf(input_file, "%999[^\n]", buffer_first_line);
            if (n_matched == EOF)
                break;
            #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
            n_matched = sscanf(buffer_first_line, "%zu %zu %zu",
                               &header_nrows, &header_ncols, &header_nclasses);
            #else
            uint64_t temp1, temp2, temp3;
            n_matched = sscanf(buffer_first_line, "%" SCNu64 " %" SCNu64 " %" SCNu64,
                               &temp1, &temp2, &temp3);
            header_nrows = temp1;
            header_ncols = temp2;
            header_nclasses = temp3;
            #endif


            if (n_matched != 3)
            {
                header_nrows = 0;
                header_ncols = 0;
                header_nclasses = 0;
                fseek_(input_file, return_to, SEEK_SET);
                is_first_line = false;
                continue;
            }

            else if (limit_nrows)
            {
                #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
                header_nrows = std::min(header_nrows, limit_nrows);
                #else
                temp1 = std::min(temp1, (uint64_t)limit_nrows);
                header_nrows = temp1;
                #endif
            }

            bool size_is_within_type
                =
            #if defined(HAS_Z_SPECIFIER) && defined(PLATFORM_IS_64_OR_HIGHER)
            check_size_is_within_type<int_t>(header_nrows, header_ncols, header_nclasses);
            #else
            check_size_is_within_type<int_t>(temp1, temp2, temp3);
            #endif
            if (!size_is_within_type)
                return false;
            indptr.reserve(header_nrows);
            labels.reserve(header_nrows);
            if (!assume_no_qid) qid.reserve(header_nrows);
            is_first_line = false;
            do { next_char = fgetc(input_file); }
            while ((char)next_char != '\n' && next_char != EOF);
            if (next_char == EOF)
                break;
            continue;
        }
        is_first_line = false;

        
        return_to = ftell_(input_file);
        
        /* check for empty line */
        next_char = fgetc(input_file);
        if (isspace((char)next_char) || (char)next_char == '#' || next_char == EOF)
        {
            if (next_char == EOF) {
                break;
            }
            else if ((char)next_char == '\n' || (char)next_char == '\r') {
                labels.push_back(missing_label);
                indptr.push_back(indices.size());
                if (!assume_no_qid) qid.push_back(missing_qid);
                while ((char)next_char != '\n' && next_char != EOF)
                { next_char = fgetc(input_file); };
                if (next_char == EOF)
                    break;
                continue;
            }
            else if ((char)next_char == '#') {
                do { next_char = fgetc(input_file); }
                while ((char)next_char != '\n' && next_char != EOF);
                if (next_char == EOF)
                    break;
                continue;
            }
            else {
                next_char = fgetc(input_file);
                if ((next_char >= 48 && next_char <= 57) ||
                    (next_char >= 43 && next_char <= 46) ||
                    ((char)next_char == 'i' || (char)next_char == 'I') ||
                    ((char)next_char == 'n' || (char)next_char == 'N'))
                {
                    /* 0-9, -, ., +, i(nf), n(an) (expected case) */
                    fseek_(input_file, return_to, SEEK_SET);
                }

                else if (next_char == EOF) {
                    break;
                }

                else {
                    labels.push_back(missing_label);
                    goto get_features;
                }
            }
        }

        else
        {
            fseek_(input_file, return_to, SEEK_SET);
        }

        if ((n_matched = fscanf(input_file, parse_code_labels, &curr_lab)) == 1)
        {
            next_char = fgetc(input_file);
            if ((char)next_char == ':')
            {
                fseek_(input_file, return_to, SEEK_SET);
                labels.push_back(missing_label);
                goto get_features;
            }
            /* in case of multi-label, should take only the first one */
            else if ((char)next_char == ',')
            {
                do { next_char = fgetc(input_file); }
                while ((char)next_char != '#' && next_char != EOF && !isspace((char)next_char));
            }
            labels.push_back(curr_lab);
            return_to = ftell_(input_file);
        }

        else if (n_matched == EOF) {
            if (!assume_no_qid) qid.push_back(missing_qid);
            break;
        }

        else {
            fseek_(input_file, return_to, SEEK_SET);
            labels.push_back(missing_label);
        }

        get_features:
        if ((char)next_char == '\n' || (char)next_char == '\r' || (char)next_char == '#') {
            if (!assume_no_qid) qid.push_back(missing_qid);
            goto next_line;
        }
        return_to = ftell_(input_file);
        next_char = fgetc(input_file);
        if ((char)next_char == '\n' || (char)next_char == '\r' || (char)next_char == '#' || next_char == EOF) {
            if (!assume_no_qid) qid.push_back(missing_qid);
            goto next_line;
        }
        else
            fseek_(input_file, return_to, SEEK_SET);

        if (!assume_no_qid)
        {
            n_matched = fscanf(input_file, parse_code_qid, &curr_col);
            if (n_matched == EOF) {
                qid.push_back(missing_qid);
                break;
            }
            else if (n_matched == 0)
                qid.push_back(missing_qid);
            else
                qid.push_back(curr_col);
        }

        if (assume_trailing_ws)
        {
            while ((n_matched = fscanf(input_file, parse_code_features, &curr_col, &curr_val)) == 2)
            {
                if (!ignore_zero_valued || curr_val)
                {
                    indices.push_back(curr_col);
                    values.push_back(curr_val);
                }
                
                return_to = ftell_(input_file);
                do { next_char = fgetc(input_file); }
                while (next_char != EOF && isspace(next_char) && (char)next_char != '\n');
                if (next_char == EOF || (char)next_char == '\n' || (char)next_char == '#')
                    goto next_line;
                else
                    fseek_(input_file, return_to, SEEK_SET);
            }
                if (n_matched == 1) {
                fseek_(input_file, return_to, SEEK_SET);
                goto next_line;
            }
        }

        else
        {
             while ((n_matched = fscanf(input_file, parse_code_features, &curr_col, &curr_val)) == 2)
            {
                if (!ignore_zero_valued || curr_val)
                {
                    indices.push_back(curr_col);
                    values.push_back(curr_val);
                }
                
                next_char = fgetc(input_file);
                if ((char)next_char == '\n' || (char)next_char == '\r' || !isspace((char)next_char))
                    break;
                }
        }

        next_line:
        indptr.push_back(indices.size());
        if (indptr.back() == indptr[indptr.size()-2])
        {
            fseek_(input_file, return_to, SEEK_SET);
            do { next_char = fgetc(input_file); }
            while ((char)next_char != '\n' && next_char != EOF);
        }

        if (n_matched == EOF || next_char == EOF)
            break;
        while (next_char != EOF && (char)next_char != '\n')
        {
            next_char = fgetc(input_file);
        }

        if (limit_nrows && indptr.size()-1 == limit_nrows)
            break;
    }

    if (!feof(input_file) && ferror(input_file))
    {
        print_errno();
        return false;
    }

    if (text_is_base1)
        subtract_one_from_vec(indices);

    nrows = indptr.size() - 1;
    int_t missing_ind;
    if (std::is_same<int_t, size_t>::value)
        missing_ind = SIZE_MAX;
    else
        missing_ind = -1;
    ncols = std::max(header_ncols, find_largest_val<int_t>(indices, missing_ind)+1);
    if (!(std::is_same<label_t, float>::value || std::is_same<label_t, double>::value || std::is_same<label_t, long double>::value))
        nclasses = std::max(header_nclasses, find_largest_val(labels, missing_label)+1);
    else
        nclasses = 0;

    sort_sparse_indices_known_ncol(indptr, indices, values, ncols);

    return true;
}

#ifdef _MSC_VER
#   pragma warning( pop )
#endif
