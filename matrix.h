#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <vector>
#include <initializer_list>
#include "errorhandler.h"

class SubMatrix;

class Matrix
{
private:
    std::vector<std::vector<double>> mtrx;
    size_t rows_;
    size_t columns_;

public:
    Matrix(): rows_(0), columns_(0) { }
    Matrix(size_t row, size_t col);
    Matrix(const std::initializer_list<std::vector<double>>& lst);
    Matrix(const std::initializer_list<double>& lst);
    Matrix(const Matrix& m) { *this = m; }
    Matrix(const std::vector<double>& v);
    Matrix& operator=(const Matrix& m);
    std::vector<double> Vector() const;
    size_t rows() const { return rows_; }
    size_t columns() const { return columns_; }
    size_t size() const;
    void setRows(size_t r);
    void setColumns(size_t c);
    void fill(double val);
    const std::vector<std::vector<double>>& data() const { return mtrx; }
    std::vector<double>& operator[](size_t i);
    const std::vector<double>& operator[](size_t i) const;
    SubMatrix operator()(size_t l, size_t r) const;
    double minor(size_t row, size_t col) const;
    double algeb(size_t row, size_t col) const;
    double det() const;
    Matrix& operator+=(Matrix& m);
    Matrix& operator-=(Matrix& m);
    Matrix transpose() const;
    Matrix inverse() const;
    Matrix adjugate() const;
    Matrix solve(Matrix B) const;
    Matrix rowVector() const;
    Matrix columnVector() const;
    bool isSquare() const { return rows() == columns(); }
    bool isIdentity() const;
    bool isZero() const;
    bool isRowVector() const;
    bool isColumnVector() const;
    bool isEmpty() const;
};

class SubMatrix
{
    const Matrix& mx;
    size_t lr;
    size_t rr;
    friend class Matrix;

public:
    Matrix operator()(size_t lc, size_t rc) const;

private:
    SubMatrix(const Matrix& m, size_t l, size_t r): mx(m), lr(l), rr(r) {}
};

bool isEqualSizes(const Matrix& m1, const Matrix& m2);

bool operator==(const Matrix& m1, const Matrix& m2);

bool operator!=(const Matrix& m1, const Matrix& m2);

Matrix operator+(const Matrix& m1, const Matrix& m2);

Matrix operator-(const Matrix& m1, const Matrix& m2);

Matrix operator*(const Matrix& m1, const Matrix& m2);

Matrix operator+(const Matrix& m, double val);

Matrix operator-(const Matrix& m, double val);

Matrix operator*(const Matrix& m, double val);

#endif // MATRIX_H
