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

#ifdef _FOR_R

#include <Rcpp.h>
#include <Rcpp/unwindProtect.h>
#include "readsparse.hpp"
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::plugins(unwindProtect)]]

extern "C" {
    FILE *RC_fopen(const SEXP fn, const char *mode, const Rboolean expand);
}


SEXP convert_IntVecToRcpp(void *data)
{
    return Rcpp::IntegerVector(((std::vector<int>*)data)->begin(),
                               ((std::vector<int>*)data)->end());
}

SEXP convert_NumVecToRcpp(void *data)
{
    return Rcpp::NumericVector(((std::vector<double>*)data)->begin(),
                               ((std::vector<double>*)data)->end());
}

SEXP convert_StringStreamToRcpp(void *data)
{
    return Rcpp::wrap<Rcpp::CharacterVector>(((std::stringstream*)data)->str());
}

// [[Rcpp::export(rng = false)]]
Rcpp::List read_multi_label_R
(
    Rcpp::CharacterVector fname,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["nrows"] = Rcpp::IntegerVector(1),
        Rcpp::_["ncols"] = Rcpp::IntegerVector(1),
        Rcpp::_["nclasses"] = Rcpp::IntegerVector(1),
        Rcpp::_["values"] = R_NilValue,
        Rcpp::_["indptr"] = R_NilValue,
        Rcpp::_["indices"] = R_NilValue,
        Rcpp::_["indptr_lab"] = R_NilValue,
        Rcpp::_["indices_lab"] = R_NilValue,
        Rcpp::_["qid"] = R_NilValue
    );

    std::vector<int> indptr, indices, indptr_lab, indices_lab;
    std::vector<double> values;
    std::vector<int> qid;
    size_large nrows, ncols, nclasses;

    FILE *input_file = RC_fopen(fname[0], "r", TRUE);
    if (input_file == NULL)
    {
        REprintf("Error %d: %s\n", errno, strerror(errno));
        return Rcpp::List();
    }
    bool succeeded = read_multi_label(
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
        assume_no_qid
    );
    if (input_file != NULL) fclose(input_file);

    if (!succeeded)
        return Rcpp::List();

    if (nrows >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(1));
    else if (ncols >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(2));
    else if (nclasses >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(3));

    INTEGER(out["nrows"])[0] = (int)nrows;
    INTEGER(out["ncols"])[0] = (int)ncols;
    INTEGER(out["nclasses"])[0] = (int)nclasses;
    out["values"] = Rcpp::unwindProtect(convert_NumVecToRcpp, (void*)&values);

    values.clear();
    out["indptr"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indptr);
    indptr.clear();
    out["indices"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indices);
    indices.clear();
    out["indptr_lab"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indptr_lab);
    indptr_lab.clear();
    out["indices_lab"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indices_lab);
    indices_lab.clear();
    out["qid"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&qid);
    qid.clear();
    return out;
}

// [[Rcpp::export(rng = false)]]
Rcpp::List read_multi_label_from_str_R
(
    Rcpp::CharacterVector file_as_str,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["nrows"] = Rcpp::IntegerVector(1),
        Rcpp::_["ncols"] = Rcpp::IntegerVector(1),
        Rcpp::_["nclasses"] = Rcpp::IntegerVector(1),
        Rcpp::_["values"] = R_NilValue,
        Rcpp::_["indptr"] = R_NilValue,
        Rcpp::_["indices"] = R_NilValue,
        Rcpp::_["indptr_lab"] = R_NilValue,
        Rcpp::_["indices_lab"] = R_NilValue,
        Rcpp::_["qid"] = R_NilValue
    );

    std::string file_as_str_cpp = Rcpp::as<std::string>(file_as_str);
    std::stringstream ss;
    ss.str(file_as_str_cpp);

    std::vector<int> indptr, indices, indptr_lab, indices_lab;
    std::vector<double> values;
    std::vector<int> qid;
    size_large nrows, ncols, nclasses;

    bool succeeded = read_multi_label(
        ss,
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
        assume_no_qid
    );
    if (!succeeded)
        return Rcpp::List();

    if (nrows >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(1));
    else if (ncols >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(2));
    else if (nclasses >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(3));

    INTEGER(out["nrows"])[0] = (int)nrows;
    INTEGER(out["ncols"])[0] = (int)ncols;
    INTEGER(out["nclasses"])[0] = (int)nclasses;
    out["values"] = Rcpp::unwindProtect(convert_NumVecToRcpp, (void*)&values);

    values.clear();
    out["indptr"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indptr);
    indptr.clear();
    out["indices"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indices);
    indices.clear();
    out["indptr_lab"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indptr_lab);
    indptr_lab.clear();
    out["indices_lab"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indices_lab);
    indices_lab.clear();
    out["qid"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&qid);
    qid.clear();
    return out;
}

// [[Rcpp::export(rng = false)]]
Rcpp::List read_single_label_R
(
    Rcpp::CharacterVector fname,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["nrows"] = Rcpp::IntegerVector(1),
        Rcpp::_["ncols"] = Rcpp::IntegerVector(1),
        Rcpp::_["nclasses"] = Rcpp::IntegerVector(1),
        Rcpp::_["values"] = R_NilValue,
        Rcpp::_["indptr"] = R_NilValue,
        Rcpp::_["indices"] = R_NilValue,
        Rcpp::_["labels"] = R_NilValue,
        Rcpp::_["qid"] = R_NilValue
    );

    std::vector<int> indptr, indices;
    std::vector<double> values, labels;
    std::vector<int> qid;
    size_large nrows, ncols, nclasses;

    FILE *input_file = RC_fopen(fname[0], "r", TRUE);
    if (input_file == NULL)
    {
        REprintf("Error %d: %s\n", errno, strerror(errno));
        return Rcpp::List();
    }
    bool succeeded = read_single_label(
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
        assume_no_qid
    );
    if (input_file != NULL) fclose(input_file);
    
    if (!succeeded)
        return Rcpp::List();


    if (nrows >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(1));
    else if (ncols >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(2));
    else if (nclasses >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(3));

    INTEGER(out["nrows"])[0] = (int)nrows;
    INTEGER(out["ncols"])[0] = (int)ncols;
    INTEGER(out["nclasses"])[0] = (int)nclasses;
    out["values"] = Rcpp::unwindProtect(convert_NumVecToRcpp, (void*)&values);

    values.clear();
    out["indptr"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indptr);
    indptr.clear();
    out["indices"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indices);
    indices.clear();
    out["labels"] = Rcpp::unwindProtect(convert_NumVecToRcpp, (void*)&labels);
    labels.clear();
    out["qid"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&qid);
    qid.clear();
    return out;
}

// [[Rcpp::export(rng = false)]]
Rcpp::List read_single_label_from_str_R
(
    Rcpp::CharacterVector file_as_str,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool assume_no_qid
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["nrows"] = Rcpp::IntegerVector(1),
        Rcpp::_["ncols"] = Rcpp::IntegerVector(1),
        Rcpp::_["nclasses"] = Rcpp::IntegerVector(1),
        Rcpp::_["values"] = R_NilValue,
        Rcpp::_["indptr"] = R_NilValue,
        Rcpp::_["indices"] = R_NilValue,
        Rcpp::_["labels"] = R_NilValue,
        Rcpp::_["qid"] = R_NilValue
    );

    std::string file_as_str_cpp = Rcpp::as<std::string>(file_as_str);
    std::stringstream ss;
    ss.str(file_as_str_cpp);

    std::vector<int> indptr, indices;
    std::vector<double> values, labels;
    std::vector<int> qid;
    size_large nrows, ncols, nclasses;

    bool succeeded = read_single_label(
        ss,
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
        assume_no_qid
    );
    if (!succeeded)
        return Rcpp::List();

    if (nrows >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(1));
    else if (ncols >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(2));
    else if (nclasses >= INT_MAX - 1)
        return Rcpp::List::create(Rcpp::_["err"] = Rcpp::wrap(3));

    INTEGER(out["nrows"])[0] = (int)nrows;
    INTEGER(out["ncols"])[0] = (int)ncols;
    INTEGER(out["nclasses"])[0] = (int)nclasses;
    out["values"] = Rcpp::unwindProtect(convert_NumVecToRcpp, (void*)&values);

    values.clear();
    out["indptr"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indptr);
    indptr.clear();
    out["indices"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&indices);
    indices.clear();
    out["labels"] = Rcpp::unwindProtect(convert_NumVecToRcpp, (void*)&labels);
    labels.clear();
    out["qid"] = Rcpp::unwindProtect(convert_IntVecToRcpp, (void*)&qid);
    qid.clear();
    return out;
}

// [[Rcpp::export(rng = false)]]
bool write_multi_label_R
(
    Rcpp::CharacterVector fname,
    Rcpp::IntegerVector indptr,
    Rcpp::IntegerVector indices,
    Rcpp::NumericVector values,
    Rcpp::IntegerVector indptr_lab,
    Rcpp::IntegerVector indices_lab,
    Rcpp::IntegerVector qid,
    const int ncols,
    const int nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places,
    const bool append
)
{
    FILE *output_file = RC_fopen(fname[0], append? "a" : "w", TRUE);
    if (output_file == NULL)
    {
        REprintf("Error %d: %s\n", errno, strerror(errno));
        return false;
    }
    bool succeeded = write_multi_label(
        output_file,
        INTEGER(indptr),
        INTEGER(indices),
        REAL(values),
        INTEGER(indptr_lab),
        INTEGER(indices_lab),
        INTEGER(qid),
        NA_INTEGER,
        qid.size() > 0,
        indptr.size() - 1,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (output_file != NULL) fclose(output_file);
    return succeeded;
}


// [[Rcpp::export(rng = false)]]
Rcpp::List write_multi_label_to_str_R
(
    Rcpp::IntegerVector indptr,
    Rcpp::IntegerVector indices,
    Rcpp::NumericVector values,
    Rcpp::IntegerVector indptr_lab,
    Rcpp::IntegerVector indices_lab,
    Rcpp::IntegerVector qid,
    const int ncols,
    const int nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["str"] = R_NilValue
    );
    std::stringstream ss;
    bool succeeded = write_multi_label(
        ss,
        INTEGER(indptr),
        INTEGER(indices),
        REAL(values),
        INTEGER(indptr_lab),
        INTEGER(indices_lab),
        INTEGER(qid),
        NA_INTEGER,
        qid.size() > 0,
        indptr.size() - 1,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (!succeeded)
        return Rcpp::List();
    
    out["str"] = Rcpp::unwindProtect(convert_StringStreamToRcpp, (void*)&ss);
    return out;
}

// [[Rcpp::export(rng = false)]]
bool write_single_label_numeric_R
(
    Rcpp::CharacterVector fname,
    Rcpp::IntegerVector indptr,
    Rcpp::IntegerVector indices,
    Rcpp::NumericVector values,
    Rcpp::NumericVector labels,
    Rcpp::IntegerVector qid,
    const int ncols,
    const int nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places,
    const bool append
)
{
    FILE *output_file = RC_fopen(fname[0], append? "a" : "w", TRUE);
    if (output_file == NULL)
    {
        REprintf("Error %d: %s\n", errno, strerror(errno));
        return false;
    }
    bool succeeded = write_single_label(
        output_file,
        INTEGER(indptr),
        INTEGER(indices),
        REAL(values),
        REAL(labels),
        INTEGER(qid),
        NA_INTEGER,
        NA_REAL,
        qid.size() > 0,
        indptr.size()-1,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (output_file != NULL) fclose(output_file);
    return succeeded;
}

// [[Rcpp::export(rng = false)]]
bool write_single_label_integer_R
(
    Rcpp::CharacterVector fname,
    Rcpp::IntegerVector indptr,
    Rcpp::IntegerVector indices,
    Rcpp::NumericVector values,
    Rcpp::IntegerVector labels,
    Rcpp::IntegerVector qid,
    const int ncols,
    const int nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places,
    const bool append
)
{
    FILE *output_file = RC_fopen(fname[0], append? "a" : "w", TRUE);
    if (output_file == NULL)
    {
        REprintf("Error %d: %s\n", errno, strerror(errno));
        return false;
    }
    bool succeeded = write_single_label(
        output_file,
        INTEGER(indptr),
        INTEGER(indices),
        REAL(values),
        INTEGER(labels),
        INTEGER(qid),
        NA_INTEGER,
        NA_INTEGER,
        qid.size() > 0,
        indptr.size()-1,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (output_file != NULL) fclose(output_file);
    return succeeded;
}

// [[Rcpp::export(rng = false)]]
Rcpp::List write_single_label_numeric_to_str_R
(
    Rcpp::IntegerVector indptr,
    Rcpp::IntegerVector indices,
    Rcpp::NumericVector values,
    Rcpp::NumericVector labels,
    Rcpp::IntegerVector qid,
    const int ncols,
    const int nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["str"] = R_NilValue
    );

    std::stringstream ss;
    bool succeeded = write_single_label(
        ss,
        INTEGER(indptr),
        INTEGER(indices),
        REAL(values),
        REAL(labels),
        INTEGER(qid),
        NA_INTEGER,
        NA_REAL,
        qid.size() > 0,
        indptr.size()-1,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (!succeeded)
        return Rcpp::List();

    out["str"] = Rcpp::unwindProtect(convert_StringStreamToRcpp, (void*)&ss);
    return out;
}

// [[Rcpp::export(rng = false)]]
Rcpp::List write_single_label_integer_to_str_R
(
    Rcpp::IntegerVector indptr,
    Rcpp::IntegerVector indices,
    Rcpp::NumericVector values,
    Rcpp::IntegerVector labels,
    Rcpp::IntegerVector qid,
    const int ncols,
    const int nclasses,
    const bool ignore_zero_valued,
    const bool sort_indices,
    const bool text_is_base1,
    const bool add_header,
    const int decimal_places
)
{
    Rcpp::List out = Rcpp::List::create(
        Rcpp::_["str"] = R_NilValue
    );

    std::stringstream ss;
    bool succeeded = write_single_label(
        ss,
        INTEGER(indptr),
        INTEGER(indices),
        REAL(values),
        INTEGER(labels),
        INTEGER(qid),
        NA_INTEGER,
        NA_INTEGER,
        qid.size() > 0,
        indptr.size()-1,
        ncols,
        nclasses,
        ignore_zero_valued,
        sort_indices,
        text_is_base1,
        add_header,
        decimal_places
    );
    if (!succeeded)
        return Rcpp::List();

    out["str"] = Rcpp::unwindProtect(convert_StringStreamToRcpp, (void*)&ss);
    return out;
}

#endif /* _FOR_R */
