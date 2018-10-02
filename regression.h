#ifndef REGRESSION_H
#define REGRESSION_H

#include "matrix.h"
#include "errorhandler.h"
#include <vector>

class Regression
{
public:
    enum RegressionType{Linear, Other};

public:
    Regression(): type(Linear) {}
    void addX(const Matrix& m);
    void setX(const Matrix& m);
    void setY(const Matrix& m);
    Matrix findRegression(RegressionType rt);
    Matrix LinearRegression();
    RegressionType getRegressionType() const { return type; }
    double PredictFor(const Matrix& m);

private:
    double PredictLinear(const Matrix& m);

private:
    Matrix X;
    Matrix Y;
    std::vector<double> coeff;
    RegressionType type;
};

#endif // REGRESSION_H

