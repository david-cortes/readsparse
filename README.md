# ReadSparse

Efficient library for reading and writing labelled sparse matrices in delimited text format (a.k.a. SVMLight format), as used by software such as SVMLight, LibSVM, ThunderSVM, LibFM, xLearn, XGBoost, LightGBM, and others. Supports labels for regression, classification (binary, multi-class, and multi-label), and ranking (with `qid` field).

Written in C++ with interfaces for Python and R. As a comparison point, the library is about 2.2x faster than SciKit-Learn at reading large files.

# Supported formats

The aim of the library is to read and write sparse CSR matrices into text in a delimited format (which is used by popular machine learning software) in which each row is represented by a line, with the data written as follows:
```
<label(s)> <column>:<value> <column>:<value> ...
```

Example line (row):
```
1 1:1.234 3:20
```
This line denotes a row with label (target variable) equal to 1, a value for the first column of 1.234, a value of zero for the second column (which is missing), and a value of 20 for the third column.


Lines can come in slightly different formats according to the desired task:


* Regression

```
0.321 2:1.21 5:2.05
-1.234 1:0.45 3:0.001 4:-10
```

* Classification

```
1 2:1.21 5:2.05
-1 1:0.45 3:0.001 4:-10
```

* Multi-Label classification

```
1,2,3 2:1.21 5:2.05
2 1:0.45 3:0.001 4:-10
```

* Ranking

```
1 qid:1 2:1.21 5:2.05
2 qid:2 1:0.45 3:0.001 4:-10
```

Lines might also contain comments (everything after a `#` is considered a comment) and/or a header with metadata (number of rows, columns, and classes).


# Installation

* Python

```
pip install git+https://www.github.com/david-cortes/readsparse.git
```

(A small note: on Windows, if compiling with MinGW, will use its default `stdio` library, which at the time of writing takes it from an outdated MSVC library. To use MinGW's own workarounds for `stdio`, one can define an environment variable `ANSISTDIO` or pass argument `-ansistdio` to `setup.py`)


* R

```
remotes::install_github("david-cortes/readsparse")
```


* C++

```
git clone https://www.github.com/david-cortes/readsparse.git
mkdir build
cd build
cmake ..
make
sudo make install
```

# Documentation

* Python: documentation is available at [ReadTheDocs](http://readsparse.readthedocs.io/en/latest/).

* R: documentation is internally available (e.g. `?readsparse::read.sparse` and `?readsparse::write.sparse`).

* C++: documentation is available under the [public header](https://github.com/david-cortes/readsparse/blob/master/src/readsparse.hpp).


# Sample usage

* Python

```python
import numpy as np
import readsparse
import scipy.sparse as sp

coded_matrix = """
1 2:1.21 5:2.05
-1 1:0.45 3:0.001 4:-10
""".strip()

r = readsparse.read_sparse(coded_matrix, from_string=True)
r
```
```
{'X': <2x5 sparse matrix of type '<class 'numpy.float64'>'
    with 5 stored elements in Compressed Sparse Row format>,
 'y': array([ 1., -1.])}
```
```python
r["X"].toarray()
```
```
array([[ 0.00e+00,  1.21e+00,  0.00e+00,  0.00e+00,  2.05e+00],
       [ 4.50e-01,  0.00e+00,  1.00e-03, -1.00e+01,  0.00e+00]])
```
```python
### Convert it back to text
recoded_martix = readsparse.write_sparse(file=None, X=r["X"], y=r["y"], to_string=True)
print(recoded_martix)
```
```
1.00000000 2:1.21000000 5:2.05000000
-1.00000000 1:0.45000000 3:0.00100000 4:-10.00000000
```
```python
### Example with file I/O
## generate a random sparse matrix and labels
np.random.seed(1)
X = sp.random(m=5, n=10, density=0.2)
y = np.random.normal(size=5)

## save into a text file
temp_file = "matrix.txt"
readsparse.write_sparse(temp_file, X, y, integer_labels=False)

## inspect the text file
with open(temp_file, "r") as f:
    print(f.read())
```
```
-1.27321995 8:0.67165410 9:0.41178788
1.01498680 7:0.62402999 10:0.03417131
-1.48105971 1:0.28962964 6:0.59306552
-0.28709989 1:0.14212014 8:0.19755090 10:0.78331447
-0.05682428 6:0.41253884
```
```python
## read it back
r = readsparse.read_sparse(temp_file)
r
```
```
{'X': <5x10 sparse matrix of type '<class 'numpy.float64'>'
    with 10 stored elements in Compressed Sparse Row format>,
 'y': array([-1.27321995,  1.0149868 , -1.48105971, -0.28709989, -0.05682428])}
```

* R

(Example also available under the internal documentation)

```r
library(Matrix)
library(readsparse)

### Example input file
"1 2:1.21 5:2.05
-1 1:0.45 3:0.001 4:-10" -> coded.matrix

r <- read.sparse(coded.matrix, from_string=TRUE)
print(r)
```
```
$X
2 x 5 sparse Matrix of class "dgRMatrix"
                             
[1,] .    1.21 .       . 2.05
[2,] 0.45 .    0.001 -10 .   

$y
[1]  1 -1
```
```r
### Convert it back to text
recoded.matrix <- write.sparse(file=NULL, X=r$X, y=r$y, to_string=TRUE)
cat(recoded.matrix)
```
```
1 2:1.21000000 5:2.05000000
-1 1:0.45000000 3:0.00100000 4:-10.00000000
```
```r
### Example with file I/O
## generate a random sparse matrix and labels
set.seed(1)
X <- rsparsematrix(nrow=5, ncol=10, nnz=8)
y <- rnorm(5)

## save into a text file
temp_file <- file.path(tempdir(), "matrix.txt")
write.sparse(temp_file, X, y, integer_labels=FALSE)

## inspect the text file
cat(paste(readLines(temp_file), collapse="\n"))
```
```
-2.21469989 1:0.74000000
1.12493092 
-0.04493361 4:-0.62000000 5:-0.31000000 9:1.50000000
-0.01619026 1:-0.82000000 3:0.39000000 7:0.58000000 8:0.49000000
0.94383621
```
```r
## read it back
r <- read.sparse(temp_file)
print(r)
```
```
$X
5 x 9 sparse Matrix of class "dgRMatrix"
                                             
[1,]  0.74 . .     .     .    . .    .    .  
[2,]  .    . .     .     .    . .    .    .  
[3,]  .    . .    -0.62 -0.31 . .    .    1.5
[4,] -0.82 . 0.39  .     .    . 0.58 0.49 .  
[5,]  .    . .     .     .    . .    .    .  

$y
[1] -2.21469989  1.12493092 -0.04493361 -0.01619026  0.94383621
```

* C++

See file [readsparse_cpp_example.cpp](https://github.com/david-cortes/readsparse/blob/master/example/readsparse_cpp_example.cpp)

# Where to find datasets

Public datasets in this format can be found under the [LibSVM datasets](https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets) and the [Extreme Classification Repository](http://manikvarma.org/downloads/XC/XMLRepository.html).
