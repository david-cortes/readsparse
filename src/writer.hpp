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

#ifdef __cplusplus
#   define above_min_decimals(x) (x && std::abs(x) >= thr_decimals)
#else
#   define above_min_decimals(x) (x && fabs((double)x) >= thr_decimals)
#endif

// template <class int_t=int64_t, class real_t=double>
template <class int_t, class real_t>
bool write_multi_label_template
(
    std::ostream &output_file,
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *restrict values,
    int_t *restrict indptr_lab,
    int_t *restrict indices_lab,
    int_t *restrict qid,
    const int_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    if (output_file.fail())
    {
        throw_err("Error: invalid output_file.\n");
        return false;
    }

    if (decimal_places < 0)
    {
        throw_err("Error: 'decimal_places' cannot be negative.\n");
        return false;
    }

    if (nrows == 0 && !add_header)
       return true;

    bool succeded = true;
    #ifdef _MSC_VER
    #   pragma warning(push)
    #   pragma warning(disable:4244)
    #endif
    real_t thr_decimals = std::pow(10., (double)(-decimal_places)) / 2.;
    #ifdef _MSC_VER
    #   pragma warning(pop)
    #endif
    size_t n_this, ix1, ix2;

    /* https://stackoverflow.com/questions/8554441/how-to-output-with-3-digits-after-the-decimal-point-with-c-stream */
    std::ios_base::fmtflags oldflags = output_file.flags();
    std::streamsize oldprecision = output_file.precision();
    output_file << std::fixed << std::setprecision(decimal_places);

    if (add_header)
    {
        output_file << nrows << ' ' << ncols << ' ' << nclasses << '\n';
        if (output_file.bad()) goto terminate_badly;
    }

    if (nrows == 0)
        goto terminate_func;

    if (sort_indices)
    {
        sort_sparse_indices(indptr, indices, values, nrows);
        sort_sparse_indices(indptr_lab, indices_lab, nrows);
    }

    for (size_t row = 0; row < nrows; row++)
    {
        ix1 = indptr_lab[row];
        ix2 = indptr_lab[row+1];
        n_this = ix2 - ix1;
        if (n_this == 1)
        {
            output_file << (indices_lab[ix1] + text_is_base1);
            if (output_file.bad()) goto terminate_badly;
        }

        else if (n_this > 1)
        {
            for (size_t ix = ix1; ix < ix2-1; ix++) {
                output_file << (indices_lab[ix] + text_is_base1) << ',';
                if (output_file.bad()) goto terminate_badly;
            }
            output_file << (indices_lab[ix2-1] + text_is_base1);
            if (output_file.bad()) goto terminate_badly;
        }

        output_file << ' ';
        if (output_file.bad()) goto terminate_badly;

        if (has_qid && qid[row] != missing_qid)
        {
            output_file << "qid:" << qid[row] << ' ';
            if (output_file.bad()) goto terminate_badly;
        }

        ix1 = indptr[row];
        ix2 = indptr[row+1];
        n_this = ix2 - ix1;
        if (n_this == 1)
        {
            if (!ignore_zero_valued || above_min_decimals(values[ix1]))
            {
                output_file << (indices[ix1] + text_is_base1) << ':' << values[ix1];
                if (output_file.bad()) goto terminate_badly;
            }
        }

        else if (n_this > 1)
        {
            for (size_t ix = ix1; ix < ix2-1; ix++)
            {
                if (!ignore_zero_valued || above_min_decimals(values[ix]))
                {
                    output_file << (indices[ix] + text_is_base1) << ':' << values[ix] << ' ';
                    if (output_file.bad()) goto terminate_badly;
                }
            }
            if (!ignore_zero_valued || above_min_decimals(values[ix2-1]))
            {
                output_file << (indices[ix2-1] + text_is_base1) << ':' << values[ix2-1];
                if (output_file.bad()) goto terminate_badly;
            }
        }

        output_file << '\n';
        if (output_file.bad()) goto terminate_badly;
    }

    terminate_func:
    output_file.flags(oldflags);
    output_file.precision(oldprecision);
    return succeded;

    terminate_badly:
    succeded = false;
    goto terminate_func;
}

// template <class int_t=int64_t, class real_t=double>
template <class int_t, class real_t>
bool write_multi_label_template
(
    FILE *output_file,
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *restrict values,
    int_t *restrict indptr_lab,
    int_t *restrict indices_lab,
    int_t *restrict qid,
    const int_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    if (output_file == NULL)
    {
        throw_err("Error: invalid output_file.\n");
        return false;
    }

    if (decimal_places < 0)
    {
        throw_err("Error: 'decimal_places' cannot be negative.\n");
        return false;
    }

    int succeded;
    if (add_header)
    {
        #if defined(PLATFORM_IS_64_OR_HIGHER) && defined(HAS_Z_SPECIFIER)
        succeded = fprintf(output_file, "%zu %zu %zu\n",
                           nrows, ncols, nclasses);
        #else
        succeded = fprintf(output_file, "%" PRIu64 " %" PRIu64 " %" PRIu64 "\n",
                           (uint64_t)nrows, (uint64_t)ncols, (uint64_t)nclasses);
        #endif
        if (succeded < 0) return false;
    }

    if (nrows == 0)
        return true;

    if (sort_indices)
    {
        sort_sparse_indices(indptr, indices, values, nrows);
        sort_sparse_indices(indptr_lab, indices_lab, nrows);
    }
    
    #ifdef _MSC_VER
    #   pragma warning(push)
    #   pragma warning(disable:4244)
    #endif
    real_t thr_decimals = std::pow(10., (double)(-decimal_places)) / 2.;
    #ifdef _MSC_VER
    #   pragma warning(pop)
    #endif

    
    #if defined(HAS_FULL_STDIO)
    const char *format_specifier_spaced;
    const char *format_specifier;
    #else
    char format_specifier_spaced[23];
    char format_specifier[23];
    #endif
    const char *label_specifier_comma;
    const char *label_specifier;
    const char *qid_specifier;
    if (std::is_same<int_t, size_t>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%zu:%.*f ";
        format_specifier =  "%zu:%.*f";
        label_specifier_comma = "%zu,";
        label_specifier = "%zu ";
        qid_specifier = "qid:%zu ";
        #elif defined(HAS_Z_SPECIFIER)
        sprintf(format_specifier_spaced, "%%zu:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%zu:%%.%df", decimal_places);
        label_specifier_comma = "%zu,";
        label_specifier = "%zu ";
        qid_specifier = "qid:%zu ";
        #else
        sprintf(format_specifier_spaced, "%%" PRIu64 ":%%.%df ", decimal_places);
        sprintf(format_specifier, "%%" PRIu64 ":%%.%df", decimal_places);
        label_specifier_comma = "%" PRIu64 ",";
        label_specifier = "%" PRIu64  " ";
        qid_specifier = "qid:%" PRIu64 " ";
        #endif
    }
    else if (std::is_same<int_t, int64_t>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%" PRId64 ":%.*f ";
        format_specifier =  "%" PRId64 ":%.*f";
        #else
        sprintf(format_specifier_spaced, "%%" PRId64 ":%%.%df ", decimal_places);
        sprintf(format_specifier, "%%" PRId64 ":%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%" PRId64 ",";
        label_specifier = "%" PRId64  " ";
        qid_specifier = "qid:%" PRId64 " ";
    }
    else if (std::is_same<int_t, uint64_t>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%" PRIu64 ":%.*f ";
        format_specifier =  "%" PRIu64 ":%.*f";
        #else
        sprintf(format_specifier_spaced, "%%" PRIu64 ":%%.%df ", decimal_places);
        sprintf(format_specifier, "%%" PRIu64 ":%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%" PRIu64 ",";
        label_specifier = "%" PRIu64 " ";
        qid_specifier = "qid:%" PRId64 " ";
    }
    else if (std::is_same<int_t, long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%ld:%.*f ";
        format_specifier =  "%ld:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%ld:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%ld:%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%ld,";
        label_specifier = "%ld ";
        qid_specifier = "qid:%ld ";
    }
    else if (std::is_same<int_t, unsigned long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%lu:%.*f ";
        format_specifier =  "%lu:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%lu:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%lu:%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%lu,";
        label_specifier = "%lu ";
        qid_specifier = "qid:%lu ";
    }
    else if (std::is_same<int_t, long long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%lld:%.*f ";
        format_specifier =  "%lld:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%lld:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%lld:%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%lld,";
        label_specifier = "%lld ";
        qid_specifier = "qid:%lld ";
    }
    else if (std::is_same<int_t, unsigned long long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%llu:%.*f ";
        format_specifier =  "%llu:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%llu:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%llu:%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%llu,";
        label_specifier = "%llu ";
        qid_specifier = "qid:%llu ";
    } else if (std::is_same<int_t, unsigned int>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%u:%.*f ";
        format_specifier =  "%u:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%u:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%u:%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%u,";
        label_specifier = "%u ";
        qid_specifier = "qid:%u ";
    } else {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%d:%.*f ";
        format_specifier =  "%d:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%d:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%d:%%.%df", decimal_places);
        #endif
        label_specifier_comma = "%d,";
        label_specifier = "%d ";
        qid_specifier = "qid:%d ";
    }


    size_t n_this, ix1, ix2;
    for (size_t row = 0; row < nrows; row++)
    {
        ix1 = indptr_lab[row];
        ix2 = indptr_lab[row+1];
        n_this = ix2 - ix1;

        if (n_this == 0)
        {
            succeded = fprintf(output_file, " ");
            if (succeded < 0) goto throw_err;
        }

        else if (n_this == 1)
        {
            #ifdef HAS_Z_SPECIFIER
            succeded = fprintf(output_file, label_specifier,
                               indices_lab[ix1] + text_is_base1);
            #else
            if (std::is_same<int_t, size_t>::value)
                succeded = fprintf(output_file, label_specifier,
                                   (uint64_t)indices_lab[ix1] + text_is_base1);
            else
                succeded = fprintf(output_file, label_specifier,
                                   indices_lab[ix1] + text_is_base1);
            #endif
            if (succeded < 0) goto throw_err;
        }

        else if (n_this > 1)
        {
            for (size_t ix = ix1; ix < ix2-1; ix++) {
                #ifdef HAS_Z_SPECIFIER
                succeded = fprintf(output_file, label_specifier_comma,
                                   indices_lab[ix] + text_is_base1);
                #else
                if (std::is_same<int_t, size_t>::value)
                    succeded = fprintf(output_file, label_specifier_comma,
                                       (uint64_t)indices_lab[ix] + text_is_base1);
                else
                    succeded = fprintf(output_file, label_specifier_comma,
                                       indices_lab[ix] + text_is_base1);
                #endif
                if (succeded < 0) goto throw_err;
            }

            #ifdef HAS_Z_SPECIFIER
            succeded = fprintf(output_file, label_specifier,
                               indices_lab[ix2-1] + text_is_base1);
            #else
            if (std::is_same<int_t, size_t>::value)
                succeded = fprintf(output_file, label_specifier,
                                   (uint64_t)indices_lab[ix2-1] + text_is_base1);
            else
                succeded = fprintf(output_file, label_specifier,
                                   indices_lab[ix2-1] + text_is_base1);
            #endif
            if (succeded < 0) goto throw_err;
        }

        if (has_qid && qid[row] != missing_qid)
        {
            #ifdef HAS_Z_SPECIFIER
            succeded = fprintf(output_file, qid_specifier, qid[row]);
            #else
            if (std::is_same<int_t, size_t>::value)
                succeded = fprintf(output_file, qid_specifier, (uint64_t)qid[row]);
            else
                succeded = fprintf(output_file, qid_specifier, qid[row]);
            #endif
            if (succeded < 0) goto throw_err;
        }

        ix1 = indptr[row];
        ix2 = indptr[row+1];
        n_this = ix2 - ix1;
        if (n_this == 1)
        {
            if (!ignore_zero_valued || above_min_decimals(values[ix1]))
            {
                #if defined(HAS_FULL_STDIO)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix1] + text_is_base1, decimal_places, values[ix1]);
                #elif defined(HAS_Z_SPECIFIER)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix1] + text_is_base1, values[ix1]);
                #else
                if (std::is_same<int_t, size_t>::value)
                    succeded = fprintf(output_file, format_specifier,
                                       (uint64_t)indices[ix1] + text_is_base1, values[ix1]);
                else
                    succeded = fprintf(output_file, format_specifier,
                                       indices[ix1] + text_is_base1, values[ix1]);
                #endif
                if (succeded < 0) goto throw_err;
            }
        }

        else if (n_this > 1)
        {
            for (size_t ix = ix1; ix < ix2-1; ix++)
            {
                if (!ignore_zero_valued || above_min_decimals(values[ix]))
                {
                    #if defined(HAS_FULL_STDIO)
                    succeded = fprintf(output_file, format_specifier_spaced,
                                       indices[ix] + text_is_base1, decimal_places, values[ix]);
                    #elif defined(HAS_Z_SPECIFIER)
                    succeded = fprintf(output_file, format_specifier_spaced,
                                       indices[ix] + text_is_base1, values[ix]);
                    #else
                    if (std::is_same<int_t, size_t>::value)
                        succeded = fprintf(output_file, format_specifier_spaced,
                                           (uint64_t)indices[ix] + text_is_base1, values[ix]);
                    else
                        succeded = fprintf(output_file, format_specifier_spaced,
                                           indices[ix] + text_is_base1, values[ix]);
                    #endif
                    if (succeded < 0) goto throw_err;
                }
            }
            if (!ignore_zero_valued || above_min_decimals(values[ix2-1]))
            {
                #if defined(HAS_FULL_STDIO)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix2-1] + text_is_base1, decimal_places, values[ix2-1]);
                #elif defined(HAS_Z_SPECIFIER)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix2-1] + text_is_base1, values[ix2-1]);
                #else
                if (std::is_same<int_t, size_t>::value)
                    succeded = fprintf(output_file, format_specifier,
                                       (uint64_t)indices[ix2-1] + text_is_base1, values[ix2-1]);
                else
                    succeded = fprintf(output_file, format_specifier,
                                       indices[ix2-1] + text_is_base1, values[ix2-1]);
                #endif
                if (succeded < 0) goto throw_err;
            }
        }

        succeded = fprintf(output_file, "\n");
        if (succeded < 0) goto throw_err;
    }

    return true;

    throw_err:
    {
        print_errno();
        return false;
    }
}

// template <class int_t=int64_t, class real_t=double, class label_t=double>
template <class int_t, class real_t, class label_t>
bool write_single_label_template
(
    std::ostream &output_file,
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *restrict values,
    label_t *restrict labels,
    int_t *restrict qid,
    const int_t missing_qid,
    const label_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    if (output_file.fail())
    {
        throw_err("Error: invalid output_file.\n");
        return false;
    }

    if (decimal_places < 0)
    {
        throw_err("Error: 'decimal_places' cannot be negative.\n");
        return false;
    }

    if (nrows == 0 && !add_header)
       return true;

    bool succeded = true;
    #ifdef _MSC_VER
    #   pragma warning(push)
    #   pragma warning(disable:4244)
    #endif
    real_t thr_decimals = std::pow(10., (double)(-decimal_places)) / 2.;
    #ifdef _MSC_VER
    #   pragma warning(pop)
    #endif
    size_t n_this, ix1, ix2;
    bool label_is_num = std::is_same<label_t, double>::value ||
                        std::is_same<label_t, float>::value ||
                        std::is_same<label_t, long double>::value;

    /* https://stackoverflow.com/questions/8554441/how-to-output-with-3-digits-after-the-decimal-point-with-c-stream */
    std::ios_base::fmtflags oldflags = output_file.flags();
    std::streamsize oldprecision = output_file.precision();
    output_file << std::fixed << std::setprecision(decimal_places);

    if (add_header)
    {
        output_file << nrows << ' ' << ncols << ' ' << nclasses << '\n';
        if (output_file.bad()) goto terminate_badly;
    }

    if (nrows == 0)
        goto terminate_func;

    if (sort_indices)
    {
        sort_sparse_indices(indptr, indices, values, nrows);
    }

    for (size_t row = 0; row < nrows; row++)
    {
        if (label_is_num)
        {
            #ifndef _FOR_R
            if (!std::isnan((double)labels[row]))
            #else
            if (!ISNAN(labels[row]))
            #endif
            {
                output_file << labels[row];
                if (output_file.bad()) goto terminate_badly;
            }
        }

        else
        {
            if (labels[row] != missing_label)
            {
                output_file << labels[row];
                if (output_file.bad()) goto terminate_badly;
            }
        }

        output_file << ' ';
        if (output_file.bad()) goto terminate_badly;

        if (has_qid && qid[row] != missing_qid)
        {
            output_file << "qid:" << qid[row] << ' ';
            if (output_file.bad()) goto terminate_badly;
        }

        ix1 = indptr[row];
        ix2 = indptr[row+1];
        n_this = ix2 - ix1;
        if (n_this == 1)
        {
            if (!ignore_zero_valued || above_min_decimals(values[ix1]))
            {
                output_file << (indices[ix1] + text_is_base1) << ':' << values[ix1];
                if (output_file.bad()) goto terminate_badly;
            }
        }

        else if (n_this > 1)
        {
            for (size_t ix = ix1; ix < ix2-1; ix++)
            {
                if (!ignore_zero_valued || above_min_decimals(values[ix]))
                {
                    output_file << (indices[ix] + text_is_base1) << ':' << values[ix] << ' ';
                    if (output_file.bad()) goto terminate_badly;
                }
            }
            if (!ignore_zero_valued || above_min_decimals(values[ix2-1]))
            {
                output_file << (indices[ix2-1] + text_is_base1) << ':' << values[ix2-1];
                if (output_file.bad()) goto terminate_badly;
            }
        }

        output_file << '\n';
        if (output_file.bad()) goto terminate_badly;
    }

    terminate_func:
    output_file.flags(oldflags);
    output_file.precision(oldprecision);
    return succeded;

    terminate_badly:
    succeded = false;
    goto terminate_func;
}

// template <class int_t=int64_t, class real_t=double, class label_t=double>
template <class int_t, class real_t, class label_t>
bool write_single_label_template
(
    FILE *output_file,
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *restrict values,
    label_t *restrict labels,
    int_t *restrict qid,
    const int_t missing_qid,
    const label_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    if (output_file == NULL)
    {
        throw_err("Error: invalid output_file.\n");
        return false;
    }

    if (decimal_places < 0)
    {
        throw_err("Error: 'decimal_places' cannot be negative.\n");
        return false;
    }

    int succeded;
    if (add_header)
    {
        #if defined(PLATFORM_IS_64_OR_HIGHER) && defined(HAS_Z_SPECIFIER)
        succeded = fprintf(output_file, "%zu %zu %zu\n",
                           nrows, ncols, nclasses);
        #else
        succeded = fprintf(output_file, "%" PRIu64 " %" PRIu64 " %" PRIu64 "\n",
                           (uint64_t)nrows, (uint64_t)ncols, (uint64_t)nclasses);
        #endif
        if (succeded < 0) return false;
    }

    if (nrows == 0)
        return true;

    if (sort_indices)
    {
        sort_sparse_indices(indptr, indices, values, nrows);
    }

    #ifdef _MSC_VER
    #   pragma warning(push)
    #   pragma warning(disable:4244)
    #endif
    real_t thr_decimals = std::pow(10., (double)(-decimal_places)) / 2.;
    #ifdef _MSC_VER
    #   pragma warning(pop)
    #endif


    bool label_is_num = std::is_same<label_t, double>::value ||
                        std::is_same<label_t, float>::value ||
                        std::is_same<label_t, long double>::value;

    #if defined(HAS_FULL_STDIO)
    const char *format_specifier_spaced;
    const char *format_specifier;
    const char *label_specifier;
    #else
    char format_specifier_spaced[23];
    char format_specifier[23];
    char label_specifier[15];
    #endif
    const char *qid_specifier;
    if (std::is_same<int_t, size_t>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%zu:%.*f ";
        format_specifier =  "%zu:%.*f";
        qid_specifier = "qid:%zu ";
        #elif defined(HAS_Z_SPECIFIER)
        sprintf(format_specifier_spaced, "%%zu:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%zu:%%.%df", decimal_places);
        qid_specifier = "qid:%zu ";
        #else
        sprintf(format_specifier_spaced, "%%" PRIu64 ":%%.%df ", decimal_places);
        sprintf(format_specifier, "%%" PRIu64 ":%%.%df", decimal_places);
        qid_specifier = "qid:%" PRIu64 " ";
        #endif
    }
    else if (std::is_same<int_t, int64_t>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%" PRId64 ":%.*f ";
        format_specifier =  "%" PRId64 ":%.*f";
        #else
        sprintf(format_specifier_spaced, "%%" PRId64 ":%%.%df ", decimal_places);
        sprintf(format_specifier, "%%" PRId64 ":%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%" PRId64 " ";
    }
    else if (std::is_same<int_t, uint64_t>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%" PRIu64 ":%.*f ";
        format_specifier =  "%" PRIu64 ":%.*f";
        #else
        sprintf(format_specifier_spaced, "%%" PRIu64 ":%%.%df ", decimal_places);
        sprintf(format_specifier, "%%" PRIu64 ":%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%" PRId64 " ";
    }
    else if (std::is_same<int_t, long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%ld:%.*f ";
        format_specifier =  "%ld:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%ld:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%ld:%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%ld ";
    }
    else if (std::is_same<int_t, unsigned long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%lu:%.*f ";
        format_specifier =  "%lu:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%lu:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%lu:%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%lu ";
    }
    else if (std::is_same<int_t, long long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%lld:%.*f ";
        format_specifier =  "%lld:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%lld:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%lld:%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%lld ";
    }
    else if (std::is_same<int_t, unsigned long long>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%llu:%.*f ";
        format_specifier =  "%llu:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%llu:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%llu:%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%llu ";
    } else if (std::is_same<int_t, unsigned int>::value) {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%u:%.*f ";
        format_specifier =  "%u:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%u:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%u:%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%u ";
    } else {
        #if defined(HAS_FULL_STDIO)
        format_specifier_spaced = "%d:%.*f ";
        format_specifier =  "%d:%.*f";
        #else
        sprintf(format_specifier_spaced, "%%d:%%.%df ", decimal_places);
        sprintf(format_specifier, "%%d:%%.%df", decimal_places);
        #endif
        qid_specifier = "qid:%d ";
    }

    
    if (label_is_num) {
        #if defined(HAS_FULL_STDIO)
        if (std::is_same<label_t, long double>::value)
            label_specifier = "%.*Lf ";
        else
            label_specifier = "%.*f ";
        #else
        if (std::is_same<label_t, long double>::value)
            sprintf(label_specifier, "%%.%dLf ", decimal_places);
        else
            sprintf(label_specifier, "%%.%df ", decimal_places);
        #endif
    } else if (std::is_same<label_t, size_t>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%zu ";
        #elif defined(HAS_Z_SPECIFIER)
        sprintf(label_specifier, "%s", "%zu ");
        #else
        sprintf(label_specifier, "%s", "%" PRIu64 " ");
        #endif
    } else if (std::is_same<label_t, int64_t>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%" PRId64 " ";
        #else
        sprintf(label_specifier, "%s", "%" PRId64 " ");
        #endif
    } else if (std::is_same<label_t, uint64_t>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%" PRIu64 " ";
        #else
        sprintf(label_specifier, "%s", "%" PRIu64 " ");
        #endif
    } else if (std::is_same<label_t, long>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%ld ";
        #else
        sprintf(label_specifier, "%s", "%ld ");
        #endif
    } else if (std::is_same<label_t, long long>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%lld ";
        #else
        sprintf(label_specifier, "%s", "%lld ");
        #endif
    } else if (std::is_same<label_t, unsigned long>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%lu ";
        #else
        sprintf(label_specifier, "%s", "%lu ");
        #endif
    } else if (std::is_same<label_t, unsigned long long>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%llu ";
        #else
        sprintf(label_specifier, "%s", "%llu ");
        #endif
    } else if (std::is_same<label_t, unsigned int>::value) {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%u ";
        #else
        sprintf(label_specifier, "%s", "%u ");
        #endif
    } else {
        #if defined(HAS_FULL_STDIO)
        label_specifier = "%d ";
        #else
        sprintf(label_specifier, "%s", "%d ");
        #endif
    }

    size_t n_this, ix1, ix2;
    for (size_t row = 0; row < nrows; row++)
    {
        if (label_is_num)
        {
            #ifndef _FOR_R
            if (!std::isnan((double)labels[row]))
            #else
            if (!ISNAN(labels[row]))
            #endif
            {
                #ifdef HAS_FULL_STDIO
                #   if (defined(__GNUC__) || defined(__GNUG__)) && (__GNUC__ >= 5)
                #       pragma GCC diagnostic push
                #       pragma GCC diagnostic ignored "-Wformat"
                #   elif defined(__clang__)
                #       pragma clang diagnostic push
                #       pragma clang diagnostic ignored "-Wformat"
                #   endif
                succeded = fprintf(output_file, label_specifier, decimal_places, labels[row]);
                #   if (defined(__GNUC__) || defined(__GNUG__)) && (__GNUC__ >= 5)
                #       pragma GCC diagnostic pop
                #   elif defined(__clang__)
                #       pragma clang diagnostic pop
                #   endif
                #else
                #   if defined(_MSC_VER)
                #       pragma warning( push )
                #       pragma warning( disable : 4705 )
                #       pragma warning( disable : 4477 )
                #   endif
                succeded = fprintf(output_file, label_specifier, labels[row]);
                #   if defined(_MSC_VER)
                #       pragma warning( pop )
                #   endif
                #endif
            }

            else {
                succeded = fprintf(output_file, " ");
            }
        }

        else
        {
            if (labels[row] != missing_label)
            {
                #if defined(HAS_Z_SPECIFIER)
                succeded = fprintf(output_file, label_specifier, labels[row]);
                #else
                if (std::is_same<label_t, size_t>::value)
                    succeded = fprintf(output_file, label_specifier, (uint64_t)labels[row]);
                else
                    succeded = fprintf(output_file, label_specifier, labels[row]);
                #endif
            }

            else {
                succeded = fprintf(output_file, " ");
            }
        }

        if (succeded < 0) goto throw_err;

        if (has_qid && qid[row] != missing_qid)
        {
            #ifdef HAS_Z_SPECIFIER
            succeded = fprintf(output_file, qid_specifier, qid[row]);
            #else
            if (std::is_same<int_t, size_t>::value)
                succeded = fprintf(output_file, qid_specifier, (uint64_t)qid[row]);
            else
                succeded = fprintf(output_file, qid_specifier, qid[row]);
            #endif
            if (succeded < 0) goto throw_err;
        }

        ix1 = indptr[row];
        ix2 = indptr[row+1];
        n_this = ix2 - ix1;
        if (n_this == 1)
        {
            if (!ignore_zero_valued || above_min_decimals(values[ix1]))
            {
                #if defined(HAS_FULL_STDIO)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix1] + text_is_base1, decimal_places, values[ix1]);
                #elif defined(HAS_Z_SPECIFIER)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix1] + text_is_base1, values[ix1]);
                #else
                if (std::is_same<int_t, size_t>::value)
                    succeded = fprintf(output_file, format_specifier,
                                       (uint64_t)indices[ix1] + text_is_base1, values[ix1]);
                else
                    succeded = fprintf(output_file, format_specifier,
                                       indices[ix1] + text_is_base1, values[ix1]);
                #endif
                if (succeded < 0) goto throw_err;
            }
        }

        else if (n_this > 1)
        {
            for (size_t ix = ix1; ix < ix2-1; ix++)
            {
                if (!ignore_zero_valued || above_min_decimals(values[ix]))
                {
                    #if defined(HAS_FULL_STDIO)
                    succeded = fprintf(output_file, format_specifier_spaced,
                                       indices[ix] + text_is_base1, decimal_places, values[ix]);
                    #elif defined(HAS_Z_SPECIFIER)
                    succeded = fprintf(output_file, format_specifier_spaced,
                                       indices[ix] + text_is_base1, values[ix]);
                    #else
                    if (std::is_same<int_t, size_t>::value)
                        succeded = fprintf(output_file, format_specifier_spaced,
                                           (uint64_t)indices[ix] + text_is_base1, values[ix]);
                    else
                        succeded = fprintf(output_file, format_specifier_spaced,
                                           indices[ix] + text_is_base1, values[ix]);
                    #endif
                    if (succeded < 0) goto throw_err;
                }
            }
            if (!ignore_zero_valued || above_min_decimals(values[ix2-1]))
            {
                #if defined(HAS_FULL_STDIO)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix2-1] + text_is_base1, decimal_places, values[ix2-1]);
                #elif defined(HAS_Z_SPECIFIER)
                succeded = fprintf(output_file, format_specifier,
                                   indices[ix2-1] + text_is_base1, values[ix2-1]);
                #else
                if (std::is_same<int_t, size_t>::value)
                    succeded = fprintf(output_file, format_specifier,
                                       (uint64_t)indices[ix2-1] + text_is_base1, values[ix2-1]);
                else
                    succeded = fprintf(output_file, format_specifier,
                                       indices[ix2-1] + text_is_base1, values[ix2-1]);
                #endif
                if (succeded < 0) goto throw_err;
            }
        }

        succeded = fprintf(output_file, "\n");
        if (succeded < 0) goto throw_err;
    }

    return true;

    throw_err:
    {
        print_errno();
        return false;
    }
}
