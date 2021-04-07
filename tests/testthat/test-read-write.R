library("testthat")
library("readsparse")
context("Read and Write sparse matrices")

test_that("Regression mode", {
    txt_mat <- paste(
        "-1.234 1:10 4:4.500000000",
        "0 ",
        "1e3 1:.001 2:5e-3",
        sep="\n"
    )
    r <- read.sparse(txt_mat, from_string=TRUE)
    expect_s4_class(r$X, "dgRMatrix")
    expect_type(r$y, "double")

    expected_X <- matrix(c(10, 0, 0, 4.5, 0, 0, 0, 0, 0.001, 0.005, 0, 0),
                         nrow=3, ncol=4, byrow=TRUE)
    expected_y <- c(-1.234, 0, 1000)
    compare_vals <- function(expected_X, expected_y, X, y) {
        expect_equal(expected_X, unname(as.matrix(X)))
        expect_equal(expected_y, y)
    }

    compare_vals(expected_X, expected_y, r$X, r$y)

    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    write.sparse(file_name, r$X, r$y, integer_labels=FALSE)
    r <- read.sparse(file_name, from_string=FALSE)
    compare_vals(expected_X, expected_y, r$X, r$y)

    s <- write.sparse(file_name, r$X, r$y, integer_labels=FALSE, to_string=TRUE)
    r <- read.sparse(s, from_string=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)
})

test_that("Classification mode", {
    txt_mat <- paste(
        "1 1:10 4:4.500000000",
        "0 ",
        "2 1:.001 2:5e-3",
        sep="\n"
    )
    r <- read.sparse(txt_mat, from_string=TRUE, integer_labels=TRUE)
    expect_s4_class(r$X, "dgRMatrix")
    expect_type(r$y, "integer")

    expected_X <- matrix(c(10, 0, 0, 4.5, 0, 0, 0, 0, 0.001, 0.005, 0, 0),
                         nrow=3, ncol=4, byrow=TRUE)
    expected_y <- c(1L, 0L, 2L)
    compare_vals <- function(expected_X, expected_y, X, y) {
        expect_equal(expected_X, unname(as.matrix(X)))
        expect_equal(expected_y, y)
    }

    compare_vals(expected_X, expected_y, r$X, r$y)

    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    write.sparse(file_name, r$X, r$y, integer_labels=TRUE)
    r <- read.sparse(file_name, from_string=FALSE)
    compare_vals(expected_X, expected_y, r$X, r$y)

    s <- write.sparse(file_name, r$X, r$y, integer_labels=TRUE,
                      to_string=TRUE, add_header=TRUE)
    r <- read.sparse(s, from_string=TRUE, integer_labels=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)
})

test_that("Multilabel mode", {
    txt_mat <- paste(
        "1,2 1:10 4:4.500000000",
        " ",
        "3 1:.001 2:5e-3",
        sep="\n"
    )
    r <- read.sparse(txt_mat, from_string=TRUE, multilabel=TRUE)
    expect_s4_class(r$X, "dgRMatrix")
    expect_s4_class(r$y, "ngRMatrix")

    expected_X <- matrix(c(10, 0, 0, 4.5, 0, 0, 0, 0, 0.001, 0.005, 0, 0),
                         nrow=3, ncol=4, byrow=TRUE)
    expected_y <- matrix(c(1,1,0,0,0,0,0,0,1),
                         nrow=3, ncol=3, byrow=TRUE)
    compare_vals <- function(expected_X, expected_y, X, y) {
        y <- unname(as.matrix(y))
        mode(y) <- "double"
        expect_equal(expected_X, unname(as.matrix(X)))
        expect_equal(expected_y, y)
    }

    compare_vals(expected_X, expected_y, r$X, r$y)

    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    write.sparse(file_name, r$X, r$y)
    r <- read.sparse(file_name, from_string=FALSE, multilabel=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)

    s <- write.sparse(file_name, r$X, r$y,
                      to_string=TRUE, add_header=TRUE)
    r <- read.sparse(s, from_string=TRUE, multilabel=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)
    
    
    s <- write.sparse(file_name, r$X, as.matrix(r$y),
                      to_string=TRUE, add_header=TRUE)
    r <- read.sparse(s, from_string=TRUE, multilabel=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)
    
    
    s <- write.sparse(file_name, r$X, as(as.matrix(r$y), "RsparseMatrix"),
                      to_string=TRUE, add_header=TRUE)
    r <- read.sparse(s, from_string=TRUE, multilabel=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)
})

test_that("Ranking mode", {
    txt_mat <- paste(
        "1 qid:1 1:10 4:4.500000000",
        "0 qid:2",
        "2 qid:1 1:.001 2:5e-3",
        sep="\n"
    )
    r <- read.sparse(txt_mat, from_string=TRUE, integer_labels=TRUE, has_qid=TRUE)
    expect_s4_class(r$X, "dgRMatrix")
    expect_type(r$y, "integer")
    expect_type(r$qid, "integer")

    expected_X <- matrix(c(10, 0, 0, 4.5, 0, 0, 0, 0, 0.001, 0.005, 0, 0),
                         nrow=3, ncol=4, byrow=TRUE)
    expected_y <- c(1L, 0L, 2L)
    expected_qid <- c(1L, 2L, 1L)
    compare_vals <- function(expected_X, expected_y, expected_qid, X, y, qid) {
        expect_equal(expected_X, unname(as.matrix(X)))
        expect_equal(expected_y, y)
        expect_equal(expected_qid, qid)
    }

    compare_vals(expected_X, expected_y, expected_qid, r$X, r$y, r$qid)

    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    write.sparse(file_name, r$X, r$y, r$qid, integer_labels=TRUE)
    r <- read.sparse(file_name, from_string=FALSE, has_qid=TRUE)
    compare_vals(expected_X, expected_y, expected_qid, r$X, r$y, r$qid)

    s <- write.sparse(file_name, r$X, r$y, r$qid, integer_labels=TRUE,
                      to_string=TRUE, add_header=TRUE)
    r <- read.sparse(s, from_string=TRUE, integer_labels=TRUE, has_qid=TRUE)
    compare_vals(expected_X, expected_y, expected_qid, r$X, r$y, r$qid)
})

test_that("Non-ascii file names", {
    if (readsparse_nonascii_support()) {
        X <- matrix(1:10, nrow=5)
        y <- 11:15
        mode(X) <- "numeric"
        mode(y) <- "numeric"
        
        file_name <- "\u00d1o\u00f1o.txt"
        file_name <- file.path(tempdir(), file_name)
        write.sparse(file_name, X, y)
        r <- read.sparse(file_name)
        expect_equal(unname(X), unname(as.matrix(r$X)))
        expect_equal(unname(y), unname(as.numeric(r$y)))
        
        
        file_name <- "\u0440\u0443\u0441\u0441\u043a\u0438\u0439.txt"
        file_name <- file.path(tempdir(), file_name)
        write.sparse(file_name, X, y)
        r <- read.sparse(file_name)
        expect_equal(unname(X), unname(as.matrix(r$X)))
        expect_equal(unname(y), unname(as.numeric(r$y)))
        
        if (Sys.info()['sysname'] == "Windows") {
            file_name <- "\u0440\u0443\u0441\u0441\u043a\u0438\u0439.txt"
            f_backslash <- paste(tempdir(), file_name, sep="\\")
            f_forwardslash <- paste(tempdir(), file_name, sep="//")
            
            write.sparse(f_backslash, X, y)
            r <- read.sparse(f_backslash)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
            
            write.sparse(f_forwardslash, X, y)
            r <- read.sparse(f_forwardslash)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
        }
    } else {
        testthat::expect_true(TRUE)
    }
})
