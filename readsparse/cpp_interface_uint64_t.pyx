#cython: language_level=3

from libc.stdint cimport uint64_t

ctypedef uint64_t size_large

include "cpp_interface.pxi"
