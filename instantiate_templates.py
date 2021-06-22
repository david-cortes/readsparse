### This file generates C++ code to instantiate the
### templated functions in all the supported types.
import re
from copy import deepcopy

info_header = """/* Generated through 'instantiate_templates.py', do not edit manually. */
"""

license_info = """
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
"""

msvc_export = """
#if defined(_FOR_PYTHON) || defined(_FOR_R) || !defined(_WIN32)
    #define EXPORTABLE 
#else
    #ifdef READSPARSE_COMPILE
        #define EXPORTABLE __declspec(dllexport)
    #else
        #define EXPORTABLE __declspec(dllimport)
    #endif
#endif
"""

untyped_reader_multi_label = """
EXPORTABLE bool read_multi_label
(
    <stream_t>input_file,
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    std::vector<int_t> &indptr_lab,
    std::vector<int_t> &indices_lab,
    std::vector<int_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
)
{
    return read_multi_label_template(
        input_file,
        indptr,
        indices,
        values,
        indptr_lab,
        indices_lab,
        qid,
        nrows,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        assume_no_qid,
        assume_trailing_ws
    );
}
"""

untyped_reader_single_label = """
EXPORTABLE bool read_single_label
(
    <stream_t>input_file,
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    std::vector<label_t_> &labels,
    std::vector<int_t> &qid,
    size_large &nrows,
    size_large &ncols,
    size_large &nclasses,
    const bool ignore_zero_valued = true,
    const bool sort_indices = true,
    const bool text_is_base1 = true,
    const bool assume_no_qid = true,
    const bool assume_trailing_ws = true
)
{
    return read_single_label_template(
        input_file,
        indptr,
        indices,
        values,
        labels,
        qid,
        nrows,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        assume_no_qid,
        assume_trailing_ws
    );
}
"""

untyped_writer_multi_label = """
EXPORTABLE bool write_multi_label
(
    <stream_t>output_file,
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
"""

untyped_writer_single_label = """
EXPORTABLE bool write_single_label
(
    <stream_t>output_file,
    int_t *indptr,
    int_t *indices,
    real_t *values,
    label_t_ *labels,
    int_t *qid,
    const int_t missing_qid,
    const label_t_ missing_label,
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
"""

size_def = """
#ifndef size_large
#   include <cstdint>
#   include <cstdint>
#   if SIZE_MAX >= UINT64_MAX
#       define size_large size_t
#   else
#       define size_large uint64_t
#   endif
#endif
"""

with open("src/readsparse_detemplated.hpp", "w") as f_dt:
    f_dt.write(info_header)
    f_dt.write(license_info)
    f_dt.write(size_def)
    f_dt.write(msvc_export)

    f_dt.write("\n/* reader.cpp */\n")
    with open("src/reader.cpp", "w") as f:
        ### Beginning
        f.write(info_header)
        f.write(license_info)
        f.write(msvc_export)
        f.write("\n#include \"reader.hpp\"\n")

        ### For R
        f.write("\n#ifdef _FOR_R\n")
        f_dt.write("\n#ifdef _FOR_R\n")
        for stream_t in ["FILE *", "std::istream &"]:
            for int_t in ["int"]:
                for real_t in ["double"]:
                    func_write = deepcopy(untyped_reader_multi_label)
                    func_write = func_write.replace("<stream_t>", stream_t)
                    func_write = func_write.replace("int_t", int_t)
                    func_write = func_write.replace("real_t", real_t)
                    f.write(func_write)
                    f_dt.write(func_write.split(")")[0] + ");\n")

                    for label_t_ in ["double"]:
                        func_write = deepcopy(untyped_reader_single_label)
                        func_write = func_write.replace("<stream_t>", stream_t)
                        func_write = func_write.replace("int_t", int_t)
                        func_write = func_write.replace("real_t", real_t)
                        func_write = func_write.replace("label_t_", label_t_)
                        f.write(func_write)
                        f_dt.write(func_write.split(")")[0] + ");\n")

        ### For Python
        f.write("\n#elif defined(_FOR_PYTHON)\n")
        f_dt.write("\n#elif defined(_FOR_PYTHON)\n")
        for stream_t in ["FILE *", "std::istream &"]:
            for int_t in ["int", "int64_t"]:
                for real_t in ["float", "double"]:
                    func_write = deepcopy(untyped_reader_multi_label)
                    func_write = func_write.replace("<stream_t>", stream_t)
                    func_write = func_write.replace("int_t", int_t)
                    func_write = func_write.replace("real_t", real_t)
                    f.write(func_write)
                    f_dt.write(func_write.split(")")[0] + ");\n")

                    for label_t_ in ["float", "double"]:
                        func_write = deepcopy(untyped_reader_single_label)
                        func_write = func_write.replace("<stream_t>", stream_t)
                        func_write = func_write.replace("int_t", int_t)
                        func_write = func_write.replace("real_t", real_t)
                        func_write = func_write.replace("label_t_", label_t_)
                        f.write(func_write)
                        f_dt.write(func_write.split(")")[0] + ");\n")

        ### For C++
        f.write("\n#else\n")
        f_dt.write("\n#else\n")
        for stream_t in ["FILE *", "std::istream &"]:
            for int_t in ["int", "int64_t", "size_t"]:
                for real_t in ["float", "double"]:
                    func_write = deepcopy(untyped_reader_multi_label)
                    func_write = func_write.replace("<stream_t>", stream_t)
                    func_write = func_write.replace("int_t", int_t)
                    func_write = func_write.replace("real_t", real_t)
                    f.write(func_write)
                    f_dt.write(func_write.split(")")[0] + ");\n")

                    for label_t_ in ["int", "int64_t", "size_t", "float", "double"]:
                        func_write = deepcopy(untyped_reader_single_label)
                        func_write = func_write.replace("<stream_t>", stream_t)
                        func_write = func_write.replace("int_t", int_t)
                        func_write = func_write.replace("real_t", real_t)
                        func_write = func_write.replace("label_t_", label_t_)
                        f.write(func_write)
                        f_dt.write(func_write.split(")")[0] + ");\n")

        f.write("\n#endif /* _FOR_R, _FOR_PYTHON */\n")
        f_dt.write("\n#endif /* _FOR_R, _FOR_PYTHON */\n")

    f_dt.write("\n/* writer.cpp */\n")
    with open("src/writer.cpp", "w") as f:
        ### Beginning
        f.write(info_header)
        f.write(license_info)
        f.write(msvc_export)
        f.write("\n#include \"writer.hpp\"\n")

        ### For R
        f.write("\n#ifdef _FOR_R\n")
        f_dt.write("\n#ifdef _FOR_R\n")
        for stream_t in ["FILE *", "std::ostream &"]:
            for int_t in ["int"]:
                for real_t in ["double"]:
                    func_write = deepcopy(untyped_writer_multi_label)
                    func_write = func_write.replace("<stream_t>", stream_t)
                    func_write = func_write.replace("int_t", int_t)
                    func_write = func_write.replace("real_t", real_t)
                    f.write(func_write)
                    f_dt.write(func_write.split(")")[0] + ");\n")

                    for label_t_ in ["int", "double"]:
                        func_write = deepcopy(untyped_writer_single_label)
                        func_write = func_write.replace("<stream_t>", stream_t)
                        func_write = func_write.replace("int_t", int_t)
                        func_write = func_write.replace("real_t", real_t)
                        func_write = func_write.replace("label_t_", label_t_)
                        f.write(func_write)
                        f_dt.write(func_write.split(")")[0] + ");\n")

        ### For Python and C++
        f.write("\n#else\n")
        f_dt.write("\n#else\n")
        for stream_t in ["FILE *", "std::ostream &"]:
            for int_t in ["int", "int64_t", "size_t"]:
                for real_t in ["float", "double"]:
                    func_write = deepcopy(untyped_writer_multi_label)
                    func_write = func_write.replace("<stream_t>", stream_t)
                    func_write = func_write.replace("int_t", int_t)
                    func_write = func_write.replace("real_t", real_t)
                    f.write(func_write)
                    f_dt.write(func_write.split(")")[0] + ");\n")

                    for label_t_ in ["int", "int64_t", "size_t", "float", "double"]:
                        func_write = deepcopy(untyped_writer_single_label)
                        func_write = func_write.replace("<stream_t>", stream_t)
                        func_write = func_write.replace("int_t", int_t)
                        func_write = func_write.replace("real_t", real_t)
                        func_write = func_write.replace("label_t_", label_t_)
                        f.write(func_write)
                        f_dt.write(func_write.split(")")[0] + ");\n")

        f.write("\n#endif /* _FOR_R */\n")
        f_dt.write("\n#endif /* _FOR_R */\n")
