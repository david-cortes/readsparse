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

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdint>
#include <cinttypes>
#ifndef size_t
#   include <stddef.h>
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

/*  Functions for reading and writing sparse CSR matrices in text format.
    See SVMLight's webpage for some details about the format:
        http://svmlight.joachims.org
    
    Datasets in this format can be downloaded from:
        - LibSVM datasets:
            https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets
        - Extreme Classification Repository:
            http://manikvarma.org/downloads/XC/XMLRepository.html

    The function templates below will be available for the following types:
        - int_t : [int, int64_t, size_t]
        - real_t : [float, double]
        - label_t : [int, int64_t, size_t, float, double]

    Each function has a variant that uses C++ streams and another that uses C file pointers.
    The ones with C file pointers are faster, but might be less robust to errors.
    Note that the code paths are different and might produce slightly different
    results for the same file or same input.

    The functions assume the inputs and outputs are a sparse matrix 'X' in CSR format,
    and either a vector of labels or a binary CSR matrix of labels 'y'. In the case
    of CSR labels, only column indices are used, not values.

    In general, a CSR matrix consists of the following structure:
        - indptr is an array of size=nrows+1, with each entry denoting
          at which position [row] where does each row start in the
          indices and data arrays, and at position [row+1] where does
          the row end.
        - indices is an array of size=nnz containing the non-missing
          column indices, with rows delimited by indptr.
        - values is an array of size=nnz which has the associated values
          for each entry in indices.

    Parameters
    ==========
    - input_file
        Input stream, either as an input stream, or as a file pointer in non-binary read mode.
    - output_file
        Output stream, either as an output stream, or as a file pointer in non-binary write mode.
    - indptr
        Array/vector of size nrows+1, containing the index pointer for the 'X' matrix.
        For the reading functions, this is an output variable and the data that it
        contains will be overwritten.
    - indices
        Array/vector of size nnz, containing the non-missing column indices for the
        'X' matrix. For the reading functions, this is an output variable and the
        data that it contains will be overwritten.
    - values
        Array/vector of size nnz, containing the associated values to each entry
        in 'indices'. For the reading functions, this is an output variable and the
        data that it contains will be overwritten.
    - labels
        Array/vector of size nrows, containing the 'y' values (labels for each row
        in "X'"). For the reading functions, this is an output variable and the
        data that it contains will be overwritten.
    - indptr_lab
        Array/vector of size nrows+1, containing the indptr of the 'y' values.
        For the reading functions, this is an output variable and the
        data that it contains will be overwritten.
    - indices_lab
        Array/vector of size nnz_y, containing the indices of the 'y' values.
        For the reading functions, this is an output variable and the
        data that it contains will be overwritten.
    - qid
        Optional rray/vector of size nrows. This will contain secondary label
        information used for ranking under 'qid' tag. Will not be read unless passing
        'assume_no_qid=false', and will not be written unless passing 'has_qid=true'.
        For the reading functions, this is an output variable and the
        data that it contains will be overwritten.
    - missing_qid
        Value that denotes that a given qid is missing. Missing qids will not
        be written into the data (such row will now have qid:<value>). When
        reading the data, missing qids will have value SIZE_MAX for size_t
        types and -INT_MAX for other types. Note that in general, other software
        does not work with a mixture of missing and non-missing qid.
    - missing_label
        Value that denotes that a given label is missing, when the labels are
        integer types (for numeric types, will be assumed that NAN means missing).
        Missing labels will not be written into the data, so those rows will start
        with a space. When reading the data, missing labels will have a value of
        NAN for numeric types, SIZE_MAX for size_t, and -INT_MAX for other types.
        Note that, in general, other software does not work with missing labels.
    - has_qid
        Whether the input data has qid field. If passing 'has_qid=false', will
        not write the qid. See 'assume_no_qid' for the reading functions.
    - nrows
        Number of rows in the data. When reading the data, this is an output variable.
        When writing the data, it is a non-optional input variable.
        Note that, on 32-bit systems and lower, this variable will be an unsigned 64-bit
        integer, but the number or rows cannot exceed the maximum value for size_t
        (SIZE_MAX, which is 2^32-1 on 32-bit systems). The number of columns and classes
        can however still be larger.
    - ncols
        Number of columns in the data. When reading the data, this is an output variable,
        and if the data has a header row that suggests a larger number of columns,
        this larger number is what will be output. When writing the data, this is
        optional and only used when passing 'add_header=true'.
    - nclasses
        Number of classes in the 'y' data. When reading the data, this is an output variable,
        and if the data has a header row that suggests a larger number of classes,
        this larger number is what will be output. When writing the data, this is
        optional and only used when passing 'add_header=true'.
    - ignore_zero_valued
        Whether to ignore features that have a value of zero. When writing the data,
        this will be determined heuristically by std::abs(x) >= pow(10., -decimal_places)/2.,
        which might not match with how the libc/libc++ functions round them.
    - sort_indices
        Whether to sort the column indices in the CSR matrix after reading or before writing.
        When writing, the input data will be modified in-place. It is recommended
        to pass true as other software might assume that they are sorted.
    - text_is_base1
        Whether the indices in the text have or should have numeration starting at 1.
        The library uses 0-based numeration, so if passing 'text_is_base1=true',
        it will subtract minus 1 from the indices after reading the data
        (unless it finds an index of zero or less), and will add +1 to the indices
        when writing. Most software assumes the text is base-1.
    - assume_no_qid
        Whether to assume that the data has a qid field. If passing false and the
        data does turn out to have qid, the features will not be read. See
        'has_qid' for the writing functions.
     - assume_trailing_ws
        Whether to assume that lines in the input can have extra whitespaces at the
        end before a newline. For large files which do not have any extra whitespace
        at the end, parsing with this set to 'false' is typically 1.5x faster,
        but if the file does turn out to have e.g. spaces at the end, the result
        will be incorrect.
    - add_header
        Whether to add a header with metadata (number of rows, columns, and classes)
        as the first row. The reading functions will automatically detect if the
        data has a header as first row.
    - decimal_places
        Number of decimal places after the point with which to write the numeric
        values. This applies also to labels when they are numeric types.
        Note that the values will be rounded.

    Returns
    =======
    success
        Will return 'true' if the operation completes successfully, 'false' otherwise.
        When it fails, it might throw an error message to stderr, but will not
        be able to tell what failed exactly.
*/


/* Note: the functions here are not really templated, but are rather pre-compiled
   for all possible combinations of valid types (see documentation above for valid
   types for each template).
   The header includes at the end a longer header with the de-templated prototypes,
   which is automatically generated.
   Thus, do not rely on the default arguments when using this header. These are
   only for documentation purposes as suggested values. */


#if SIZE_MAX >= UINT64_MAX
#   define size_large size_t
#else
#   define size_large uint64_t
#endif

template <class int_t=int64_t, class real_t=double, class label_t=double>
EXPORTABLE bool read_single_label
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
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

template <class int_t=int64_t, class real_t=double, class label_t=double>
EXPORTABLE bool read_single_label
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
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

template <class int_t=int64_t, class real_t=double>
EXPORTABLE bool read_multi_label
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
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

template <class int_t=int64_t, class real_t=double> 
EXPORTABLE bool read_multi_label
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
    const size_t limit_nrows = 0,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
);

template <class int_t=int64_t, class real_t=double, class label_t=double>
EXPORTABLE bool write_single_label
(
    std::ostream &output_file,
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
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

template <class int_t=int64_t, class real_t=double, class label_t=double>
EXPORTABLE bool write_single_label
(
    FILE *output_file,
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
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

template <class int_t=int64_t, class real_t=double>
EXPORTABLE bool write_multi_label
(
    std::ostream &output_file,
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
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

template <class int_t=int64_t, class real_t=double>
EXPORTABLE bool write_multi_label
(
    FILE *output_file,
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
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool add_header = false,
    const int decimal_places = 8
);

#include "readsparse_detemplated.hpp"
