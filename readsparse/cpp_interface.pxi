#cython: language_level=3
import numpy as np
cimport numpy as np

from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool as bool_t
from libc.stdint cimport int64_t, uint64_t, SIZE_MAX, UINT64_MAX
from libc.stdio cimport FILE, fopen, fclose, snprintf
from libc.string cimport memcpy, strerror
from libc.stddef cimport size_t
from libc.limits cimport INT_MAX
from libc.errno cimport errno
from cython cimport boundscheck, nonecheck, wraparound

import ctypes

ctypedef fused int_t:
    int
    int64_t
    size_t

ctypedef fused real_t:
    float
    double

ctypedef fused label_t:
    int
    int64_t
    size_t
    float
    double

def py_throw_err():
    cdef np.ndarray[char, ndim=1] str_buffer = np.empty(1000, dtype=ctypes.c_char)
    cdef int nchar = snprintf(&str_buffer[0], 999, "Error %d: %s\n", errno, strerror(errno))
    cdef string s = string(&str_buffer[0], nchar)
    cdef str msg = s.decode()
    raise ValueError(msg)


cdef FILE* cy_fopen(str fname, bool_t read, bool_t append):
    cdef bytes fname_py = fname.encode()
    cdef char* fname_c = fname_py
    cdef char* mode
    if (read):
        mode = b"r"
    elif (append):
        mode = b"a"
    else:
        mode = b"w"
    cdef FILE *out = fopen(fname_c, mode)
    return out

cdef extern from "reader.hpp":
    bool_t read_single_label_template[int_t_, real_t_, label_t_](
        FILE *input_file,
        vector[int_t_] &indptr,
        vector[int_t_] &indices,
        vector[real_t_] &values,
        vector[label_t_] &labels,
        vector[int_t_] &qid,
        size_large &nrows,
        size_large &ncols,
        size_large &nclasses,
        const size_t limit_nrows,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t assume_no_qid,
        const bool_t assume_trailing_ws
    ) except + nogil

    bool_t read_multi_label_template[int_t_, real_t_](
        FILE *input_file,
        vector[int_t_] &indptr,
        vector[int_t_] &indices,
        vector[real_t_] &values,
        vector[int_t_] &indptr_lab,
        vector[int_t_] &indices_lab,
        vector[int_t_] &qid,
        size_large &nrows,
        size_large &ncols,
        size_large &nclasses,
        const size_t limit_nrows,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t assume_no_qid,
        const bool_t assume_trailing_ws
    ) except + nogil

cdef extern from "writer.hpp":

    bool_t write_single_label_template[int_t_, real_t_, label_t_](
        FILE *output_file,
        int_t_ *indptr,
        int_t_ *indices,
        real_t_ *values,
        label_t_ *labels,
        int_t_ *qid,
        const int_t_ missing_qid,
        const label_t_ missing_label,
        const bool_t has_qid,
        const size_large nrows,
        const size_large ncols,
        const size_large nclasses,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t add_header,
        const int decimal_places
    ) except + nogil

    bool_t write_multi_label_template[int_t_, real_t_](
        FILE *output_file,
        int_t_ *indptr,
        int_t_ *indices,
        real_t_ *values,
        int_t_ *indptr_lab,
        int_t_ *indices_lab,
        int_t_ *qid,
        const int_t_ missing_qid,
        const bool_t has_qid,
        const size_large nrows,
        const size_large ncols,
        const size_large nclasses,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t add_header,
        const int decimal_places
    ) except + nogil


cdef extern from "python_streams.hpp":
    bool_t read_multi_label_str[int_t_, real_t_](
        string &input_str,
        vector[int_t_] &indptr,
        vector[int_t_] &indices,
        vector[real_t_] &values,
        vector[int_t_] &indptr_lab,
        vector[int_t_] &indices_lab,
        vector[int_t_] &qid,
        size_large &nrows,
        size_large &ncols,
        size_large &nclasses,
        const size_t limit_nrows,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t assume_no_qid,
        const bool_t assume_trailing_ws
    ) except + nogil

    string write_multi_label_str[int_t_, real_t_](
        int_t_ *indptr,
        int_t_ *indices,
        real_t_ *values,
        int_t_ *indptr_lab,
        int_t_ *indices_lab,
        int_t_ *qid,
        const int_t_ missing_qid,
        const bool_t has_qid,
        const size_large nrows,
        const size_large ncols,
        const size_large nclasses,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t add_header,
        const int decimal_places
    ) except + nogil

    bool_t read_single_label_str[int_t_, real_t_, label_t_](
        string &input_str,
        vector[int_t_] &indptr,
        vector[int_t_] &indices,
        vector[real_t_] &values,
        vector[label_t_] &labels,
        vector[int_t_] &qid,
        size_large &nrows,
        size_large &ncols,
        size_large &nclasses,
        const size_t limit_nrows,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t assume_no_qid,
        const bool_t assume_trailing_ws
    ) except + nogil

    string write_single_label_str[int_t_, real_t_, label_t_](
        int_t_ *indptr,
        int_t_ *indices,
        real_t_ *values,
        label_t_ *labels,
        int_t_ *qid,
        const int_t_ missing_qid,
        const label_t_ missing_label,
        const bool_t has_qid,
        const size_large nrows,
        const size_large ncols,
        const size_large nclasses,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t add_header,
        const int decimal_places
    ) except + nogil

    bool_t read_multi_label_filestream[int_t_, real_t_](
        const char *fname,
        vector[int_t_] &indptr,
        vector[int_t_] &indices,
        vector[real_t_] &values,
        vector[int_t_] &indptr_lab,
        vector[int_t_] &indices_lab,
        vector[int_t_] &qid,
        size_large &nrows,
        size_large &ncols,
        size_large &nclasses,
        const size_t limit_nrows,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t assume_no_qid,
        const bool_t assume_trailing_ws
    ) except + nogil

    bool_t read_single_label_filestream[int_t_, real_t_, label_t_](
        const char *fname,
        vector[int_t_] &indptr,
        vector[int_t_] &indices,
        vector[real_t_] &values,
        vector[label_t_] &labels,
        vector[int_t_] &qid,
        size_large &nrows,
        size_large &ncols,
        size_large &nclasses,
        const size_t limit_nrows,
        const bool_t ignore_zero_valued,
        const bool_t sort_indices,
        const bool_t text_is_base1,
        const bool_t assume_no_qid,
        const bool_t assume_trailing_ws
    ) except + nogil

cdef extern from "utils.hpp":
    void sort_sparse_indices_known_ncol[int_t_, real_t_](
        int_t_ *indptr,
        int_t_ *indices,
        real_t_ *values,
        size_t nrows, size_t ncols
    )

cdef void* get_ptr_int(np.ndarray[int_t, ndim=1] a):
    if a.shape[0]:
        return &a[0]
    else:
        return NULL

cdef void* get_ptr_num(np.ndarray[real_t, ndim=1] a):
    if a.shape[0]:
        return &a[0]
    else:
        return NULL

cdef void* get_ptr_lab(np.ndarray[label_t, ndim=1] a):
    if a.shape[0]:
        return &a[0]
    else:
        return NULL

def sort_matrix_indices(
        np.ndarray[int_t, ndim=1] indptr,
        np.ndarray[int_t, ndim=1] indices,
        np.ndarray[real_t, ndim=1] values,
        size_t nrows, size_t ncols
    ):
    cdef int_t *ptr_indptr = NULL
    cdef int_t *ptr_indices = NULL
    cdef real_t *ptr_values = NULL

    if int_t is int:
        ptr_indptr = <int_t*>get_ptr_int[int](indptr)
        ptr_indices = <int_t*>get_ptr_int[int](indices)
    elif int_t is int64_t:
        ptr_indptr = <int_t*>get_ptr_int[int64_t](indptr)
        ptr_indices = <int_t*>get_ptr_int[int64_t](indices)
    else:
        ptr_indptr = <int_t*>get_ptr_int[size_t](indptr)
        ptr_indices = <int_t*>get_ptr_int[size_t](indices)

    if real_t is float:
        ptr_values = <real_t*>get_ptr_num[float](values)
    else:
        ptr_values = <real_t*>get_ptr_num[double](values)
    
    sort_sparse_indices_known_ncol(
        ptr_indptr,
        ptr_indices,
        ptr_values,
        nrows, ncols
    )

cdef extern from *:
    """
    template <class T>
    void delete_cpp_vector(void *vector_pointer)
    {
        std::vector<T> *ptr_typed = (std::vector<T>*)vector_pointer;
        delete ptr_typed;
    }
    
    template <class T>
    void delete_vector_capsule(PyObject *obj)
    {
        void *ptr = (void*)PyCapsule_GetPointer(obj, NULL);
        delete_cpp_vector<T>(ptr);
    }

    template <class T>
    NPY_TYPES get_numpy_type()
    {
        if (std::is_same<T, double>::value) return NPY_DOUBLE;

        if (std::is_same<T, float>::value) return NPY_FLOAT;

        if (std::is_same<T, int>::value) return NPY_INT;

        if (std::is_same<T, int64_t>::value) return NPY_INT64;

        if (std::is_same<T, size_t>::value)
        {
            if (sizeof(size_t) == 8) return NPY_UINT64;

            if (sizeof(size_t) == 4) return NPY_UINT32;

            if (sizeof(size_t) == 2) return NPY_UINT16;

            throw std::runtime_error("Platform has unsupported variant for 'size_t' indices.");
        }

        throw std::runtime_error("Internal error. Please open a bug report.");
    }
    
    template <class T>
    PyObject* make_numpy_from_cpp_vec(void *vector_pointer, size_t dim)
    {
        std::vector<T> *typed_pointer = (std::vector<T>*)vector_pointer;
        std::vector<T> *temp_vector = new std::vector<T>();
        std::swap(*typed_pointer, *temp_vector);
        PyObject *wrapped_vec_obj_base = PyCapsule_New((void*)temp_vector, NULL, delete_vector_capsule<T>);
        if (!wrapped_vec_obj_base) {
            delete temp_vector;
            throw std::bad_alloc();
        }
        npy_intp dims[] = {(npy_intp)dim};
        PyObject *out = PyArray_SimpleNewFromData(1, dims, get_numpy_type<T>(), (void*)temp_vector->data());
        if (!out) {
            throw std::bad_alloc();
        }
        PyArray_SetBaseObject((PyArrayObject*)out, wrapped_vec_obj_base);
        return out;
    }

    PyObject* make_numpy_from_cpp_vec_double(void *vector_pointer, size_t dim) { return make_numpy_from_cpp_vec<double>(vector_pointer, dim); }
    PyObject* make_numpy_from_cpp_vec_float(void *vector_pointer, size_t dim) { return make_numpy_from_cpp_vec<float>(vector_pointer, dim); }
    PyObject* make_numpy_from_cpp_vec_int(void *vector_pointer, size_t dim) { return make_numpy_from_cpp_vec<int>(vector_pointer, dim); }
    PyObject* make_numpy_from_cpp_vec_int64(void *vector_pointer, size_t dim) { return make_numpy_from_cpp_vec<int64_t>(vector_pointer, dim); }
    PyObject* make_numpy_from_cpp_vec_size_t(void *vector_pointer, size_t dim) { return make_numpy_from_cpp_vec<size_t>(vector_pointer, dim); }
    """
    object make_numpy_from_cpp_vec_double(void *vector_pointer, size_t dim) except +
    object make_numpy_from_cpp_vec_float(void *vector_pointer, size_t dim) except +
    object make_numpy_from_cpp_vec_int(void *vector_pointer, size_t dim) except +
    object make_numpy_from_cpp_vec_int64(void *vector_pointer, size_t dim) except +
    object make_numpy_from_cpp_vec_size_t(void *vector_pointer, size_t dim) except +


### https://github.com/cython/cython/issues/3968
cdef np.ndarray[label_t, ndim=1] cast_vec(label_t *v0, vector[label_t] &inp) with gil:
    if label_t is int:
        return make_numpy_from_cpp_vec_int(&inp, inp.size())
    elif label_t is int64_t:
        return make_numpy_from_cpp_vec_int64(&inp, inp.size())
    elif label_t is size_t:
        return make_numpy_from_cpp_vec_size_t(&inp, inp.size())
    elif label_t is float:
        return make_numpy_from_cpp_vec_float(&inp, inp.size())
    elif label_t is double:
        return make_numpy_from_cpp_vec_double(&inp, inp.size())
    else:
        raise ValueError("Internal error. Please open a bug report.")

cdef dict convert_cpp_vectors_to_numpy(
        int_t *v0, real_t *v1, label_t *v2,
        vector[int_t] &indptr,
        vector[int_t] &indices,
        vector[real_t] &values,
        vector[label_t] &labels,
        vector[int_t] &indptr_lab,
        vector[int_t] &indices_lab,
        vector[int_t] &qid
    ):
    cdef np.ndarray[int_t, ndim=1] indptr_py = cast_vec(indptr.data(), indptr)
    cdef np.ndarray[int_t, ndim=1] indices_py = cast_vec(indices.data(), indices)
    cdef np.ndarray[int_t, ndim=1] qid_py = cast_vec(qid.data(), qid)
    cdef np.ndarray[real_t, ndim=1] values_py = cast_vec(values.data(), values)
    cdef np.ndarray[label_t, ndim=1] labels_py = cast_vec(labels.data(), labels)
    cdef np.ndarray[int_t, ndim=1] indptr_lab_py = cast_vec(indptr_lab.data(), indptr_lab)
    cdef np.ndarray[int_t, ndim=1] indices_lab_py = cast_vec(indices_lab.data(), indices_lab)

    cdef dict out = dict()
    out["indptr"] = indptr_py
    out["indices"] = indices_py
    out["values"] = values_py
    out["labels"] = labels_py
    out["indptr_lab"] = indptr_lab_py
    out["indices_lab"] = indices_lab_py
    out["qid"] = qid_py
    return out


def read_single_label_py(
        str fname,
        bool_t ignore_zero_valued = True,
        bool_t sort_indices = True,
        bool_t text_is_base1 = True,
        bool_t assume_no_qid = True,
        bool_t use_int64 = True,
        bool_t use_double = True,
        size_t limit_nrows = 0
    ):
    cdef vector[int64_t] indptr, indices
    cdef vector[double] values, labels
    cdef vector[int64_t] qid
    cdef size_large nrows = 0
    cdef size_large ncols = 0
    cdef size_large nclasses = 0

    cdef vector[int] indptr_int, indices_int
    cdef vector[int] qid_int
    cdef vector[float] values_float, labels_float

    cdef bytes fname_py = fname.encode()
    cdef char* fname_c = fname_py
    cdef bool_t succeded
    with nogil, boundscheck(False), nonecheck(False), wraparound(False):
        if use_int64:
            if use_double:
                succeded = read_single_label_filestream[int64_t, double, double](
                    fname_c,
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
                    True
                )
            else:
                succeded = read_single_label_filestream[int64_t, float, float](
                    fname_c,
                    indptr,
                    indices,
                    values_float,
                    labels_float,
                    qid,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )
        else:
            if use_double:
                succeded = read_single_label_filestream[int, double, double](
                    fname_c,
                    indptr_int,
                    indices_int,
                    values,
                    labels,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )
            else:
                succeded = read_single_label_filestream[int, float, float](
                    fname_c,
                    indptr_int,
                    indices_int,
                    values_float,
                    labels_float,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )

    cdef vector[int64_t] indptr_lab, indices_lab
    cdef vector[int] indptr_lab_int, indices_lab_int
    cdef dict out
    if not succeded:
        return dict()
    else:
        if use_int64:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int64_t, double, double](
                    indptr.data(), values.data(), labels.data(),
                    indptr,
                    indices,
                    values,
                    labels,
                    indptr_lab,
                    indices_lab,
                    qid
                )
            else:
                out = convert_cpp_vectors_to_numpy[int64_t, float, float](
                    indptr.data(), values_float.data(), labels_float.data(),
                    indptr,
                    indices,
                    values_float,
                    labels_float,
                    indptr_lab,
                    indices_lab,
                    qid
                )
        else:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int, double, double](
                    indptr_int.data(), values.data(), labels.data(),
                    indptr_int,
                    indices_int,
                    values,
                    labels,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
            else:
                out = convert_cpp_vectors_to_numpy[int, float, float](
                    indptr_int.data(), values_float.data(), labels_float.data(),
                    indptr_int,
                    indices_int,
                    values_float,
                    labels_float,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
        out["nrows"] = nrows
        out["ncols"] = ncols
        out["nclasses"] = nclasses
        return out


def read_multi_label_py(
        str fname,
        bool_t ignore_zero_valued = True,
        bool_t sort_indices = True,
        bool_t text_is_base1 = True,
        bool_t assume_no_qid = True,
        bool_t use_int64 = True,
        bool_t use_double = True,
        size_t limit_nrows = 0
    ):
    cdef vector[int64_t] indptr, indices
    cdef vector[int64_t] indptr_lab, indices_lab
    cdef vector[double] values
    cdef vector[int64_t] qid
    cdef size_large nrows = 0
    cdef size_large ncols = 0
    cdef size_large nclasses = 0

    cdef vector[int] indptr_int, indices_int
    cdef vector[int] indptr_lab_int, indices_lab_int
    cdef vector[int] qid_int
    cdef vector[float] values_float

    cdef bytes fname_py = fname.encode()
    cdef char* fname_c = fname_py
    cdef bool_t succeded
    with nogil, boundscheck(False), nonecheck(False), wraparound(False):
        if use_int64:
            if use_double:
                succeded = read_multi_label_filestream[int64_t, double](
                    fname_c,
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
                    True
                )
            else:
                succeded = read_multi_label_filestream[int64_t, float](
                    fname_c,
                    indptr,
                    indices,
                    values_float,
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
                    True
                )
        else:
            if use_double:
                succeded = read_multi_label_filestream[int, double](
                    fname_c,
                    indptr_int,
                    indices_int,
                    values,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )
            else:
                succeded = read_multi_label_filestream[int, float](
                    fname_c,
                    indptr_int,
                    indices_int,
                    values_float,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )

    cdef vector[double] labels
    cdef vector[float] labels_float
    cdef dict out
    if not succeded:
        return dict()
    else:
        if use_int64:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int64_t, double, double](
                    indptr.data(), values.data(), labels.data(),
                    indptr,
                    indices,
                    values,
                    labels,
                    indptr_lab,
                    indices_lab,
                    qid
                )
            else:
                out = convert_cpp_vectors_to_numpy[int64_t, float, float](
                    indptr.data(), values_float.data(), labels_float.data(),
                    indptr,
                    indices,
                    values_float,
                    labels_float,
                    indptr_lab,
                    indices_lab,
                    qid
                )
        else:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int, double, double](
                    indptr_int.data(), values.data(), labels.data(),
                    indptr_int,
                    indices_int,
                    values,
                    labels,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
            else:
                out = convert_cpp_vectors_to_numpy[int, float, float](
                    indptr_int.data(), values_float.data(), labels_float.data(),
                    indptr_int,
                    indices_int,
                    values_float,
                    labels_float,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
        out["nrows"] = nrows
        out["ncols"] = ncols
        out["nclasses"] = nclasses
        return out

def read_single_label_from_str_py(
        str data_str,
        const bool_t ignore_zero_valued = True,
        const bool_t sort_indices = True,
        const bool_t text_is_base1 = True,
        const bool_t assume_no_qid = True,
        bool_t use_int64 = True,
        bool_t use_double = True,
        size_t limit_nrows = 0
    ):
    cdef bytes data_bytes = data_str.encode()
    cdef string cpp_str = data_bytes

    cdef vector[int64_t] indptr, indices
    cdef vector[double] values, labels
    cdef vector[int64_t] qid
    cdef size_large nrows = 0
    cdef size_large ncols = 0
    cdef size_large nclasses = 0

    cdef vector[int] indptr_int, indices_int
    cdef vector[int] qid_int
    cdef vector[float] values_float, labels_float

    cdef bool_t succeded
    with nogil, boundscheck(False), nonecheck(False), wraparound(False):
        if use_int64:
            if use_double:
                succeded = read_single_label_str[int64_t, double, double](
                    cpp_str,
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
                    True
                )
            else:
                succeded = read_single_label_str[int64_t, float, float](
                    cpp_str,
                    indptr,
                    indices,
                    values_float,
                    labels_float,
                    qid,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )
        else:
            if use_double:
                succeded = read_single_label_str[int, double, double](
                    cpp_str,
                    indptr_int,
                    indices_int,
                    values,
                    labels,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )
            else:
                succeded = read_single_label_str[int, float, float](
                    cpp_str,
                    indptr_int,
                    indices_int,
                    values_float,
                    labels_float,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )

    cdef vector[int64_t] indptr_lab, indices_lab
    cdef vector[int] indptr_lab_int, indices_lab_int
    cdef dict out
    if not succeded:
        return dict()
    else:
        if use_int64:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int64_t, double, double](
                    indptr.data(), values.data(), labels.data(),
                    indptr,
                    indices,
                    values,
                    labels,
                    indptr_lab,
                    indices_lab,
                    qid
                )
            else:
                out = convert_cpp_vectors_to_numpy[int64_t, float, float](
                    indptr.data(), values_float.data(), labels_float.data(),
                    indptr,
                    indices,
                    values_float,
                    labels_float,
                    indptr_lab,
                    indices_lab,
                    qid
                )
        else:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int, double, double](
                    indptr_int.data(), values.data(), labels.data(),
                    indptr_int,
                    indices_int,
                    values,
                    labels,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
            else:
                out = convert_cpp_vectors_to_numpy[int, float, float](
                    indptr_int.data(), values_float.data(), labels_float.data(),
                    indptr_int,
                    indices_int,
                    values_float,
                    labels_float,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
        out["nrows"] = nrows
        out["ncols"] = ncols
        out["nclasses"] = nclasses
        return out


def read_multi_label_from_str_py(
        str data_str,
        const bool_t ignore_zero_valued = True,
        const bool_t sort_indices = True,
        const bool_t text_is_base1 = True,
        const bool_t assume_no_qid = True,
        bool_t use_int64 = True,
        bool_t use_double = True,
        size_t limit_nrows = 0
    ):
    cdef bytes data_bytes = data_str.encode()
    cdef string cpp_str = data_bytes
    cdef vector[int64_t] indptr, indices
    cdef vector[int64_t] indptr_lab, indices_lab
    cdef vector[double] values
    cdef vector[int64_t] qid
    cdef size_large nrows = 0
    cdef size_large ncols = 0
    cdef size_large nclasses = 0

    cdef vector[int] indptr_int, indices_int
    cdef vector[int] indptr_lab_int, indices_lab_int
    cdef vector[int] qid_int
    cdef vector[float] values_float

    cdef bool_t succeded
    with nogil, boundscheck(False), nonecheck(False), wraparound(False):
        if use_int64:
            if use_double:
                succeded = read_multi_label_str[int64_t, double](
                    cpp_str,
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
                    True
                )
            else:
                succeded = read_multi_label_str[int64_t, float](
                    cpp_str,
                    indptr,
                    indices,
                    values_float,
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
                    True
                )
        else:
            if use_double:
                succeded = read_multi_label_str[int, double](
                    cpp_str,
                    indptr_int,
                    indices_int,
                    values,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )
            else:
                succeded = read_multi_label_str[int, float](
                    cpp_str,
                    indptr_int,
                    indices_int,
                    values_float,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int,
                    nrows,
                    ncols,
                    nclasses,
                    limit_nrows,
                    ignore_zero_valued,
                    sort_indices,
                    text_is_base1,
                    assume_no_qid,
                    True
                )

    cdef vector[double] labels
    cdef vector[float] labels_float
    cdef dict out
    if not succeded:
        return dict()
    else:
        if use_int64:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int64_t, double, double](
                    indptr.data(), values.data(), labels.data(),
                    indptr,
                    indices,
                    values,
                    labels,
                    indptr_lab,
                    indices_lab,
                    qid
                )
            else:
                out = convert_cpp_vectors_to_numpy[int64_t, float, float](
                    indptr.data(), values_float.data(), labels_float.data(),
                    indptr,
                    indices,
                    values_float,
                    labels_float,
                    indptr_lab,
                    indices_lab,
                    qid
                )
        else:
            if use_double:
                out = convert_cpp_vectors_to_numpy[int, double, double](
                    indptr_int.data(), values.data(), labels.data(),
                    indptr_int,
                    indices_int,
                    values,
                    labels,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
            else:
                out = convert_cpp_vectors_to_numpy[int, float, float](
                    indptr_int.data(), values_float.data(), labels_float.data(),
                    indptr_int,
                    indices_int,
                    values_float,
                    labels_float,
                    indptr_lab_int,
                    indices_lab_int,
                    qid_int
                )
        out["nrows"] = nrows
        out["ncols"] = ncols
        out["nclasses"] = nclasses
        return out

def write_single_label_py(
        str fname,
        np.ndarray[int_t, ndim=1] indptr,
        np.ndarray[int_t, ndim=1] indices,
        np.ndarray[real_t, ndim=1] values,
        np.ndarray[label_t, ndim=1] labels,
        np.ndarray[int_t, ndim=1] qid,
        const size_large ncols,
        const size_large nclasses,
        const bool_t ignore_zero_valued = True,
        const bool_t sort_indices = True,
        const bool_t text_is_base1 = True,
        const bool_t add_header = False,
        const int decimal_places = 8,
        bool_t append = False
    ):
    cdef bool_t success = False
    cdef int_t missing_qid = SIZE_MAX if (int_t is size_t) else -INT_MAX
    cdef label_t missing_label
    if (label_t is float) or (label_t is double):
        missing_label = np.nan
    elif (label_t is size_t):
        missing_label = SIZE_MAX
    else:
        missing_label = -INT_MAX

    cdef bool_t has_qid = qid.shape[0] > 0
    cdef size_t nrows = indptr.shape[0]-1

    cdef int_t *ptr_indptr = <int_t*>get_ptr_int(indptr)
    cdef int_t *ptr_indices = <int_t*>get_ptr_int(indices)
    cdef real_t *ptr_values = <real_t*>get_ptr_num(values)
    cdef int_t *ptr_qid = <int_t*>get_ptr_int(qid)
    cdef label_t *ptr_labels = <label_t*>get_ptr_lab(labels)

    cdef FILE *output_file = cy_fopen(fname, False, append)
    if output_file == NULL:
        py_throw_err()

    try:
        with nogil, boundscheck(False), nonecheck(False), wraparound(False):
            if int_t is int:
                if real_t is float:
                    if label_t is int:
                        success =  write_single_label_template[int, float, int](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is int64_t:
                        success =  write_single_label_template[int, float, int64_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is size_t:
                        success =  write_single_label_template[int, float, size_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is float:
                        success =  write_single_label_template[int, float, float](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    else:
                        success =  write_single_label_template[int, float, double](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                else:
                    if label_t is int:
                        success =  write_single_label_template[int, double, int](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is int64_t:
                        success =  write_single_label_template[int, double, int64_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is size_t:
                        success =  write_single_label_template[int, double, size_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is float:
                        success =  write_single_label_template[int, double, float](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    else:
                        success =  write_single_label_template[int, double, double](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
            elif int_t is int64_t:
                if real_t is float:
                    if label_t is int:
                        success =  write_single_label_template[int64_t, float, int](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is int64_t:
                        success =  write_single_label_template[int64_t, float, int64_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is size_t:
                        success =  write_single_label_template[int64_t, float, size_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is float:
                        success =  write_single_label_template[int64_t, float, float](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    else:
                        success =  write_single_label_template[int64_t, float, double](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                else:
                    if label_t is int:
                        success =  write_single_label_template[int64_t, double, int](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is int64_t:
                        success =  write_single_label_template[int64_t, double, int64_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is size_t:
                        success =  write_single_label_template[int64_t, double, size_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is float:
                        success =  write_single_label_template[int64_t, double, float](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    else:
                        success =  write_single_label_template[int64_t, double, double](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
            else:
                if real_t is float:
                    if label_t is int:
                        success =  write_single_label_template[size_t, float, int](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is int64_t:
                        success =  write_single_label_template[size_t, float, int64_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is size_t:
                        success =  write_single_label_template[size_t, float, size_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is float:
                        success =  write_single_label_template[size_t, float, float](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    else:
                        success =  write_single_label_template[size_t, float, double](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                else:
                    if label_t is int:
                        success =  write_single_label_template[size_t, double, int](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is int64_t:
                        success =  write_single_label_template[size_t, double, int64_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is size_t:
                        success =  write_single_label_template[size_t, double, size_t](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    elif label_t is float:
                        success =  write_single_label_template[size_t, double, float](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
                    else:
                        success =  write_single_label_template[size_t, double, double](
                            output_file, ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                            missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                            ignore_zero_valued, sort_indices, text_is_base1,
                            add_header, decimal_places
                        )
    finally:
        if (output_file != NULL):
            fclose(output_file)
            output_file = NULL
    
    if not success:
        raise ValueError("Error: could not write file.")
    return success

def write_multi_label_py(
        str fname,
        np.ndarray[int_t, ndim=1] indptr,
        np.ndarray[int_t, ndim=1] indices,
        np.ndarray[real_t, ndim=1] values,
        np.ndarray[int_t, ndim=1] indptr_lab,
        np.ndarray[int_t, ndim=1] indices_lab,
        np.ndarray[int_t, ndim=1] qid,
        size_large ncols,
        size_large nclasses,
        bool_t ignore_zero_valued = True,
        bool_t sort_indices = True,
        bool_t text_is_base1 = True,
        bool_t add_header = False,
        int decimal_places = 8,
        bool_t append = False
    ):
    cdef int_t missing_qid = SIZE_MAX if int_t is size_t else -INT_MAX
    cdef bool_t succeded = False

    cdef int_t * ptr_indptr = <int_t*>get_ptr_int(indptr)
    cdef int_t * ptr_indices = <int_t*>get_ptr_int(indices)
    cdef real_t * ptr_values = <real_t*>get_ptr_num(values)
    cdef int_t * ptr_indptr_lab = <int_t*>get_ptr_int(indptr_lab)
    cdef int_t * ptr_indices_lab = <int_t*>get_ptr_int(indices_lab)
    cdef int_t * ptr_qid = <int_t*>get_ptr_int(qid)

    cdef bool_t has_qid = qid.shape[0] > 0
    cdef size_t nrows = indptr.shape[0] - 1

    cdef FILE *output_file = cy_fopen(fname, False, append)
    if output_file == NULL:
        py_throw_err()

    try:
        with nogil, boundscheck(False), nonecheck(False), wraparound(False):
            if int_t is int:
                if real_t is float:
                    succeded = write_multi_label_template[int, float](
                        output_file, ptr_indptr, ptr_indices,
                        ptr_values, ptr_indptr_lab, ptr_indices_lab,
                        ptr_qid, missing_qid,
                        has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                        sort_indices, text_is_base1, add_header, decimal_places
                    )
                else:
                    succeded = write_multi_label_template[int, double](
                        output_file, ptr_indptr, ptr_indices,
                        ptr_values, ptr_indptr_lab, ptr_indices_lab,
                        ptr_qid, missing_qid,
                        has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                        sort_indices, text_is_base1, add_header, decimal_places
                    )
            elif int_t is int64_t:
                if real_t is float:
                    succeded = write_multi_label_template[int64_t, float](
                        output_file, ptr_indptr, ptr_indices,
                        ptr_values, ptr_indptr_lab, ptr_indices_lab,
                        ptr_qid, missing_qid,
                        has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                        sort_indices, text_is_base1, add_header, decimal_places
                    )
                else:
                    succeded = write_multi_label_template[int64_t, double](
                        output_file, ptr_indptr, ptr_indices,
                        ptr_values, ptr_indptr_lab, ptr_indices_lab,
                        ptr_qid, missing_qid,
                        has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                        sort_indices, text_is_base1, add_header, decimal_places
                    )
            else:
                if real_t is float:
                    succeded = write_multi_label_template[size_t, float](
                        output_file, ptr_indptr, ptr_indices,
                        ptr_values, ptr_indptr_lab, ptr_indices_lab,
                        ptr_qid, missing_qid,
                        has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                        sort_indices, text_is_base1, add_header, decimal_places
                    )
                else:
                    succeded = write_multi_label_template[size_t, double](
                        output_file, ptr_indptr, ptr_indices,
                        ptr_values, ptr_indptr_lab, ptr_indices_lab,
                        ptr_qid, missing_qid,
                        has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                        sort_indices, text_is_base1, add_header, decimal_places
                    )
    finally:
        if (output_file != NULL):
            fclose(output_file)
            output_file = NULL
    
    if not succeded:
        raise ValueError("Error: could not write file.")
    return succeded

def write_single_label_to_str_py(
        np.ndarray[int_t, ndim=1] indptr,
        np.ndarray[int_t, ndim=1] indices,
        np.ndarray[real_t, ndim=1] values,
        np.ndarray[label_t, ndim=1] labels,
        np.ndarray[int_t, ndim=1] qid,
        size_large ncols,
        size_large nclasses,
        bool_t ignore_zero_valued = True,
        bool_t sort_indices = True,
        bool_t text_is_base1 = True,
        bool_t add_header = False,
        int decimal_places = 8,
        bool_t append = False
    ):
    cdef string succeeded
    cdef int_t missing_qid = SIZE_MAX if (int_t is size_t) else -INT_MAX
    cdef label_t missing_label
    if (label_t is float) or (label_t is double):
        missing_label = np.nan
    elif (label_t is size_t):
        missing_label = SIZE_MAX
    else:
        missing_label = -INT_MAX

    cdef int_t *ptr_indptr = <int_t*>get_ptr_int(indptr)
    cdef int_t *ptr_indices = <int_t*>get_ptr_int(indices)
    cdef real_t *ptr_values = <real_t*>get_ptr_num(values)
    cdef int_t *ptr_qid = <int_t*>get_ptr_int(qid)
    cdef label_t *ptr_labels = <label_t*>get_ptr_lab(labels)

    cdef bool_t has_qid = qid.shape[0] > 0
    cdef size_t nrows = indptr.shape[0] - 1

    with nogil, boundscheck(False), nonecheck(False), wraparound(False):
        if int_t is int:
            if real_t is float:
                if label_t is int:
                    succeeded = write_single_label_str[int, float, int](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is int64_t:
                    succeeded = write_single_label_str[int, float, int64_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is size_t:
                    succeeded = write_single_label_str[int, float, size_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is float:
                    succeeded = write_single_label_str[int, float, float](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                else:
                    succeeded = write_single_label_str[int, float, double](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
            else:
                if label_t is int:
                    succeeded = write_single_label_str[int, double, int](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is int64_t:
                    succeeded = write_single_label_str[int, double, int64_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is size_t:
                    succeeded = write_single_label_str[int, double, size_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is float:
                    succeeded = write_single_label_str[int, double, float](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                else:
                    succeeded = write_single_label_str[int, double, double](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
        elif int_t is int64_t:
            if real_t is float:
                if label_t is int:
                    succeeded = write_single_label_str[int64_t, float, int](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is int64_t:
                    succeeded = write_single_label_str[int64_t, float, int64_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is size_t:
                    succeeded = write_single_label_str[int64_t, float, size_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is float:
                    succeeded = write_single_label_str[int64_t, float, float](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                else:
                    succeeded = write_single_label_str[int64_t, float, double](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
            else:
                if label_t is int:
                    succeeded = write_single_label_str[int64_t, double, int](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is int64_t:
                    succeeded = write_single_label_str[int64_t, double, int64_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is size_t:
                    succeeded = write_single_label_str[int64_t, double, size_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is float:
                    succeeded = write_single_label_str[int64_t, double, float](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                else:
                    succeeded = write_single_label_str[int64_t, double, double](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
        else:
            if real_t is float:
                if label_t is int:
                    succeeded = write_single_label_str[size_t, float, int](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is int64_t:
                    succeeded = write_single_label_str[size_t, float, int64_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is size_t:
                    succeeded = write_single_label_str[size_t, float, size_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is float:
                    succeeded = write_single_label_str[size_t, float, float](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                else:
                    succeeded = write_single_label_str[size_t, float, double](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
            else:
                if label_t is int:
                    succeeded = write_single_label_str[size_t, double, int](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is int64_t:
                    succeeded = write_single_label_str[size_t, double, int64_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is size_t:
                    succeeded = write_single_label_str[size_t, double, size_t](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                elif label_t is float:
                    succeeded = write_single_label_str[size_t, double, float](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )
                else:
                    succeeded = write_single_label_str[size_t, double, double](
                        ptr_indptr, ptr_indices, ptr_values, ptr_labels, ptr_qid,
                        missing_qid, missing_label, has_qid, nrows, ncols, nclasses,
                        ignore_zero_valued, sort_indices, text_is_base1,
                        add_header, decimal_places
                    )

    if (succeeded.size() == 1) and (succeeded.c_str()[0] == <char>101): #101:e
        raise ValueError("Error: could not write matrix to string.")
    return succeeded

def write_multi_label_to_str_py(
        np.ndarray[int_t, ndim=1] indptr,
        np.ndarray[int_t, ndim=1] indices,
        np.ndarray[real_t, ndim=1] values,
        np.ndarray[int_t, ndim=1] indptr_lab,
        np.ndarray[int_t, ndim=1] indices_lab,
        np.ndarray[int_t, ndim=1] qid,
        size_large ncols,
        size_large nclasses,
        bool_t ignore_zero_valued = True,
        bool_t sort_indices = True,
        bool_t text_is_base1 = True,
        bool_t add_header = False,
        int decimal_places = 8,
        bool_t append = False
    ):
    cdef int_t missing_qid = SIZE_MAX if int_t is size_t else -INT_MAX
    cdef string succeded

    cdef int_t * ptr_indptr = <int_t*>get_ptr_int(indptr)
    cdef int_t * ptr_indices = <int_t*>get_ptr_int(indices)
    cdef real_t * ptr_values = <real_t*>get_ptr_num(values)
    cdef int_t * ptr_indptr_lab = <int_t*>get_ptr_int(indptr_lab)
    cdef int_t * ptr_indices_lab = <int_t*>get_ptr_int(indices_lab)
    cdef int_t * ptr_qid = <int_t*>get_ptr_int(qid)

    cdef bool_t has_qid = qid.shape[0] > 0
    cdef size_t nrows = indptr.shape[0] - 1

    with nogil, boundscheck(False), nonecheck(False), wraparound(False):
        if int_t is int:
            if real_t is float:
                succeded = write_multi_label_str[int, float](
                    ptr_indptr, ptr_indices,
                    ptr_values, ptr_indptr_lab, ptr_indices_lab,
                    ptr_qid, missing_qid,
                    has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                    sort_indices, text_is_base1, add_header, decimal_places
                )
            else:
                succeded = write_multi_label_str[int, double](
                    ptr_indptr, ptr_indices,
                    ptr_values, ptr_indptr_lab, ptr_indices_lab,
                    ptr_qid, missing_qid,
                    has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                    sort_indices, text_is_base1, add_header, decimal_places
                )
        elif int_t is int64_t:
            if real_t is float:
                succeded = write_multi_label_str[int64_t, float](
                    ptr_indptr, ptr_indices,
                    ptr_values, ptr_indptr_lab, ptr_indices_lab,
                    ptr_qid, missing_qid,
                    has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                    sort_indices, text_is_base1, add_header, decimal_places
                )
            else:
                succeded = write_multi_label_str[int64_t, double](
                    ptr_indptr, ptr_indices,
                    ptr_values, ptr_indptr_lab, ptr_indices_lab,
                    ptr_qid, missing_qid,
                    has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                    sort_indices, text_is_base1, add_header, decimal_places
                )
        else:
            if real_t is float:
                succeded = write_multi_label_str[size_t, float](
                    ptr_indptr, ptr_indices,
                    ptr_values, ptr_indptr_lab, ptr_indices_lab,
                    ptr_qid, missing_qid,
                    has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                    sort_indices, text_is_base1, add_header, decimal_places
                )
            else:
                succeded = write_multi_label_str[size_t, double](
                    ptr_indptr, ptr_indices,
                    ptr_values, ptr_indptr_lab, ptr_indices_lab,
                    ptr_qid, missing_qid,
                    has_qid, nrows, ncols, nclasses, ignore_zero_valued,
                    sort_indices, text_is_base1, add_header, decimal_places
                )
    
    if (succeded.size() == 1) and (succeded.c_str()[0] == <char>101): #101:e
        raise ValueError("Error: could not write matrix to string.")
    return succeded
