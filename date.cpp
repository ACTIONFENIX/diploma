#include "date.h"
#include "errorhandler.h"
#include <stdexcept>
#include <cstdlib>

date::date(int m, int d, const std::string& str)
{
    month = m;
    day = d;
    hour = atoi(str.substr(0, str.find(":")).c_str());
    minute = atoi(str.substr(str.find(":") + 1).c_str());
}

date::date(const std::string& src)
{
    int     i;

    i = 0;
    day = atoi(src.c_str());
    while (src[i] != '.')
        ++i;
    ++i;
    month = atoi(src.c_str() + i);
    while (src[i] != ' ')
        ++i;
    ++i;

    hour = atoi(src.c_str() + i);
    while (src[i] != ':')
        ++i;
    ++i;
    minute = atoi(src.c_str() + i);
}

date& date::operator=(const date& src)
{
    month = src.month;
    day = src.day;
    hour = src.hour;
    minute = src.minute;
}

void date::setMonth(int m)
{
    month = m;
}

const int& date::Month() const
{
    return month;
}

void date::setDay(int d)
{
    day = d;
}

const int& date::Day() const
{
    return day;
}

void date::setHour(int h)
{
    hour = h;
}

const int& date::Hour() const
{
    return hour;
}

const int& date::Minute() const
{
    return minute;
}

void date::setMinute(int m)
{
    minute = m;
}

bool date::isValid() const
{
    return month >= 1 && month <=12 && day >= 1 && day <= 31 && hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

std::string date::Date() const
{
    std::string result;

    if (day < 10)
        result += '0';
    result += std::to_string(day);
    result += '.';
    if (month < 10)
        result += '0';
    result += std::to_string(month);
    return result;
}

std::string date::Time() const
{
    std::string result;

    if (hour < 10)
        result += '0';
    result += std::to_string(hour);
    result += '.';
    if (minute < 10)
        result += '0';
    result += std::to_string(minute);
    return result;
}

std::string date::asStr() const
{
    return Date() + " " + Time();
}

date& date::operator+=(const date& src)
{
    *this = *this + src;
    return *this;
}

date& date::operator-=(const date& src)
{
    *this = *this - src;
    return *this;
}


char date::DigitToChar(int d) const
{
    return char(d + 48);
}

bool operator==(const date& src1, const date& src2)
{
    return src1.Month() == src2.Month() && src1.Day() == src2.Day() && src1.Hour() == src2.Hour() && src1.Minute() == src2.Minute();
}

bool operator!=(const date& src1, const date& src2)
{
    return !(src1 == src2);
}

bool operator<(const date& src1, const date& src2)
{
    if (src1.Month() < src2.Month())
    {
        return 1;
    }
    else if (src1.Month() == src2.Month() && src1.Day() < src2.Day())
    {
        return 1;
    }
    else if (src1.Month() == src2.Month() && src1.Day() == src2.Day() && src1.Hour() < src2.Hour())
    {
        return 1;
    }
    else if (src1.Month() == src2.Month() && src1.Day() == src2.Day() && src1.Hour() == src2.Hour() && src1.Minute() < src2.Minute())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool operator<=(const date& src1, const date& src2)
{
    return (src1 < src2) || (src1 == src2);
}

bool operator>(const date& src1, const date& src2)
{
    return !(src1 <= src2);
}

bool operator>=(const date& src1, const date& src2)
{
    return (src1 > src2) || (src1 == src2);
}

date operator+(const date& src1, const date& src2)
{
    date result;
    result.month = src1.month + src2.month;
    result.day = src1.day + src2.day;
    result.hour = src1.hour + src2.hour;
    result.minute = src1.minute + src2.minute;
    while (result.minute >= 60)
    {
        result.minute -= 60;
        ++result.hour;
    }
    while (result.hour >= 24)
    {
        result.hour -= 24;
        ++result.day;
    }
    while (result.day >= 31)
    {
        result.day -= 31;
        ++result.month;
    }
    if (result.month > 12)
    {
        ErrorHandler("При операціях отримано дату, що виходить за межі року.\nТакі дати не підтримуються.");
    }
    return result;
}

date operator-(const date& src1, const date& src2)
{
    if (src1 < src2) ErrorHandler("При операціях отримано від'ємну дату.", ErrorHandler::exception);
    date result;
    result.month = src1.month - src2.month;
    result.day = src1.day - src2.day;
    result.hour = src1.hour - src2.hour;
    result.minute = src1.minute - src2.minute;
    while (result.minute < 0)
    {
        result.minute += 60;
        --result.hour;
    }
    while (result.hour < 0)
    {
        result.hour += 24;
        --result.day;
    }
    while (result.day < 0)
    {
        result.day += 31;
        --result.month;
    }
    return result;
}

size_t steps(const date& d)
{
    return steps(d, HalfHour);
}

size_t steps(const date& d, const date& step)
{
    size_t res = 0;
    for (date i = date(0, 0, "00:00"); i <= d; i = i + step)
    {
        ++res;
    }
    return res - 1;
}

