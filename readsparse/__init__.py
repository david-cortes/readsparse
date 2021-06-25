import numpy as np
import os, warnings
import ctypes
from scipy.sparse import csr_matrix, isspmatrix_csr
from . import _cpp_interface 

__all__ = ["read_sparse", "write_sparse"]


### Main functions
def read_sparse(
        file, multilabel=False, has_qid=False, integer_labels=False,
        index1=True, sort_indices=True, ignore_zeros=True,
        min_cols=0, min_classes=0, limit_nrows=0, no_trailing_ws = False,
        use_int64=False, use_double=True, use_cpp=True, from_string=False
    ):
    """
    Read Sparse Matrix from Text File

    Read a labelled sparse CSR matrix in text format as used by libraries
    such as SVMLight, LibSVM, ThunderSVM, LibFM, xLearn, XGBoost, LightGBM, and more.


    The format is as follows:

    <label(s)> <column>:<value> <column>:<value> ...


    Example line (row):

    1 1:1.234 3:20


    This line denotes a row with label (target variable) equal to 1, a value
    for the first column of 1.234, a value of zero for the second column (which is
    missing), and a value of 20 for the third column.

    The labels might be decimal (for regression), and each row might contain more
    than one label (must be integers in this case), separated by commas *without*
    spaces inbetween - e.g.:

    1,5,10 1:1.234 3:20

    This line indicates a row with labels 1, 5, and 10 (for multi-class classification).
    If the line has no labels, it should still include a space before the features.


    The rows might additionally contain a 'qid' parameter as used in ranking algorithms,
    which should always lay inbetween the labels and the features and must be an integer - e.g.:

    1 qid:2 1:1.234 3:20


    The file might optionally contain a header as the first line with metadata
    (number of rows, number of columns, number of classes). Presence of a header will be
    automatically detected, and is recommended to include it for speed purposes. Datasets
    from the extreme classification repository (see references) usually include such a header.

    Lines might include comments, which start after a '#' character. Lines consisting
    of only a '#' will be ignored. When reading from a file, such file might have a
    BOM (information about encoding uses in Windows sytems), which will be automatically
    skipped.


    Some extra notes - this function:
        - Will not make any checks for negative column indices.
        - Will be able to read numeric values in scientific notation only if the E is capitalized.
        - Will fill missing labels with NAs when passing ``multilabel=False``.
        - Will fill with zeros (empty values) the lines that are empty (that is,
          they generate a row in the data), but will ignore (that is, will not generate
          a row in the data) the lines that start with '#'.
    
    On 64-bit Windows systems, if compiling the library with a compiler other than MinGW
    or MSVC, it will not be able to read files larger than 2GB and might crash the system
    if the file is larger.

    If the file contains a header, and this header denotes a larger number of columns
    or of labels than the largest index in the data, the resulting object will have
    this dimension set according to the header. The third entry in the header (number
    of classes/labels) will be ignored when passing ``multilabel=False``.

    The function uses different code paths when reading from a file or from a string,
    and there might be slight differences between the obtained results from them.
    For example, reading from a file might produce the desired output if the file
    uses tabs as separators instead of spaces (not supported by most other software
    and not standard), whereas reading from a string will not.
    If any such difference is encountered, please submit a bug report in the
    package's GitHub page.

    Parameters
    ----------
    file : str, None, or file connection
        Either a file path from which the data will be read, or a string
        containing the text from which the data will be read.
        In the latter case, must pass ``from_string=True``.
    multilabel : bool
        Whether the input file can have multiple labels per observation.
        If passing ``multilabel=False`` and it turns out to have multiple labels, will only
        take the first one for each row. If the labels are non-integers or have decimal point,
        the results will be invalid.
    has_qid : bool
        Whether the input file has 'qid' field (used for ranking). If passing
        ``False`` and the file does turns out to have 'qid', the features will not be read for any
        observations.
    integer_labels : bool
        Whether to output the observation labels as integers.
    index1 : bool
        Whether the input file uses numeration starting at 1 for the column
        numbers (and for the label numbers when passing ``multilabel=True``). This is usually
        the case for files downloaded from the repositories in the references. The function
        will check for whether any of the column indices is zero, and will ignore this
        option if so (i.e. will assume it is ``False``).
    sort_indices : bool
        Whether to sort the indices of the columns after reading the data.
        These should already be sorted in the files from the repositories in the references.
    ignore_zeros : bool
        Whether to avoid adding features which have a value of zero.
        If the zeros are caused due to numerical rounding in the software that wrote the
        input file, they can be post-processed by passing ``ignore_zeros=False`` and then
        something like 'X.data[X.data == 0] = 1e-8'.
    min_cols : int
        Minimum number of columns that the output ``X`` object should have,
        in case some columns are all missing in the input data.
    min_classes : int
        Minimum number of columns that the output ``y`` object should have,
        in case some columns are all missing in the input data. Only used when passing
        ``multilabel=True``.
    limit_nrows : int
        Maximum number of rows to read from the data. If there are more than this
        number of rows, it will only read the first 'limit_nrows' rows.
        If passing zero (the default), there will be no row limit.
    no_trailing_ws : bool
        Whether to assume that lines in the file will never have extra whitespaces
        at the end before a new line. Parsing large files with this option set to
        'True' can be 1.5x faster, but if the file does turn up to have e.g. extra
        spaces at the end of lines, the results will be incorrect.
    use_int64 : bool
        Whether to use 64-bit integers for column and label indices (when passing
        ``multilabel=True``). If passing ``False``, will use the machine's 'int' type
        (typically np.int32 but this could differ in non-standard CPU architectures).
        Using 'int' is faster and uses less memory, but cannot store values higher
        than the machine's 'INT_MAX' (typically 2^31-1, or around 2.2 billion).
    use_double : bool
        Whether to use C 'double' type (typically np.float64) for numeric values.
        If passing ``False``, will use C 'float' type (typically np.float32), which
        uses less memory but might be very slightly slower to parse. Most machine
        learning software for Python works with 'double' data.
    use_cpp : bool
        Whether to use C++ functions directly for file IO. If passing ``False``, will
        read the contents using Python's own functions into a string variable, from
        which the data will then be read. If passing ``True``, will parse from the file
        directly, which is faster and uses less memory. Using the C++ engine can have
        issues on Windows if the file is larger than 2GB and the library was compiled
        with something other than MSVC or MinGW.
    from_string : bool
        Whether to read the data from a string variable instead of a file.
        If passing ``from_string=True``, then ``file`` is assumed to be a variable with the
        data contents on it.

    Returns
    -------
    data : dict
        A dict with the following entries:
            - 'X' : the features, as a CSR Matrix from SciPy, with data of type ``np.float64``
              or ``np.float32`` depending on argument ``use_double``.
            - 'y' : the labels. If passing ``multilabel=False`` (the default), will be a vector
              (NumPy 1-d array, with dtype float64 when passing ``integer_labels=False``, or
              dtype `equal to the indices of 'X' when passing ``integer_labels=True``),
              otherwise will be a binary CSR Matrix (same dtype as the values of 'X').
            - 'qid' : the query IDs used for ranking, as an integer vector.
              This entry will **only** be present when passing ``has_qid=True``


    References
    ----------
    Datasets in this format can be found here:
        - LibSVM Data: https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets
        - Extreme Classification Repository: http://manikvarma.org/downloads/XC/XMLRepository.html

    The format is also described at the SVMLight webpage: http://svmlight.joachims.org
    """
    multilabel      =  bool(multilabel)
    has_qid         =  bool(has_qid)
    integer_labels  =  bool(integer_labels)
    index1          =  bool(index1)
    sort_indices    =  bool(sort_indices)
    ignore_zeros    =  bool(ignore_zeros)
    from_string     =  bool(from_string)

    min_cols = int(min_cols)
    min_classes = int(min_classes)
    limit_nrows = int(limit_nrows)
    if (min_cols < 0):
        raise ValueError("'min_cols' must be a non-negative integer.")
    if (min_classes < 0):
        raise ValueError("'min_classes' must be a non-negative integer.")
    if (limit_nrows < 0):
        raise ValueError("'limit_nrows' must be a non-negative integer.")

    if ("read" in dir(file)):
        file = file.read()
        from_string = True
    elif from_string and isinstance(file, bytes):
        file = file.decode()

    if not from_string:
        file = os.path.expanduser(file)
        if not os.path.exists(file):
            raise ValueError("Error: file does not exist.")

        if multilabel:
            read_func = _cpp_interface.read_multi_label_py
        else:
            read_func = _cpp_interface.read_single_label_py

    else: ## from_string
        if (not isinstance(file, str)) and (not isinstance(file, bytes)):
            raise ValueError("Error: 'file' is not a string.")

        if multilabel:
            read_func = _cpp_interface.read_multi_label_from_str_py
        else:
            read_func = _cpp_interface.read_single_label_from_str_py
    

    r = read_func(
            file,
            ignore_zero_valued = ignore_zeros,
            sort_indices = sort_indices,
            text_is_base1 = index1,
            assume_no_qid = not has_qid,
            use_int64 = use_int64,
            use_double = use_double,
            assume_trailing_ws = not no_trailing_ws,
            limit_nrows = limit_nrows
    )

    if len(r) == 0:
        raise ValueError("Error: could not read file successfully.")

    r["ncols"] = max(r["ncols"], min_cols)
    r["nclasses"] = max(r["nclasses"], min_classes)

    features = csr_matrix((r["values"], r["indices"], r["indptr"]),
                          shape=(r["nrows"], r["ncols"]))

    if multilabel:
        labels = csr_matrix((np.ones_like(r["indices_lab"], dtype=r["values"].dtype),
                             r["indices_lab"], r["indptr_lab"]),
                            shape=(r["nrows"], r["nclasses"]))
    else:
        labels = r["labels"]
        if integer_labels:
            if use_int64:
                labels = labels.astype(np.int64)
            else:
                labels = labels.astype(ctypes.c_int)

    if (not has_qid) and (r["indptr"].shape[0] > 1) and (r["indptr"][0] == r["indptr"][-1]):
        warnings.warn("Data has empty features. Perhaps the file has 'qid' field?")

    if not has_qid:
        return {"X":features, "y":labels}
    else:
        return {"X":features, "y":labels, "qid":r["qid"]}

def write_sparse(
        file, X, y, qid=None, integer_labels=True,
        index1=True, sort_indices=True, ignore_zeros=True,
        add_header=False, decimal_places=8,
        use_cpp=True, append=False, to_string=False
    ):
    """
    Write Sparse Matrix in Text Format

    Write a labelled sparse matrix into text format as used by software
    such as SVMLight, LibSVM, ThunderSVM, LibFM, xLearn, XGBoost, LightGBM, and others - i.e.:

    <labels(s)> <column:value> <column:value> ...


    For more information about the format and usage examples, see the documentation for 'read_sparse'.


    Can write labels for regression, classification (binary, multi-class, and multi-label),
    and ranking (with 'qid'), but note that most software that makes use of this data
    format supports only regression and binary classification.

    Note
    ----
    Be aware that writing sparse matrices to text is not a lossless operation
    - that is, some information might be lost due to numeric precision, and metadata such
    as row and column names will not be saved. It is recommended to use ``save_npz`` or similar
    for saving data between Python sessions, or to use binary formats for passing between
    different software such as Python->R.

    Note
    ----
    The option ``ignore_zeros`` is implemented heuristically, by comparing
    'abs(x) >= 10^(-decimal_places)/2', which might not match exactly with
    the rounding that is done implicitly in string conversions in the libc/libc++
    functions - thus there might still be some corner cases of all-zeros written into
    features if the (absolute) values are very close to the rounding threshold.
    
    Note
    ----
    The function uses different code paths when writing to a file or to a string,
    and there might be slight differences between the generated texts from them.
    If any such difference is encountered, please submit a bug report in the
    package's GitHub page.

    Parameters
    ----------
    file : str or None
        Output file path into which to write the data.
        Will be ignored when passing ``to_string=True``.
    X : CSR(n_samples, n_labels)
        Sparse data to write. Can be a sparse matrix from SciPy, a dense array from NumPy,
        or a DataFrame from Pandas, but will be converted to a CSR matrix if it isn't already.

        If ``X`` is a vector (1-d NumPy array), will be assumed to
        be a row vector and will thus write one row only.
    y : array(n_samples,) or CSR(n_samples, n_labels)
        Labels for the data. Can be passed as a vector
        if each observation has one label, or as a sparse or dense matrix (same format as ``X``)
        if each observation can have more than 1 label. In the latter case, only the non-missing
        column indices will be written, while the values are ignored.
    qid : None or array(n_samples,)
        Secondary label information used for ranking algorithms. Must be an integer vector
        if passed. Note that not all software supports this.
    integer_labels : bool
        Whether to write the labels as integers. If passing ``False``, they will
        have a decimal point regardless of whether they are integers or not. If the file is meant
        to be used for a classification algorithm, one should pass ``True`` here (the default).
         For multilabel classification, the labels will always be written as integers.
    index1 : bool
        Whether the column and label indices (if multi-label) should have numeration
        starting at 1. Most software assumes this is ``True``.
    sort_indices : bool
        Whether to sort the indices of ``X`` (and of ``y`` if multi-label) before
        writing the data. Note that this will cause in-place modifications if either ``X`` or ``y``
        are passed as CSR matrices.
    ignore_zeros : bool
        Whether to ignore (not write) features with a value of zero
        after rounding to the specified decimal places.
    add_header : bool
        Whether to add a header with metadata as the first line (number of rows,
        number of columns, number of classes). If passing ``integer_label=False`` and ``y`` is a
        vector, will write zero as the number of labels. This is not supported by most software.
    decimal_places : int
        Number of decimal places to use for numeric values. All values
        will have exactly this number of places after the decimal point. Be aware that values
        are rounded and might turn to zeros (will be skipped by default) if they are too small
        (one can do something like
        'X.data = np.where(X.data >= 0, X.data.clip(min=1e-8), X.data.clip(max=-1e-8))' to avoid this).
    use_cpp : bool
        Whether to use C++ functions directly for IO. If passing ``False``, will first write
        the output text into a Python string and then use Python's IO functions to write
        it to a file. If passing ``True``, will write to the file directly bypassing Python.
        Passing ``True`` is faster, but less resilient to errors.
    append : bool
        Whether to append text at the end of the file instead of overwriting or
        creating a new file. Ignored when passing ``to_string=True``.
    to_string : bool
        Whether to write the result into a string (which will be returned
        from the function) instead of into a file.

    Returns
    -------
    output : bool or str
        If passing ``to_string=False`` (the default), will return ``True`` if
        it completes successfully, or raise an error if it doesn't. If passing
        ``to_string=False``, will return the encoded matrix in text as a string
        variable.
    """
    if (X is None) or (y is None):
        raise ValueError("Must pass 'X' and 'y'.")

    decimal_places = int(decimal_places)
    if (decimal_places < 0):
        raise ValueError("'decimal_places' must be non-negative.")
    if (decimal_places > 20):
        warnings.warn("'decimal_places' is greater than 20.")

    integer_labels  =  bool(integer_labels)
    index1          =  bool(index1)
    sort_indices    =  bool(sort_indices)
    ignore_zeros    =  bool(ignore_zeros)
    add_header      =  bool(add_header)

    write_str_to_file_conn = False
    return_string = to_string
    if ("write" in dir(file)):
        write_str_to_file_conn = True
        to_string = True
    if not use_cpp:
        to_string = True

    ### Note: this check should be made right here due to potential
    ### in-place modifications of the data which would render the input unusable
    if sort_indices:
        if isspmatrix_csr(y):
            y.sort_indices()
        if isspmatrix_csr(X):
            X.sort_indices()
        sort_indices = False

    indptr, indices, values, nrows, ncols = _process_X(X)
    indptr_lab, indices_lab, labels, nrows_y, nclasses = _process_y(y, add_header, integer_labels)
    qid = _process_qid(qid)
    if nrows != nrows_y:
        raise ValueError("'X' and 'y' must have the same number of rows.")
    if (qid.shape[0]) and (qid.shape[0] != nrows):
        raise ValueError("'X' and 'qid' must have the same number of rows.")

    indptr, indices, indptr_lab, indices_lab, qid = \
        _as_highest_dtype(
            indptr,
            indices,
            indptr_lab,
            indices_lab,
            qid
        )

    if not to_string:
        if (add_header) and (append) and (os.path.exists(file)):
            warnings.warn("Warning: adding header to existing file with 'append=TRUE'.")

        if indptr_lab is not None:
            success = _cpp_interface.write_multi_label_py(
                file,
                indptr,
                indices,
                values,
                indptr_lab,
                indices_lab,
                qid,
                ncols,
                nclasses,
                ignore_zero_valued = ignore_zeros,
                sort_indices = sort_indices,
                text_is_base1 = index1,
                add_header = add_header,
                decimal_places = decimal_places,
                append = append
            )
        else:
            success = _cpp_interface.write_single_label_py(
                file,
                indptr,
                indices,
                values,
                labels,
                qid,
                ncols,
                nclasses,
                ignore_zero_valued = ignore_zeros,
                sort_indices = sort_indices,
                text_is_base1 = index1,
                add_header = add_header,
                decimal_places = decimal_places,
                append = append
            )
        if not success:
            raise ValueError("Error: file write failed.")
        return success

    else: ### to_string=True
        if indptr_lab is not None:
            res = _cpp_interface.write_multi_label_to_str_py(
                indptr,
                indices,
                values,
                indptr_lab,
                indices_lab,
                qid,
                ncols,
                nclasses,
                ignore_zero_valued = ignore_zeros,
                sort_indices = sort_indices,
                text_is_base1 = index1,
                add_header = add_header,
                decimal_places = decimal_places,
                append = append
            )
        else:
            res = _cpp_interface.write_single_label_to_str_py(
                indptr,
                indices,
                values,
                labels,
                qid,
                ncols,
                nclasses,
                ignore_zero_valued = ignore_zeros,
                sort_indices = sort_indices,
                text_is_base1 = index1,
                add_header = add_header,
                decimal_places = decimal_places,
                append = append
            )

        if return_string:
            return res.decode()

        if write_str_to_file_conn:
            file.write(res)
            return True

        if use_cpp:
            with open(file, "a" if append else "w") as f:
                f.write(res)
            return True
        else:
            return res


### Helpers
dtypes_indices = [ctypes.c_int, np.int64, ctypes.c_size_t]
dtypes_values = [np.float32, np.float64]
dtypes_labels = dtypes_indices + dtypes_values

def _process_X(X):
    if not isspmatrix_csr(X):
        X = csr_matrix(X)
    return (
        X.indptr,
        X.indices,
        X.data if X.data.dtype in dtypes_values else X.data.astype(np.float64),
        X.shape[0], X.shape[1]
    )

def _process_y(y, add_header=False, integer_labels=True):
    if y.__class__.__name__ == "Categorical":
        y = y.codes
    if y.__class__.__name__ == "Series":
        y = y.to_numpy()
    if isinstance(y, list) or isinstance(y, tuple):
        y = np.array(y)
    if np.isscalar(y):
        y = np.array([y])

    if isinstance(y, np.ndarray):
        if (len(y.shape) == 0):
            y = y.reshape(-1)
        if (len(y.shape) > 2):
            raise ValueError("'y' cannot have dimension greater than 2.")
        ### will reach here when 'y' is a vector
        if (len(y.shape) == 1):
            if y.dtype not in dtypes_labels:
                if integer_labels:
                    y = y.astype(np.int64)
                else:
                    y = y.astype(np.float64)

            nclasses = 0
            if (add_header) and (integer_labels):
                if np.any(np.isinf(y)):
                    nclasses = 0
                elif np.any(y < 0):
                    nclasses = np.unique(y).shape[0]
                else:
                    nclasses = y.max()
                    if 0 in y:
                        nclasses += 1
                    nclasses = max(nclasses, 1)
                    if np.isinf(nclasses) or np.isnan(nclasses):
                        nclasses = 0
                    nclasses = int(nclasses)

            if (integer_labels) and (y.dtype not in dtypes_indices):
                y = y.astype(np.int64)

            return None, None, y, y.shape[0], nclasses

    if not isspmatrix_csr(y):
        y = csr_matrix(y)
    return (
        y.indptr,
        y.indices,
        None, y.shape[0], y.shape[1]
    )

def _process_qid(qid):
    if qid is None:
        return np.empty(0, np.int64)
    if qid.__class__.__name__ == "Categorical":
        qid = qid.codes
    if qid.__class__.__name__ == "Series":
        qid = qid.to_numpy()
    if np.isscalar(qid):
        qid = np.array([qid])
    if not isinstance(qid, np.ndarray):
        qid = np.array(qid)
    if len(qid.shape) != 1:
        warnings.warn("'qid' is not unidimensional, will reshape.")
        qid = qid.reshape(-1)
    return qid

def _as_highest_dtype(
        indptr,
        indices,
        indptr_lab,
        indices_lab,
        qid
    ):
    highest_int_type = ctypes.c_int
    def promote_dtype(dtype, curr_highest):
        if np.iinfo(dtype).max > np.iinfo(curr_highest).max:
            return dtype
        else:
            return curr_highest
    
    if indptr.dtype in dtypes_indices:
        highest_int_type = promote_dtype(indptr.dtype, highest_int_type)
    else:
        highest_int_type = promote_dtype(np.int64, highest_int_type)
    if indices.dtype in dtypes_indices:
        highest_int_type = promote_dtype(indices.dtype, highest_int_type)
    else:
        highest_int_type = promote_dtype(np.int64, highest_int_type)
    if indptr_lab is not None:
        if indptr_lab.dtype in dtypes_indices:
            highest_int_type = promote_dtype(indptr_lab.dtype, highest_int_type)
        else:
            highest_int_type = promote_dtype(np.int64, highest_int_type)
    if indices_lab is not None:
        if indices_lab.dtype in dtypes_indices:
            highest_int_type = promote_dtype(indices_lab.dtype, highest_int_type)
        else:
            highest_int_type = promote_dtype(np.int64, highest_int_type)
    if qid.shape[0]:
        if qid.dtype in dtypes_indices:
            highest_int_type = promote_dtype(qid.dtype, highest_int_type)
        else:
            highest_int_type = promote_dtype(np.int64, highest_int_type)

    if indptr.dtype != highest_int_type:
        indptr = indptr.astype(highest_int_type)
    if indices.dtype != highest_int_type:
        indices = indices.astype(highest_int_type)
    if indptr_lab is not None:
        if indptr_lab.dtype != highest_int_type:
            indptr_lab = indptr_lab.astype(highest_int_type)
    if indices_lab is not None:
        if indices_lab.dtype != highest_int_type:
            indices_lab = indices_lab.astype(highest_int_type)
    if qid.dtype != highest_int_type:
        qid = qid.astype(highest_int_type)

    return indptr, indices, indptr_lab, indices_lab, qid
