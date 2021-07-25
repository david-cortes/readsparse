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

#ifndef _FOR_R
#   define throw_err(msg) {fprintf(stderr, "%s", msg); fflush(stderr);}
#else
#   define throw_err(msg) REprintf(msg)
#endif

static void print_errno()
{
    char msg[1000];
    snprintf(msg, 999, "Error %d: %s\n", errno, strerror(errno));
    throw_err(msg);
}


template <class int_t>
bool check_is_sorted(int_t* vec, size_t n)
{
    if (n <= 1)
        return true;
    if (vec[n-1] < vec[0])
        return false;
    for (size_t ix = 1; ix < n; ix++)
        if (vec[ix] < vec[ix-1])
            return false;
    return true;
}

template <class int_t, class real_t>
void sort_sparse_indices
(
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *values,
    size_t nrows,
    bool has_values
)
{
    std::vector<size_t> argsorted;
    std::vector<int_t> temp_indices;
    std::vector<real_t> temp_values;
    size_t ix1, ix2;
    size_t n_this;

    for (size_t ix = 1; ix <= nrows; ix++)
    {
        ix1 = indptr[ix-1];
        ix2 = indptr[ix];
        n_this = ix2 - ix1;
        if (n_this)
        {
            if (!check_is_sorted<int_t>(indices + ix1, n_this))
            {
                if (argsorted.size() < n_this) {
                    argsorted.resize(n_this);
                    temp_indices.resize(n_this);
                    if (has_values) temp_values.resize(n_this);
                }
                std::iota(argsorted.begin(), argsorted.begin() + n_this, (size_t)ix1);
                std::sort(argsorted.begin(), argsorted.begin() + n_this,
                          [&indices](const size_t a, const size_t b)
                          {return indices[a] < indices[b];});
                for (size_t ix = 0; ix < n_this; ix++)
                    temp_indices[ix] = indices[argsorted[ix]];
                std::copy(temp_indices.begin(), temp_indices.begin() + n_this, indices + ix1);
                if (has_values)
                {
                    for (size_t ix = 0; ix < n_this; ix++)
                        temp_values[ix] = values[argsorted[ix]];
                    std::copy(temp_values.begin(), temp_values.begin() + n_this, values + ix1);
                }
            }
        }
    }
}

template <class int_t, class real_t>
void sort_sparse_indices_known_ncol
(
    int_t *restrict indptr,
    int_t *restrict indices,
    real_t *values,
    size_t nrows, size_t ncol,
    bool has_values
)
{
    std::unique_ptr<size_t[]> argsorted_(new size_t[ncol]);
    std::unique_ptr<int_t[]> temp_indices_(new int_t[ncol]);
    std::unique_ptr<real_t[]> temp_values_(new real_t[has_values? ncol : 0]);
    auto *restrict argsorted = argsorted_.get();
    auto *restrict temp_indices = temp_values_.get();
    auto *restrict temp_values = temp_values_.get();
    size_t ix1, ix2;
    size_t n_this;

    for (size_t ix = 1; ix <= nrows; ix++)
    {
        ix1 = indptr[ix-1];
        ix2 = indptr[ix];
        n_this = ix2 - ix1;
        if (n_this)
        {
            if (!check_is_sorted<int_t>(indices + ix1, n_this))
            {
                std::iota(argsorted, argsorted + n_this, (size_t)ix1);
                std::sort(argsorted, argsorted + n_this,
                          [&indices](const size_t a, const size_t b)
                          {return indices[a] < indices[b];});
                for (size_t ix = 0; ix < n_this; ix++)
                    temp_indices[ix] = indices[argsorted[ix]];
                std::copy(temp_indices, temp_indices + n_this, indices + ix1);
                if (has_values)
                {
                    for (size_t ix = 0; ix < n_this; ix++)
                        temp_values[ix] = values[argsorted[ix]];
                    std::copy(temp_values, temp_values + n_this, values + ix1);
                }
            }
        }
    }
}

template <class int_t, class real_t>
void sort_sparse_indices
(
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values
)
{
    sort_sparse_indices<int_t, real_t>(
        indptr.data(),
        indices.data(),
        values.data(),
        indptr.size()-1,
        values.size() > 0
    );
}

template <class int_t>
void sort_sparse_indices
(
    int_t *indptr,
    int_t *indices,
    size_t nrows
)
{
    sort_sparse_indices<int_t, double>(
        indptr,
        indices,
        (double*)NULL,
        nrows,
        false
    );
}

template <class int_t, class real_t>
void sort_sparse_indices
(
    int_t *indptr,
    int_t *indices,
    real_t *values,
    size_t nrows
)
{
    sort_sparse_indices<int_t, real_t>(
        indptr,
        indices,
        values,
        nrows,
        true
    );
}


template <class int_t, class real_t>
void sort_sparse_indices_known_ncol
(
    std::vector<int_t> &indptr,
    std::vector<int_t> &indices,
    std::vector<real_t> &values,
    size_t ncol
)
{
    sort_sparse_indices_known_ncol<int_t, real_t>(
        indptr.data(),
        indices.data(),
        values.data(),
        indptr.size()-1, ncol,
        values.size() > 0
    );
}

template <class int_t>
void sort_sparse_indices_known_ncol
(
    int_t *indptr,
    int_t *indices,
    size_t nrows, size_t ncol
)
{
    sort_sparse_indices_known_ncol<int_t, double>(
        indptr,
        indices,
        (double*)NULL,
        nrows, ncol,
        false
    );
}

template <class int_t, class real_t>
void sort_sparse_indices_known_ncol
(
    int_t *indptr,
    int_t *indices,
    real_t *values,
    size_t nrows, size_t ncol
)
{
    sort_sparse_indices_known_ncol<int_t, real_t>(
        indptr,
        indices,
        values,
        nrows, ncol,
        true
    );
}
