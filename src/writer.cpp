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

#if defined(_FOR_PYTHON) || defined(_FOR_R) || !defined(_WIN32)
    #define EXPORTABLE 
#else
    #ifdef READSPARSE_COMPILE
        #define EXPORTABLE __declspec(dllexport)
    #else
        #define EXPORTABLE __declspec(dllimport)
    #endif
#endif

#include "writer.hpp"

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_multi_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

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
)
{
    return write_single_label_template(
        output_file,
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
}

#endif /* _FOR_R */
