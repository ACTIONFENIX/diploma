#include "utility.h"

void saveMatrix(const Matrix &m, const std::string& fullname)
{
    std::ofstream file(fullname);
    for (size_t i = 0; i < m.rows(); ++i)
    {
        for (size_t j = 0; j < m.columns(); ++j)
        {
            file << m[i][j] << " ";
        }
        file << std::endl;
    }
    file.close();
}

bool isInteger(double i)
{
    return i == double(int(i));
}

double dateID(const date& src)
{
    return src.Minute() + src.Hour() * 100 + src.Day() * 10000 + src.Month() * 1000000;
}

date restoreFromID(int id)
{
    date dt;
    dt.setMonth(id / 1000000);
    id -= id / 1000000 * 1000000;
    dt.setDay(id / 10000);
    id -= id / 10000 * 10000;
    dt.setHour(id / 100);
    id -= id / 100 * 100;
    dt.setMinute(id);
    return dt;
}

template<typename... Args>
Matrix fromMatrixx(Args... args)
{

}

