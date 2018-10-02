#include "regression.h"

void Regression::addX(const Matrix& m)
{
    if (Y.columns() > 1)
        ErrorHandler("Параметр функції addX має бути вектор-стовпцем.", ErrorHandler::exception);
    size_t pos = 0;
    while (pos < m.columns())
    {
        ++pos;
    }
    for (size_t i = 0; i < m.rows(); ++i)
    {
        X[i][pos] = m[i][0];
    }
}

void Regression::setX(const Matrix& m)
{
    X = m;
}

void Regression::setY(const Matrix& m)
{
    Y = m;
    if (Y.columns() > 1)
        Y = Y.columnVector();
}

Matrix Regression::findRegression(RegressionType rt)
{
    type = rt;
    switch(type)
    {
        case Linear:
        {
            return LinearRegression();
        }
        default:
        {
            ErrorHandler("Невідомий тип регресії.", ErrorHandler::exception);
            return Matrix();
        }
    }
}

Matrix Regression::LinearRegression()
{
    type = Linear;
    Matrix res;
    res = (X.transpose() * X).inverse() * (X.transpose()) * Y;
    return res;
}

double Regression::PredictLinear(const Matrix& m)
{
    return (LinearRegression().rowVector() * m.columnVector()).det();
}

double Regression::PredictFor(const Matrix& m)
{
    switch(type)
    {
        case Linear:
            return PredictLinear(m);
        default:
            ErrorHandler("Невідомий тип регресії.", ErrorHandler::exception);
            return 0.0;
    }
}

