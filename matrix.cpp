#include "matrix.h"

bool isEqualSizes(const Matrix& m1, const Matrix& m2)
{
    return m1.rows() == m2.rows() && m1.columns() == m2.columns();
}

bool operator==(const Matrix& m1, const Matrix& m2)
{
    if (!isEqualSizes(m1, m2))
        return 0;
    for (size_t i = 0; i < m1.rows(); ++i)
    {
        for (size_t j = 0; j < m1.columns(); ++j)
        {
            if (m1[i][j] != m2[i][j])
                return 0;
        }
    }
    return 1;
}

bool operator!=(const Matrix& m1, const Matrix& m2)
{
    return !(m1 == m2);
}

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
    if (!isEqualSizes(m1, m2))
        ErrorHandler("Спроба додати матриці різних розмірностей.", ErrorHandler::exception);
    Matrix res = m1;
    for (size_t i = 0; i < m1.rows(); ++i)
    {
        for (size_t j = 0; j < m1.columns(); ++j)
        {
            res[i][j] += m2[i][j];
        }
    }
    return res;
}

Matrix operator-(const Matrix& m1, const Matrix& m2)
{
    if (!isEqualSizes(m1, m2))
        ErrorHandler("Спроба відняти матриці різних розмірностей.", ErrorHandler::exception);
    Matrix res = m1;
    for (size_t i = 0; i < m1.rows(); ++i)
    {
        for (size_t j = 0; j < m1.columns(); ++j)
        {
            res[i][j] -= m2[i][j];
        }
    }
    return res;
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
    if (m1.columns() != m2.rows())
        ErrorHandler("Спроба перемножити матриці різних розмірностей.", ErrorHandler::exception);
    Matrix res(m1.rows(), m2.columns());
    for (size_t i = 0; i < m1.rows(); ++i)
    {
        for (size_t j = 0; j < m2.columns(); ++j)
        {
            for (size_t k = 0; k < m1.columns(); ++k)
            {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return res;
}

Matrix operator+(const Matrix& m, double val)
{
    Matrix res = m;
    for (size_t i = 0; i < m.rows(); ++i)
    {
        for (size_t j = 0; j < m.columns(); ++j)
        {
            res[i][j] += val;
        }
    }
    return res;
}

Matrix operator-(const Matrix& m, double val)
{
    Matrix res = m;
    for (size_t i = 0; i < m.rows(); ++i)
    {
        for (size_t j = 0; j < m.columns(); ++j)
        {
            res[i][j] -= val;
        }
    }
    return res;
}

Matrix operator*(const Matrix& m, double val)
{
    Matrix res = m;
    for (size_t i = 0; i < m.rows(); ++i)
    {
        for (size_t j = 0; j < m.columns(); ++j)
        {
            res[i][j] *= val;
        }
    }
    return res;
}

Matrix SubMatrix::operator()(size_t lc, size_t rc) const
{
    Matrix res(rr - lr + 1, rc - lc + 1);
    for (size_t i = lr, res_i = 0; i <= rr; ++i, ++res_i)
    {
        for (size_t j = lc, res_j = 0; j <= rc; ++j, ++res_j)
        {
            res[res_i][res_j] = mx[i][j];
        }
    }
    return res;
}

Matrix::Matrix(size_t row, size_t col)
{
    setRows(row);
    setColumns(col);
}

Matrix::Matrix(const std::initializer_list<std::vector<double>>& lst)
{
    setRows(lst.end() - lst.begin());
    setColumns(lst.begin()->size());
    for (auto it = lst.begin(); it != lst.end(); ++it)
    {
        mtrx[it - lst.begin()] = *it;
    }
}

Matrix::Matrix(const std::initializer_list<double>& lst)
{
    setRows(1);
    setColumns(lst.end() - lst.begin());
    for (auto it = lst.begin(); it != lst.end(); ++it)
    {
        mtrx[0][it - lst.begin()] = *it;
    }
}

Matrix::Matrix(const std::vector<double>& v)
{
    setRows(1);
    setColumns(v.size());
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        mtrx[0][it - v.begin()] = *it;
    }
}

Matrix& Matrix::operator=(const Matrix& m)
{
    if (this == &m) return *this;
    setRows(m.rows());
    setColumns(m.columns());
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < columns(); ++j)
        {
            mtrx[i][j] = m[i][j];
        }
    }
    return *this;
}

std::vector<double> Matrix::Vector() const
{
    Matrix rowVect = rowVector();
    std::vector<double> res;
    for (size_t i = 0; i < rowVect.columns(); ++i)
    {
        res.push_back(rowVect[0][i]);
    }
    return res;
}

size_t Matrix::size() const
{
    if (rows() != columns())
    {
        ErrorHandler("Спроба дізнатися розмір не квадратної матриці.");
    }
    return rows();
}

void Matrix::setRows(size_t r)
{
    rows_ = r;
    mtrx.resize(rows_);
}

void Matrix::setColumns(size_t c)
{
    columns_ = c;
    for (size_t i = 0; i < rows(); ++i)
    {
        mtrx[i].resize(columns_);
    }
}

void Matrix::fill(double val)
{
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < columns(); ++j)
        {
            mtrx[i][j] = val;
        }
    }
}

std::vector<double>& Matrix::operator[](size_t i)
{
    if (i >= rows())
    {
        ErrorHandler("i >= row.");
    }
    return mtrx[i];
}

const std::vector<double>& Matrix::operator[](size_t i) const
{
    if (i >= rows())
    {
        ErrorHandler("Вихід за межі межі матриці.", ErrorHandler::exception);
    }
    return mtrx[i];
}

SubMatrix Matrix::operator()(size_t l, size_t r) const
{
    return SubMatrix(*this, l, r);
}

double Matrix::minor(size_t row, size_t col) const
{
    Matrix res(rows() - 1, columns() - 1);
    for (size_t i = 0, res_i = 0; i < rows(); ++i)
    {
        if (i == row)
        {
            continue;
        }
        for (size_t j = 0, res_j = 0; j < columns(); ++j)
        {
            if (j == col)
            {
                continue;
            }
            res[res_i][res_j] = mtrx[i][j];
            ++res_j;
        }
        ++res_i;
    }
    return res.det();
}

double Matrix::algeb(size_t row, size_t col) const
{
    return pow(-1, row + col) * minor(row, col);
}

double Matrix::det() const
{
    if (!isSquare())
    {
        ErrorHandler("Спроба знайти визначник не квадратної матриці.", ErrorHandler::exception);
    }
    if (rows() == 0)
        return 0;
    if (rows() == 1)
        return mtrx[0][0];
    if (rows() == 2)
        return mtrx[0][0] * mtrx[1][1] - mtrx[0][1] * mtrx[1][0];
    double res = 0.0;
    for (size_t i = 0; i < columns(); ++i)
    {
        res += mtrx[0][i] * algeb(0, i);
    }
    return res;
}

Matrix& Matrix::operator+=(Matrix& m)
{
    *this = *this + m;
    return *this;
}

Matrix& Matrix::operator-=(Matrix& m)
{
    *this = *this - m;
    return *this;
}

Matrix Matrix::transpose() const
{
    Matrix res(columns(), rows());
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < columns(); ++j)
        {
            res[j][i] = mtrx[i][j];
        }
    }
    return res;
}

Matrix Matrix::inverse() const
{
    return adjugate() * (1 / det());
}

Matrix Matrix::adjugate() const
{
    Matrix res(columns(), rows());
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < columns(); ++j)
        {
            res[j][i] = algeb(i, j);
        }
    }
    return res;
}

Matrix Matrix::solve(Matrix B) const
{
    if (B.isRowVector())
        B = B.columnVector();
    return inverse() * B;
}

Matrix Matrix::rowVector() const
{
    if (!isRowVector() && !isColumnVector())
        return operator()(0, 0)(0, columns() - 1);
    if (isRowVector())
        return *this;
    //isColumnVector
    return operator()(0, rows() - 1)(0, 0).transpose();
}

Matrix Matrix::columnVector() const
{
    if (isColumnVector())
        return *this;
    else if (isRowVector() == 0)
        return operator()(0, rows() - 1)(0, 0);
    //isRowVector
    return operator()(0, 0)(0, columns() - 1).transpose();
}

bool Matrix::isIdentity() const
{
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < columns(); ++j)
        {
            if ((i == j && mtrx[i][j] != 1) || (i != j && mtrx[i][j] != 0))
                return 0;
        }
    }
    return 1;
}

bool Matrix::isZero() const
{
    for (size_t i = 0; i < rows(); ++i)
    {
        for (size_t j = 0; j < columns(); ++j)
        {
            if (mtrx[i][j]) return 0;
        }
    }
    return 1;
}

bool Matrix::isRowVector() const
{
    return rows() == 1;
}

bool Matrix::isColumnVector() const
{
    return columns() == 1;
}

bool Matrix::isEmpty() const
{
    return rows() == 0 && columns() == 0;
}
