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

/* https://stackoverflow.com/questions/14535556/why-doesnt-priu64-work-in-this-code */
#if !defined(__cplusplus) && !defined(__STDC_FORMAT_MACROS)
#   define __STDC_FORMAT_MACROS 1
#endif

#if (defined(_WIN32) || defined(_WIN64)) && (defined(__GNUG__) || defined(__GNUC__)) && (SIZE_MAX > UINT32_MAX) && !defined(_FILE_OFFSET_BITS)
#   define _FILE_OFFSET_BITS 64 /* https://stackoverflow.com/questions/16696297/ftell-at-a-position-past-2gb */
#endif

#include <cinttypes>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <limits.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>
#include <type_traits>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#ifdef _FOR_R
#   include <Rinternals.h>
#endif

#ifndef PRId64
#   if (defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
#       define PRId64 "I64d"
#   else
#       define PRId64 "lld"
#   endif
#endif
#ifndef PRIu64
#   if (defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER))
#       define PRIu64 "I64u"
#   else
#       define PRId64 "llu"
#   endif
#endif
#ifndef SCNd64
#   define SCNd64 PRId64
#endif
#ifndef SCNu64
#   define SCNu64 PRIu64
#endif


#if !(defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)) || \
     (defined(__GNUG__) && defined(__USE_MINGW_ANSI_STDIO) && (__USE_MINGW_ANSI_STDIO != 0)) &&\
    !defined(AVOID_MINGW_ANSI_STDIO)
#   define HAS_FULL_STDIO
#endif
#if defined(HAS_FULL_STDIO) || (defined(_MSC_VER) && (_MSC_VER >= 1800))
#   define HAS_Z_SPECIFIER
#endif


#if (defined(_WIN32) || defined(_WIN64)) && defined(__GNUG__)
#   define COMPILER_IS_MINGW
#endif

#if (SIZE_MAX > UINT32_MAX)
#   define PLATFORM_IS_64_OR_HIGHER
#endif

/* Aliasing for compiler optimizations */
#if defined(__GNUG__) || defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER) || defined(SUPPORTS_RESTRICT)
    #define restrict __restrict
#else
    #define restrict 
#endif

#if SIZE_MAX >= UINT64_MAX
#   define size_large size_t
#else
#   define size_large uint64_t
#endif

/* utils.hpp */
template <class int_t>
bool check_is_sorted(int_t* vec, size_t n);

static void print_errno();

template <class int_t, class real_t>
void sort_sparse_indices
(
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *values,
    size_t nrows,
    bool has_values
);

template <class int_t, class real_t>
void sort_sparse_indices
(
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values
);

template <class int_t>
void sort_sparse_indices
(
    int_t *indptr,
    int_t *indices,
    size_t nrows
);

template <class int_t, class real_t>
void sort_sparse_indices
(
    int_t *indptr,
    int_t *indices,
    real_t *values,
    size_t nrows
);

template <class int_t, class real_t>
void sort_sparse_indices_known_ncol
(
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *values,
    size_t nrows, size_t ncol,
    bool has_values
);

template <class int_t, class real_t>
void sort_sparse_indices_known_ncol
(
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values
);

template <class int_t>
void sort_sparse_indices_known_ncol
(
    int_t *indptr,
    int_t *indices,
    size_t nrows, size_t ncol
);

template <class int_t, class real_t>
void sort_sparse_indices_known_ncol
(
    int_t *indptr,
    int_t *indices,
    real_t *values,
    size_t nrows, size_t ncol
);

/* reader.hpp */

template <class int_t>
void subtract_one_from_vec(std::vector<int_t> &vec);

template <class int_t>
size_t find_largest_val(std::vector<int_t> &vec, int_t missing_label);

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
    size_t &nrows,
    size_t &ncols,
    size_t &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
);

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
    size_t &nrows,
    size_t &ncols,
    size_t &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
);

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
    size_t &nrows,
    size_t &ncols,
    size_t &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
);

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
    size_t &nrows,
    size_t &ncols,
    size_t &nclasses,
    const size_t limit_nrows,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid,
    const bool assume_trailing_ws
);

/* writer.hpp */

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
    const size_t nrows,
    const size_t ncols,
    const size_t nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
);

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
    const size_t nrows,
    const size_t ncols,
    const size_t nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
);

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
    const size_t nrows,
    const size_t ncols,
    const size_t nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
);

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
    const size_t nrows,
    const size_t ncols,
    const size_t nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
);
