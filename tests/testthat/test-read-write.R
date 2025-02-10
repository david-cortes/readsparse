library("testthat")
library("readsparse")
context("Read and Write sparse matrices")

# Note: tests are skipped when it detects that it was compiled with GCC.
# Reason being that there's a compiler bug that shows up at CRAN checks:
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=117457
# The bug there only happens when compiling with all three of: (a) LTO,
# (b) address sanitizer, (c) O2 or higher; so it should not affect any
# "production" builds (since they won't use the address sanitizer), but
# this was necessary to keep the CRAN checks passing.

skip_tests <- compiled_with_gcc_internal()

test_that("Regression mode", {
    skip_if(skip_tests)
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
    skip_if(skip_tests)
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
    skip_if(skip_tests)
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
    
    
    s <- write.sparse(file_name, r$X, as(as(as.matrix(r$y), "RsparseMatrix"), "generalMatrix"),
                      to_string=TRUE, add_header=TRUE)
    r <- read.sparse(s, from_string=TRUE, multilabel=TRUE)
    compare_vals(expected_X, expected_y, r$X, r$y)
})

test_that("Ranking mode", {
    skip_if(skip_tests)
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
    skip_if(skip_tests)
    has_utf8 <- grepl("UTF-?8$", Sys.getenv("LANG"))
    if (has_utf8) {
        X <- matrix(1:10, nrow=5)
        y <- 11:15
        mode(X) <- "numeric"
        mode(y) <- "numeric"
        
        is_windows <- Sys.info()['sysname'] == "Windows"
        
        file_name <- "\u00d1o\u00f1o.txt"
        file_name <- file.path(tempdir(), file_name)
        write.sparse(file_name, X, y)
        if (has_utf8 || is_windows)
            expect_true(file.exists(file_name))
        r <- read.sparse(file_name)
        expect_equal(unname(X), unname(as.matrix(r$X)))
        expect_equal(unname(y), unname(as.numeric(r$y)))
        if (has_utf8 || is_windows)
            file.remove(file_name)
        
        if (has_utf8 || is_windows) {
            s <- write.sparse(NULL, X, y, to_string=TRUE)
            writeLines(s, file_name, sep="")
            r <- read.sparse(file_name)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
        }
        
        if (has_utf8) {
            file_name <- "\u0440\u0443\u0441\u0441\u043a\u0438\u0439.txt"
            file_name <- file.path(tempdir(), file_name)
            write.sparse(file_name, X, y)
            if (has_utf8 || is_windows)
                expect_true(file.exists(file_name))
            r <- read.sparse(file_name)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
            if (has_utf8 || is_windows)
                file.remove(file_name)
        }
        
        if (has_utf8) {
            s <- write.sparse(NULL, X, y, to_string=TRUE)
            writeLines(s, file_name, sep="")
            r <- read.sparse(file_name)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
        }
        
        if (is_windows) {
            file_name <- "\u0440\u0443\u0441\u0441\u043a\u0438\u0439_2.txt"
            f_backslash <- paste(tempdir(), file_name, sep="\\")
            f_forwardslash <- paste(tempdir(), file_name, sep="//")
            
            write.sparse(f_backslash, X, y)
            r <- read.sparse(f_backslash)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
            file.remove(f_backslash)
            
            write.sparse(f_forwardslash, X, y)
            r <- read.sparse(f_forwardslash)
            expect_equal(unname(X), unname(as.matrix(r$X)))
            expect_equal(unname(y), unname(as.numeric(r$y)))
            file.remove(f_forwardslash)
        }
    } else {
        testthat::expect_true(TRUE)
    }
})

test_that("Problematic input 1", {
    skip_if(skip_tests)

    txt_mat <- "1 19767:0.5479394618272178 
1 20336:0.3895789860528069 
"
    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    writeLines(txt_mat, con = file_name, sep = "")
    r1 <- read.sparse(file_name, from_string=FALSE)
    r2 <- read.sparse(txt_mat, from_string=TRUE)
    r3 <- read.sparse(file_name, from_string=FALSE, multilabel=TRUE)
    r4 <- read.sparse(txt_mat, from_string=TRUE, multilabel=TRUE)
    
    expect_equal(r1$X@p, r2$X@p)
    expect_equal(r1$X@j, r2$X@j)
    expect_equal(r1$X@x, r2$X@x)

    expect_equal(r1$X@p, r3$X@p)
    expect_equal(r1$X@j, r3$X@j)
    expect_equal(r1$X@x, r3$X@x)

    expect_equal(r1$X@p, r4$X@p)
    expect_equal(r1$X@j, r4$X@j)
    expect_equal(r1$X@x, r4$X@x)

    expect_equal(r1$X@p, c(0L, 1L, 2L))
    expect_equal(r1$X@j, c(19767L-1L, 20336L-1L))
    expect_equal(r1$X@x, c(0.5479394618272178, 0.3895789860528069))
})

test_that("Problematic input 2", {
    skip_if(skip_tests)

    txt_mat <- "1 3:0.54 5:4.2 
 7:3
9 2:0.38 
"
    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    writeLines(txt_mat, con = file_name, sep = "")
    r1 <- read.sparse(file_name, from_string=FALSE)
    r2 <- read.sparse(txt_mat, from_string=TRUE)
    r3 <- read.sparse(file_name, from_string=FALSE, multilabel=TRUE)
    r4 <- read.sparse(txt_mat, from_string=TRUE, multilabel=TRUE)
    
    expect_equal(r1$X@p, r2$X@p)
    expect_equal(r1$X@j, r2$X@j)
    expect_equal(r1$X@x, r2$X@x)

    expect_equal(r1$X@p, r3$X@p)
    expect_equal(r1$X@j, r3$X@j)
    expect_equal(r1$X@x, r3$X@x)

    expect_equal(r1$X@p, r4$X@p)
    expect_equal(r1$X@j, r4$X@j)
    expect_equal(r1$X@x, r4$X@x)

    expect_equal(r1$X@p, c(0L, 2L, 3L, 4L))
    expect_equal(r1$X@j, c(3L-1L, 5L-1L, 7L-1L, 2L-1L))
    expect_equal(r1$X@x, c(0.54, 4.2, 3, 0.38))
})

test_that("Limiting nrows", {
    skip_if(skip_tests)
    txt_mat <- paste(
        "1 1:10 4:4.500000000",
        "0 ",
        "2 1:.001 2:5e-3",
        sep="\n"
    )
    check.two.rows <- function(r) {
        expect_equal(nrow(r$X), 2L)
        expect_equal(NROW(r$y), 2L)
    }
    
    r <- read.sparse(txt_mat, from_string=TRUE, limit_nrows=2)
    check.two.rows(r)
    r <- read.sparse(txt_mat, from_string=TRUE, limit_nrows=2, multilabel=TRUE)
    check.two.rows(r)
    
    
    file_name <- file.path(tempdir(), "test_sparse_matrix.txt")
    writeLines(txt_mat, con = file_name, sep = "")
    r <- read.sparse(file_name, from_string=FALSE, limit_nrows=2)
    check.two.rows(r)
    r <- read.sparse(file_name, from_string=FALSE, limit_nrows=2, multilabel=TRUE)
    check.two.rows(r)
})

test_that("With comments", {
    skip_if(skip_tests)
    txt_mat <- paste(
        "-1.234 1:10 4:4.500000000 #",
        "0 #1:1",
        "1e3 1:.001 2:5e-3 # 4:1",
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

test_that("Error on non-existent file", {
    skip_if(skip_tests)
    tentative_file_name <- "qwerty.invalid"
    while (TRUE) {
        file_name <- file.path(tempdir(), tentative_file_name)
        if (file.exists(file_name)) {
            file_name <- paste0(file_name, as.character(sample.int(.Machine$integer.max, size=1)))
        } else {
            break
        }
    }

    expect_error(read.sparse(file_name))
})
