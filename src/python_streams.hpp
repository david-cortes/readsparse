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
#ifdef _FOR_PYTHON

#pragma once
#include "readsparse_internal.hpp"
#include "readsparse_detemplated.hpp"
#include <sstream>

template <class int_t=int64_t, class real_t=double>
bool read_multi_label_str
(
    std::string &input_str,
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
    std::basic_stringstream<char> ss;
    ss.str(input_str);
    return read_multi_label(
        ss,
        indptr,
        indices,
        values,
        indptr_lab,
        indices_lab,
        qid,
        nrows,
        ncols,
        nclasses,
        limit_nrows,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        assume_no_qid,
        assume_trailing_ws
    );
}

template <class int_t=int64_t, class real_t=double>
std::string write_multi_label_str
(
    int_t *indptr,
    int_t *indices,
    real_t *values,
    int_t *indptr_lab,
    int_t *indices_lab,
    int_t *qid,
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
    std::basic_stringstream<char> ss;
    bool succeeded = write_multi_label(
        ss,
        indptr,
        indices,
        values,
        indptr_lab,
        indices_lab,
        qid,
        missing_qid,
        has_qid,
        nrows,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (!succeeded)
        return std::string("e");
    return ss.str();
}

template <class int_t=int64_t, class real_t=double, class label_t=double>
bool read_single_label_str
(
    std::string &input_str,
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
    std::basic_stringstream<char> ss;
    ss.str(input_str);
    return read_single_label(
        ss,
        indptr,
        indices,
        values,
        labels,
        qid,
        nrows,
        ncols,
        nclasses,
        limit_nrows,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        assume_no_qid,
        assume_trailing_ws
    );
}

template <class int_t=int64_t, class real_t=double, class label_t=double>
std::string write_single_label_str
(
    int_t *indptr,
    int_t *indices,
    real_t *values,
    label_t *labels,
    int_t *qid,
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
    std::basic_stringstream<char> ss;
    bool succeeded = write_single_label(
        ss,
        indptr,
        indices,
        values,
        labels,
        qid,
        missing_qid,
        missing_label,
        has_qid,
        nrows,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (!succeeded)
        return std::string("e");
    return ss.str();
}

#endif 
