#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <fstream>
#include "matrix.h"
#include "date.h"

void saveMatrix(const Matrix& m, const std::string& fullname);

bool isInteger(double i);

double dateID(const date& src);

date restoreFromID(int id);

template<typename... Args>
Matrix fromMatrixx(Args... args);

#endif // UTILITY_H

