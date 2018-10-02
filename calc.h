#ifndef CALC_H
#define CALC_H

#include "reader.h"
#include "date.h"
#include "plot.h"
#include "utility.h"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

class Calc
{
    std::vector<double> arr;
    std::vector<date> date_arr;
    double hist_d;
    std::ostringstream buffer;
    date step;

public:
    Calc();

    Calc(Reader& src);

    Calc(const Matrix& m);

    Calc(const Calc& src) = default;

    Calc& operator=(const Calc& src) = default;

    void SetReader(const Reader &src);

    void AddReader(const Reader& src);

    void SortByDate();

    void Clear();

    void setStep(const date& stp);

    date getStep() const;

    std::string GetData(const date& begin, const date& end) const;

    std::string GetData(size_t begin, size_t end) const;

    Matrix asVector() const;

    Matrix asVector(const date& begin, const date& end) const;

    Matrix asVector(size_t begin, size_t end) const;

    Matrix asMatrix() const;

    size_t size() const;

    double ExpectedValue(const date& begin, const date& end) const;

    double ExpectedValue(size_t begin, size_t end) const;

    double Dispersion(const date& begin, const date& end) const;

    double Dispersion(size_t begin, size_t end) const;

    double CKO(const date& begin, const date& end) const;

    double CKO(size_t begin, size_t end) const;

    Matrix Distance(const date& begin, const date& end) const;

    Matrix Distance(size_t begin, size_t end) const;

    double Moment(const date& begin, const date& end, size_t k) const;

    double Moment(size_t begin, size_t end, size_t k) const;

    double Fi(double u) const;

    size_t IntervalCount(size_t n) const;

    void MakeInterval(const date& begin, const date& end);

    void MakeInterval(size_t begin, size_t end); //Not good, but works

    double tgr(double eps, size_t n) const;

    size_t RemoveMisses(const date& begin, const date& end);

    size_t RemoveMisses(size_t begin, size_t end);

    double KSharle(size_t n) const;

    size_t RemoveMissesBySharle(size_t begin, size_t end);

    Matrix GenerateDistribution(const date& begin, const date& end);

    Matrix GenerateDistribution(size_t begin, size_t end);

    Matrix GenerateNormalDistribution(const date& begin, const date& end);

    Matrix GenerateNormalDistribution(size_t begin, size_t end);

    Matrix DistrToHist(Matrix dist); //Not good, but works

    double Normalize(const date& begin, const date& end);

    double Normalize(size_t begin, size_t end);

    void NormalizeNormalDist(const date& begin, const date& end);

    void NormalizeNormalDist(size_t begin, size_t end); //Works except the case with numbers with different signs

    bool Normalized(const date& begin, const date& end) const;

    bool Normalized(size_t begin, size_t end) const;

    bool NormalDistNormalized(const date& begin, const date& end, double eps = 1.0E-6) const;

    bool NormalDistNormalized(size_t begin, size_t end, double eps = 1.0E-6) const;

    std::string CheckNormalDist(const date& begin, const date& end, size_t normalize_norm_dist = 1);

    std::string CheckNormalDist(size_t begin, size_t end, size_t normalize_norm_dist = 1);

    double KohrenCheck(const date& begin, const date& end, size_t m);

    double KohrenCheck(size_t begin, size_t end, size_t m);

    std::string GetBuffer();

    const date& BeginDate() const;

    const date& EndDate() const;

    size_t FindDate(const date& src) const;

    size_t FindMonth(size_t m) const;

    size_t FindWeek(size_t w) const;

    size_t FindDay(size_t d) const;

    size_t FindNext(const date& src) const;

    size_t NearestMonth(size_t begin) const;

    size_t NearestWeek(size_t begin) const;

    size_t NearestDay(size_t begin) const;

    size_t NearestHalfDay(size_t begin) const;

    size_t NearestHour(size_t begin) const;

    size_t NearestHalfHour(size_t begin) const;

    size_t DemoFullFill(size_t begin, size_t end, size_t m, size_t N);

    void FullFillData();
};

#endif // CALC_H

