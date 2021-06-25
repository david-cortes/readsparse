/* Generated through 'instantiate_templates.py', do not edit manually. */

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

#ifndef size_large
#   include <cstdint>
#   include <cstdint>
#   if SIZE_MAX >= UINT64_MAX
#       define size_large size_t
#   else
#       define size_large uint64_t
#   endif
#endif

#if defined(_FOR_PYTHON) || defined(_FOR_R) || !defined(_WIN32)
    #define EXPORTABLE 
#else
    #ifdef READSPARSE_COMPILE
        #define EXPORTABLE __declspec(dllexport)
    #else
        #define EXPORTABLE __declspec(dllimport)
    #endif
#endif

/* reader.cpp */

#ifdef _FOR_R

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

#elif defined(_FOR_PYTHON)

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

#else

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<size_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<size_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<size_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<size_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<size_t> &indptr_lab,
    std::vector<size_t> &indices_lab,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<int> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<size_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<size_t> &indptr_lab,
    std::vector<size_t> &indices_lab,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<int> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<size_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    FILE *input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<size_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &indptr_lab,
    std::vector<int> &indices_lab,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<size_t> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int> &indptr,
    std::vector<int> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<size_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &indptr_lab,
    std::vector<int64_t> &indices_lab,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<size_t> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<int64_t> &indptr,
    std::vector<int64_t> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<int64_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<size_t> &indptr_lab,
    std::vector<size_t> &indices_lab,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<int> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<int64_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<size_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<float> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<float> &values,
    std::vector<double> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_multi_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<size_t> &indptr_lab,
    std::vector<size_t> &indices_lab,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<int> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<int64_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<size_t> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<float> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

EXPORTABLE bool read_single_label
(
    std::istream &input_file,
    std::vector<size_t> &indptr,
    std::vector<size_t> &indices,
    std::vector<double> &values,
    std::vector<double> &labels,
    std::vector<size_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

#endif /* _FOR_R, _FOR_PYTHON */

/* writer.cpp */

#ifdef _FOR_R

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    int *indptr_lab,
    int *indices_lab,
    int *qid,
    const int missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    int *labels,
    int *qid,
    const int missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    double *labels,
    int *qid,
    const int missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    int *indptr_lab,
    int *indices_lab,
    int *qid,
    const int missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    int *labels,
    int *qid,
    const int missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    double *labels,
    int *qid,
    const int missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

#else

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    float *values,
    int *indptr_lab,
    int *indices_lab,
    int *qid,
    const int missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    float *values,
    int *labels,
    int *qid,
    const int missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    float *values,
    int64_t *labels,
    int *qid,
    const int missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    float *values,
    size_t *labels,
    int *qid,
    const int missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    float *values,
    float *labels,
    int *qid,
    const int missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    float *values,
    double *labels,
    int *qid,
    const int missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    int *indptr_lab,
    int *indices_lab,
    int *qid,
    const int missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    int *labels,
    int *qid,
    const int missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    int64_t *labels,
    int *qid,
    const int missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    size_t *labels,
    int *qid,
    const int missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    float *labels,
    int *qid,
    const int missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int *indptr,
    int *indices,
    double *values,
    double *labels,
    int *qid,
    const int missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    int64_t *indptr_lab,
    int64_t *indices_lab,
    int64_t *qid,
    const int64_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    int *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    int64_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    size_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    float *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    double *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    int64_t *indptr_lab,
    int64_t *indices_lab,
    int64_t *qid,
    const int64_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    int *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    int64_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    size_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    float *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    double *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    size_t *indptr_lab,
    size_t *indices_lab,
    size_t *qid,
    const size_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    int *labels,
    size_t *qid,
    const size_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    int64_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    size_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    float *labels,
    size_t *qid,
    const size_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    double *labels,
    size_t *qid,
    const size_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    size_t *indptr_lab,
    size_t *indices_lab,
    size_t *qid,
    const size_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    int *labels,
    size_t *qid,
    const size_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    int64_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    size_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    float *labels,
    size_t *qid,
    const size_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    FILE *output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    double *labels,
    size_t *qid,
    const size_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    float *values,
    int *indptr_lab,
    int *indices_lab,
    int *qid,
    const int missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    float *values,
    int *labels,
    int *qid,
    const int missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    float *values,
    int64_t *labels,
    int *qid,
    const int missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    float *values,
    size_t *labels,
    int *qid,
    const int missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    float *values,
    float *labels,
    int *qid,
    const int missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    float *values,
    double *labels,
    int *qid,
    const int missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    int *indptr_lab,
    int *indices_lab,
    int *qid,
    const int missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    int *labels,
    int *qid,
    const int missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    int64_t *labels,
    int *qid,
    const int missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    size_t *labels,
    int *qid,
    const int missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    float *labels,
    int *qid,
    const int missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int *indptr,
    int *indices,
    double *values,
    double *labels,
    int *qid,
    const int missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    int64_t *indptr_lab,
    int64_t *indices_lab,
    int64_t *qid,
    const int64_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    int *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    int64_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    size_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    float *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    float *values,
    double *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    int64_t *indptr_lab,
    int64_t *indices_lab,
    int64_t *qid,
    const int64_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    int *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    int64_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    size_t *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    float *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    int64_t *indptr,
    int64_t *indices,
    double *values,
    double *labels,
    int64_t *qid,
    const int64_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    size_t *indptr_lab,
    size_t *indices_lab,
    size_t *qid,
    const size_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    int *labels,
    size_t *qid,
    const size_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    int64_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    size_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    float *labels,
    size_t *qid,
    const size_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    float *values,
    double *labels,
    size_t *qid,
    const size_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    size_t *indptr_lab,
    size_t *indices_lab,
    size_t *qid,
    const size_t missing_qid,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    int *labels,
    size_t *qid,
    const size_t missing_qid,
    const int missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    int64_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const int64_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    size_t *labels,
    size_t *qid,
    const size_t missing_qid,
    const size_t missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    float *labels,
    size_t *qid,
    const size_t missing_qid,
    const float missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
    size_t *indptr,
    size_t *indices,
    double *values,
    double *labels,
    size_t *qid,
    const size_t missing_qid,
    const double missing_label,
    const bool has_qid,
    const size_large nrows,
    const size_large ncols,
    const size_large nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

#endif /* _FOR_R */
