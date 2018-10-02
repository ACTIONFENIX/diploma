#ifndef DATE_H
#define DATE_H

#include <string>

class date
{
private:
    int month;
    int day;
    int hour;
    int minute;
public:
    date(): month(0), day(0), hour(0), minute(0) {}

    date(const date& src): month(src.month), day(src.day), hour(src.hour), minute(src.minute) {}

    date(int m, int d, const std::string& str);

    date(const std::string& src);

    date& operator=(const date& src);

    void setMonth(int m);

    const int& Month() const;

    void setDay(int d);

    const int& Day() const;

    void setHour(int h);

    const int& Hour() const;

    const int& Minute() const;

    void setMinute(int m);

    bool isValid() const;

    std::string Date() const;

    std::string Time() const;

    std::string asStr() const;

    friend date operator+(const date& src1, const date& src2);

    friend date operator-(const date& src1, const date& src2);

    date& operator+=(const date& src);

    date& operator-=(const date& src);

private:
    char DigitToChar(int d) const;
};

bool operator==(const date& src1, const date& src2);

bool operator!=(const date& src1, const date& src2);

bool operator<(const date& src1, const date& src2);

bool operator<=(const date& src1, const date& src2);

bool operator>(const date& src1, const date& src2);

bool operator>=(const date& src1, const date& src2);

date operator+(const date& src1, const date& src2);

date operator-(const date& src1, const date& src2);

size_t steps(const date& d);

size_t steps(const date& d, const date& step);

const date Month(1, 0, "00:00");
const date Day(0, 1, "00:00");
const date Hour(0, 0, "01:00");
const date HalfHour(0, 0, "00:30");
const date Minute(0, 0, "00:01");

#endif // DATE_H

