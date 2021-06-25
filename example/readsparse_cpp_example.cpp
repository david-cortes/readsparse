/*  Example for using ReadSparse to read/write CSR matrices to/from text.
    
    To compile this example, build the library with the cmake system - e.g.:
        mkdir build
        cd build
        cmake ..
        make
        ### optionally
        sudo make install
        sudo ldconfig
    Then compile this file linking to the created shared object and including
    the public header - examples:
        g++ example/readsparse_cpp_example.cpp -lreadsparse (for system-wide installs)
            or
        g++ example/readsparse_cpp_example.cpp -I./src \
            -L./build -l:libreadsparse.so -Wl,-rpath,./build (taking it from folder 'build')
    Then run the produced executable - e.g.:
        ./a.out
 */
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "readsparse.hpp"

void print_sparse_CSR_matrix(int indptr[], int indices[], double values[], size_t nrows);
void print_labels(double values[], size_t nrows);

/* This is the matrix that will be read */
const char *matrix_as_txt = 
"1 2:1.21 5:2.05" "\n"
"-1 1:0.45 3:0.001 4:-10" "\n";

int main()
{
    printf("--- ReadSparse short example ---\n\n");
    printf("Will read a labelled CSR matrix from the following text:\n");
    printf("%s\n", matrix_as_txt);

    std::string matrix_as_str(matrix_as_txt);
    std::stringstream ss;
    ss.str(matrix_as_txt); /* <- will read from this */

    /* Allocate vectors for the matrix structure
       (note that the library is templated and comes in many
        different combinations of real/integer widths) */
    std::vector<double> values, labels;
    std::vector<int> indptr, indices;
    size_t nrows, ncols;

    /* Other fields which are not used */
    std::vector<int> qid;
    size_t nclasses;


    /* Read the matrix from text */
    read_single_label(
        ss,
        indptr,
        indices,
        values,
        labels,
        qid,
        nrows,
        ncols,
        nclasses,
        0,
        true,
        true,
        true,
        true,
        true
    );

    /* Examine the obtained results */
    printf("Obtained objects:\n");
    print_sparse_CSR_matrix(indptr.data(), indices.data(), values.data(), nrows);
    print_labels(labels.data(), nrows);

    /* Now write it back to text */
    std::stringstream ss_recoded;
    write_single_label(
        ss_recoded,
        indptr.data(),
        indices.data(),
        values.data(),
        labels.data(),
        qid.data(),
        INT_MAX,
        NAN,
        false,
        nrows,
        ncols,
        nclasses,
        true,
        true,
        true,
        false,
        3
    );
    std::string matrix_recoded = ss_recoded.str();
    printf("\nRecoded matrix:\n");
    printf("%s\n", matrix_recoded.c_str());

    printf("\n--- End of Example ---\n");

    return 0;
}

void print_sparse_CSR_matrix(int indptr[], int indices[], double values[], size_t nrows)
{
    /* This is an inefficient function which will first convert the matrix to dense */

    /* First determine the number of columns */
    size_t ncols = 0;
    for (size_t row = 0; row < nrows; row++)
    {
        for (size_t ix = indptr[row]; ix < indptr[row+1]; ix++)
            ncols = std::max((size_t)indices[ix]+1, ncols);
    }

    /* Now allocate a dense matrix (row-major) and fill it */
    std::vector<double> X(nrows * ncols);
    for (size_t row = 0; row < nrows; row++)
    {
        for (size_t ix = indptr[row]; ix < indptr[row+1]; ix++)
            X[indices[ix] + row*ncols] = values[ix];
    }

    /* Finally, print the matrix */
    printf("X matrix:\n");
    printf("[");
    for (size_t row = 0; row < nrows; row++)
    {
        if (row > 0)
            printf(" ");
        printf("[ ");

        for (size_t col = 0; col < ncols; col++)
        {
            if (X[col + row*ncols] == 0)
                printf("  .    ");
            else {
                if (X[col + row*ncols] > 0)
                    printf("% 2.3f ", X[col + row*ncols]);
                else
                    printf("%2.2f ", X[col + row*ncols]);
            }
        }

        printf("]");
        if (row != nrows-1)
            printf(",\n");
    }
    printf("]\n");
}

void print_labels(double values[], size_t nrows)
{
    printf("Labels: [");
    for (size_t row = 0; row < nrows; row++)
    {
        if (row != nrows-1)
            printf("%.1f, ", values[row]);
        else
            printf("%.1f", values[row]);
    }
    printf("]\n");
}
