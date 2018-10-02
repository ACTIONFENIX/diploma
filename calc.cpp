#include "calc.h"
#include "date.h"
#include "reader.h"
#include "errorhandler.h"
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <fstream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <QInputDialog>

Calc::Calc(): arr(1), date_arr(1), step(HalfHour)
{

}

Calc::Calc(Reader& src): Calc()
{
    SetReader(src);
}

Calc::Calc(const Matrix& m)
{
    arr = m.Vector();
}

void Calc::SetReader(const Reader& src)
{
    Clear();
    date_arr = src.GetDateArr();
    arr = src.GetArr();
    SortByDate();
}

void Calc::AddReader(const Reader& src)
{
    std::vector<date> add_datearr(src.GetDateArr());
    size_t saved_month = date_arr.back().Month();
    size_t saved_day = 0;
    for (size_t i = 0; i < add_datearr.size(); ++i)
    {
        add_datearr[i].setMonth(saved_month + 1);
        add_datearr[i].setDay(saved_day + 1);
    }
    date_arr.insert(date_arr.end(), add_datearr.begin(), add_datearr.end());
    arr.insert(arr.end(), src.GetArr().begin(), src.GetArr().end());
}

void Calc::SortByDate()
{
    size_t i;
    size_t j;
    bool sorted;

    i = 0;
    sorted = 0;
    while (i < arr.size() - 1)
    {
        j = i + 1;
        while (j < arr.size())
        {
            if (date_arr[i] > date_arr[j])
            {
                std::swap(date_arr[i], date_arr[j]);
                std::swap(arr[i], arr[j]);
                sorted = 1;
            }
            ++j;
        }
        ++i;
    }
    if (sorted)
        ErrorHandler("Дані були відсортовані згідно дати.");
}

void Calc::Clear()
{
    date_arr.clear();
    arr.clear();
}

void Calc::setStep(const date& stp)
{
    step = stp;
}

date Calc::getStep() const
{
    return step;
}

std::string Calc::GetData(const date& begin, const date& end) const
{
    return GetData(FindDate(begin), FindDate(end));
}

std::string Calc::GetData(size_t begin, size_t end) const
{
    std::ostringstream out;
    for (size_t i = begin; i < end; i += steps(step, HalfHour))
    {
        out << date_arr[i].Date() << " " << date_arr[i].Time() << " data = " << arr[i] << std::endl;
    }
    return out.str();
}

Matrix Calc::asVector() const
{
    Matrix res(1, size());
    for (size_t i = 0; i < size(); i += steps(step, HalfHour))
    {
        res[0][i] = arr[i];
    }
    return res;
}

Matrix Calc::asVector(const date& begin, const date& end) const
{
    return asVector(FindDate(begin), FindDate(end));
}


Matrix Calc::asVector(size_t begin, size_t end) const
{
    Matrix res(1, (end - begin) / steps(step, HalfHour));
    for (size_t i = begin; i < end; i += steps(step, HalfHour))
    {
        res[0][static_cast<size_t>((i - begin) / steps(step, HalfHour))] = arr[i];
    }
    return res;
}


Matrix Calc::asMatrix() const
{
    Matrix res(3, size());
    for (size_t i = 0; i < size(); i += steps(step, HalfHour))
    {
        res[0][i] = dateID(date_arr[i]);
    }
    for (size_t i = 0; i < size(); i += steps(step, HalfHour))
    {
        res[1][i] = arr[i];
    }
    return res;
}

size_t Calc::size() const
{
    return arr.size();
}

double Calc::ExpectedValue(const date& begin, const date& end) const
{
    return ExpectedValue(FindDate(begin), FindDate(end));
}

double Calc::ExpectedValue(size_t begin, size_t end) const
{
    double res = 0.0;
    if (begin >= arr.size()) ErrorHandler("ExpectedValue(): begin >= arr->size()", ErrorHandler::exception);
    for (size_t i = begin; i != end; ++i)
    {
        res += arr[i];
    }
    res /= end - begin;
    return res;
}

double Calc::Dispersion(const date& begin, const date& end) const
{
    return Dispersion(FindDate(begin), FindDate(end));
}

double Calc::Dispersion(size_t begin, size_t end) const
{
    if (end - begin == 1) return 0.0;
    if (begin >= arr.size()) ErrorHandler("Dispersion(): begin > arr->size()", ErrorHandler::exception);
    double res = 0.0;
    double exp_val = ExpectedValue(begin, end);
    for (size_t i = begin; i != end; ++i)
    {
        res += pow(arr[i] - exp_val, 2);
    }
    if (end - begin < 30)
    {
        res /= (end - begin - 1);
    }
    else
    {
        res /= (end - begin);
    }
    return res;
}

double Calc::CKO(const date& begin, const date& end) const
{
    return CKO(FindDate(begin), FindDate(end));
}

double Calc::CKO(size_t begin, size_t end) const
{
    return sqrt(Dispersion(begin, end));
}

Matrix Calc::Distance(const date& begin, const date& end) const
{
    return Distance(begin, end);
}

Matrix Calc::Distance(size_t begin, size_t end) const
{
    Matrix distance_arr(1, end - begin);
    double exp_val = ExpectedValue(begin, end);
    for (size_t i = begin; i != end; ++i)
    {
        distance_arr[0][i - begin] = arr[i] - exp_val;
    }
    return distance_arr;
}

double Calc::Moment(const date& begin, const date& end, size_t k) const
{
    return Moment(FindDate(begin), FindDate(end), k);
}

double Calc::Moment(size_t begin, size_t end, size_t k) const
{
    if (k == 1) return 0;
    if (k == 2) return Dispersion(begin, end);
    double res = 0.0;
    double exp_val = ExpectedValue(begin, end);
    for (size_t i = begin; i != end; ++i)
    {
        res += pow(arr[i] - exp_val, k);
    }
    res /= (end - begin);
    return res;
}

double Calc::Fi(double u) const
{
    return exp(- (u * u) / 2) / sqrt(2 * 3.14159);
}

size_t Calc::IntervalCount(size_t n) const
{
    double m_min = 0.55 * pow(n, 0.4);
    double m_max = 1.25 * pow(n, 0.4);
    int m = round((m_min + m_max) / 2);
    if (m < 5)
    {
        m = 5;
    }
    return m;
}

void Calc::MakeInterval(const date& begin, const date& end)
{
    MakeInterval(FindDate(begin), FindDate(end));
}

void Calc::MakeInterval(size_t begin, size_t end)
{
    size_t m = IntervalCount(end - begin);
    double real_min = *std::min_element(arr.begin() + begin, arr.begin() + end);
    double real_max = *std::max_element(arr.begin() + begin, arr.begin() + end);
    double exp_val = ExpectedValue(begin, end);
    //corrected
    double d = fabs((real_max - real_min) / m);
    hist_d = d;
    for (size_t i = begin; i != end; ++i)
    {
        for (double j = real_min - d / 2; j <= real_max + d / 2; j += d)
        {
            if (arr[i] >= j && arr[i] < j + d)
            {
                arr[i] = j + d / 2;
                arr[i] = static_cast<int>(arr[i] * 10000) / 10000.0;
                break;
            }
        }
    }
    /*double d;
    if (fabs(real_min - exp_val) > fabs(real_max - exp_val))
    {
        d = 2 * fabs(real_min - exp_val) / m;
    }
    else
    {
        d = 2 * fabs(real_max - exp_val) / m;
    }
    double min_distance = INFINITY;
    for (size_t i = begin + 1; i != end; ++i)
    {
        if ((arr[i] - arr[i - 1]) != 0 && (arr[i] - arr[i - 1]) < min_distance)
        {
            min_distance = (arr[i] - arr[i - 1]);
        }
    }
    if (d < min_distance)
    {
        d = min_distance; //во избежание пустых столбцов в гистограмме
    }
    d = fabs(d);
    d = (real_max - real_min) / m;
    for (size_t pos = begin; pos != end; ++pos)
    {
        for (double i = exp_val + d / 2, j = exp_val - d / 2; ; i += d, j -= d)
        {
            if (arr[pos] >= i && arr[pos] < i + d)
            {
                arr[pos] = i + d / 2;
                break;
            }
            if (arr[pos] >= j && arr[pos] < j + d)
            {
                arr[pos] = j + d / 2;
                break;
            }
        }
    }*/
    //shifted
    /*double d = fabs((real_max - real_min) / m);
    double min_distance = INFINITY;
    for (size_t i = begin + 1; i != end; ++i)
    {
        if ((arr[i] - arr[i - 1]) != 0 && fabs(arr[i] - arr[i - 1]) < min_distance)
        {
            min_distance = fabs(arr[i] - arr[i - 1]);
        }
    }
    if (d < min_distance)
    {
        d = min_distance; //во избежание пустых столбцов в гистограмме
    }
    _d = d;*/
    /*for (size_t i = begin; i != end; ++i)
    {
        for (double j = real_min; j <= real_max; j += d)
        {
            if ((*arr)[i] == real_max)
            {
                (*arr)[i] = real_min;
                for (int k = 0; k < m - 1; ++k)
                {
                    (*arr)[i] += d;
                    (*arr)[i] = static_cast<int>((*arr)[i] * 10000) / 10000.0;
                }
                break;
            }
            if ((*arr)[i] >= j && (*arr)[i] < j + d)
            {
                (*arr)[i] = j;
                (*arr)[i] = static_cast<int>((*arr)[i] * 10000) / 10000.0;
                break;
            }
        }
    }*/
}

double Calc::tgr(double eps, size_t n) const
{
    return 1.55 + 0.8 * sqrt(eps - 1) * log10(double(n) / 10);
}

size_t Calc::RemoveMisses(const date& begin, const date& end)
{
    return RemoveMisses(FindDate(begin), FindDate(end));
}

size_t Calc::RemoveMisses(size_t begin, size_t end)
{
    size_t removed = 0;
    double Xc = ExpectedValue(begin, end);
    double sigma = CKO(begin, end);
    double eps = Moment(begin, end, 4) / pow(sigma, 4);
    double Xgr = tgr(eps, end - begin) * CKO(begin, end);
    std::vector<bool> erase_map(end - begin);
    for (size_t i = begin; i != end; ++i)
    {
        if ((arr[i] <= (Xc - Xgr)) || (arr[i]) >= (Xc + Xgr))
        {
            erase_map[i - begin] = 1;
        }
    }
    for (int i = erase_map.size() - 1; i >= 0; --i)
    {
        if (erase_map[i])
        {
            ++removed;
            arr.erase(arr.begin() + i);
            date_arr.erase(date_arr.begin() + i);
        }
    }
    return removed;
}

double Calc::KSharle(size_t n) const
{
    if (n >= 5 && n < 10)
    {
        return 1.3 + (n - 5) * 0.07;
    }
    else if (n >= 10 && n < 20)
    {
        return 1.65 + (n - 10) * 0.031;
    }
    else if (n >= 20 && n < 30)
    {
        return 1.96 + (n - 20) * 0.017;
    }
    else if (n >= 30 && n < 40)
    {
        return 2.13 + (n - 30) * 0.011;
    }
    else if (n >= 40 && n < 50)
    {
        return 2.24 + (n - 40) * 0.008;
    }
    else if (n >= 50 && n < 100)
    {
        return 2.32 + (n - 50) * 0.026;
    }
    else if (n == 100)
    {
        return 2.58;
    }
    else
    {
        ErrorHandler("Недопустиме значення коефіцієнту Шарле.");
        return INFINITY;
    }
}

size_t Calc::RemoveMissesBySharle(size_t begin, size_t end)
{
    size_t removed = 0;
    double Xc = ExpectedValue(begin, end);
    double sigma = CKO(begin, end);
    std::vector<bool> erase_map(end - begin);
    for (size_t i = begin; i != end; ++i)
    {
        if (fabs(arr[i] - Xc) > KSharle(end - begin) * sigma)
        {
            erase_map[i - begin] = 1;
            ++removed;
        }
    }
    for (int i = erase_map.size() - 1; i >= 0; --i)
    {
        if (erase_map[i])
        {
            arr.erase(arr.begin() + i);
            date_arr.erase(date_arr.begin() + i);
        }
    }
    return removed;
}

Matrix Calc::GenerateDistribution(const date& begin, const date& end)
{
    return GenerateDistribution(FindDate(begin), FindDate(end));
}

Matrix Calc::GenerateDistribution(size_t begin, size_t end)
{
    std::vector<double> X_real; //real
    std::vector<double> Y_real; //real
    std::map<double, double> real_data;
    for (size_t i = begin; i != end; ++i)
    {
        if (real_data.find(arr[i]) == real_data.end())
        {
            real_data.insert(std::pair<double, double>(arr[i], 1));
        }
        else
        {
            real_data[arr[i]] += 1;
        }
    }
    for (auto it = real_data.begin(); it != real_data.end(); ++it)
    {
        X_real.push_back(it->first);
        Y_real.push_back(it->second);
    }
    return Matrix{X_real, Y_real};
}

Matrix Calc::GenerateNormalDistribution(const date& begin, const date& end)
{
    return GenerateNormalDistribution(FindDate(begin), FindDate(end));
}

Matrix Calc::GenerateNormalDistribution(size_t begin, size_t end)
{
    Matrix dist = GenerateDistribution(begin, end);
    double exp_val = ExpectedValue(begin, end);
    double sigma = CKO(begin, end);
    std::vector<double> X_ideal; //ideal
    std::vector<double> Y_ideal; //ideal
    double d = hist_d;
    double h = d;
    for (double i = 3.5; i >=0; i -= 0.125)
    {
        X_ideal.push_back(-i);
        Y_ideal.push_back((end - begin) * h * Fi((i - exp_val) / sigma) / sigma / arr.size());
    }
    if (NormalDistNormalized(begin, end))
    {
        for (double i = 0; i <= 3.5; i += 0.125)
        {
            X_ideal.push_back(i);
            Y_ideal.push_back((end - begin) * h * Fi((i - exp_val) / sigma) / sigma / arr.size());
        }
    }
    return Matrix{X_ideal, Y_ideal};
}

Matrix Calc::DistrToHist(Matrix dist)
{
    std::vector<double> X_real = dist[0];
    std::vector<double> Y_real = dist[1];
    std::vector<double> X;
    std::vector<double> Y;
    double d = hist_d;
    for (size_t i = 0; i < X_real.size(); ++i)
    {
        X.push_back(X_real[i] - d / 2);
        Y.push_back(0);
        X.push_back(X.back());
        Y.push_back(Y_real[i] / arr.size());
        X.push_back(X_real[i] + d / 2);
        Y.push_back(Y_real[i] / arr.size());
        X.push_back(X_real[i] + d / 2);
        Y.push_back(0);
    }
    return Matrix{X, Y};
}

double Calc::Normalize(const date& begin, const date& end)
{
    return Normalize(FindDate(begin), FindDate(end));
}

double Calc::Normalize(size_t begin, size_t end)
{
    if (Normalized(begin, end))
    {
        return 1.0;
    }
    double max = 0.0;
    std::for_each(arr.begin() + begin, arr.begin() + end, [&max](const double& d)
    {
        if (fabs(d) > fabs(max)) max = d;
    });
    for (size_t i = begin; i != end; ++i)
    {
        arr[i] /= max;
        arr[i] = fabs(arr[i]);
    }
    return max;
}

void Calc::NormalizeNormalDist(const date& begin, const date& end)
{
    NormalizeNormalDist(FindDate(begin), FindDate(end));
}

void Calc::NormalizeNormalDist(size_t begin, size_t end)
{
    if (NormalDistNormalized(begin, end))
    {
        return;
    }
    double exp_val = ExpectedValue(begin, end);
    double sigma = CKO(begin, end);
    for (size_t i = begin; i != end; ++i)
    {
        arr[i] = (arr[i] - exp_val) / sigma;
    }
    hist_d /= sigma;
}

bool Calc::Normalized(const date& begin, const date& end) const
{
    return Normalized(FindDate(begin), FindDate(end));
}

bool Calc::Normalized(size_t begin, size_t end) const
{
    for(size_t i = begin; i != end; ++i)
    {
        if (arr[i] < 0 || arr[i] > 1) return 0;
    }
    return 1;
}

bool Calc::NormalDistNormalized(const date& begin, const date& end, double eps) const
{
    return NormalDistNormalized(FindDate(begin), FindDate(end), eps);
}

bool Calc::NormalDistNormalized(size_t begin, size_t end, double eps) const
{
    return fabs(ExpectedValue(begin, end)) < eps;
}

std::string Calc::CheckNormalDist(const date& begin, const date& end, size_t normalize_norm_dist)
{
    return CheckNormalDist(FindDate(begin), FindDate(end), normalize_norm_dist);
}

std::string Calc::CheckNormalDist(size_t begin, size_t end, size_t normalize_norm_dist)
{
    Normalize(begin, end);
    MakeInterval(begin, end);
    if (normalize_norm_dist)
    {
        NormalizeNormalDist(begin, end);
    }
    auto plotdist = DistrToHist(GenerateDistribution(begin, end));
    auto plotnormdist = GenerateNormalDistribution(begin, end);
    Plot plot;
    plot.setPath("C:\\Users\\ACTIONFENIX\\Desktop\\");
    std::string rand_filename = std::string(tmpnam(nullptr)) + ".grf";
    plot.setFileName(rand_filename);
    plot.addPlot(plotdist[0], plotnormdist[1], "Ряд 1");
    plot.addPlot(plotdist[0], plotnormdist[1], "Ряд 2");
    plot.makePlot();
    return plot.getFullName();
}

double Calc::KohrenCheck(const date& begin, const date& end, size_t m)
{
    return KohrenCheck(FindDate(begin), FindDate(end), m);
}

double Calc::KohrenCheck(size_t begin, size_t end, size_t m)
{
    std::vector<double> arr_p = arr;
    std::vector<double> fullfilled_arr(arr);
    bool bOk;
    int N = QInputDialog::getInt(nullptr, "Кількість значень у інтервалі:", "N = ", 2, 2, 1000, 1, &bOk);
    if (!bOk)
    {
        return end;
    }
    end -= RemoveMissesBySharle(begin, end);
    end = DemoFullFill(begin, end, m, N);
    size_t q = (end - begin) / m - 1;
    buffer << q;
    double kkp = 0.0;
    double max_disp = 0.0;
    double disp = 0.0;
    size_t low_border, high_border;
    const size_t size = end - begin;
    for (size_t i = 1; i <= m; ++i)
    {
        low_border = ceil((size / m)) * (i - 1);
        high_border = ceil((size / m)) * i;
        /*low_border = N * (i - 1);
        high_border = N * i;*/
        disp = Dispersion(begin + low_border, begin + high_border);
        disp *= disp;
        if (disp > max_disp) max_disp = disp;
        kkp += disp;
    }
    kkp = max_disp / kkp;
    arr = arr_p;
    return kkp;
}

std::string Calc::GetBuffer()
{
    std::string buffer_cpy = buffer.str();
    buffer.str("");
    return buffer_cpy;
}

const date& Calc::BeginDate() const
{
    return *date_arr.begin();
}

const date& Calc::EndDate() const
{
    return date_arr.back();
}

size_t Calc::FindDate(const date& src) const
{
    return std::find_if(date_arr.begin(), date_arr.end(), [&src](const date& arr_val)
    {
        return arr_val == src;
    }) - date_arr.begin();
}

size_t Calc::FindMonth(size_t m) const
{
    return std::find_if(date_arr.begin(), date_arr.end(), [&m](const date& arr_val)
    {
        return arr_val.Month() == m;
    }) - date_arr.begin();
}

size_t Calc::FindWeek(size_t w) const
{
    return std::find_if(date_arr.begin(), date_arr.end(), [&w](const date& arr_val)
    {
        return arr_val.Day() == (w - 1) * 7 + 1;
    }) - date_arr.begin();
}

size_t Calc::FindDay(size_t d) const
{
    return std::find_if(date_arr.begin(), date_arr.end(), [&d](const date& arr_val)
    {
        return arr_val.Day() == d;
    }) - date_arr.begin();
}

size_t Calc::FindNext(const date& src) const
{
    return FindDate(src + step);
}

size_t Calc::NearestMonth(size_t begin) const
{
    return std::find_if(date_arr.begin() + begin + 1, date_arr.end(), [](const date& arr_val)
    {
        return arr_val.Day() == 0 && arr_val.Time() == "00:00";
    }) - date_arr.begin();
}

size_t Calc::NearestWeek(size_t begin) const
{
    return std::find_if(date_arr.begin() + begin + 1, date_arr.end(), [](const date& arr_val)
    {
        return ((arr_val.Day() - 1) % 7) == 0 && arr_val.Time() == "00:00";
    }) - date_arr.begin();
}

size_t Calc::NearestDay(size_t begin) const
{
    return std::find_if(date_arr.begin() + begin + 1, date_arr.end(), [](const date& arr_val)
    {
        return arr_val.Time() == "00:00";
    }) - date_arr.begin();
}

size_t Calc::NearestHalfDay(size_t begin) const
{
    return std::find_if(date_arr.begin() + begin + 1, date_arr.end(), [](const date& arr_val)
    {
        return arr_val.Time() == "12:00";
    }) - date_arr.begin();
}

size_t Calc::NearestHour(size_t begin) const
{
    return std::find_if(date_arr.begin() + begin + 1, date_arr.end(), [](const date& arr_val)
    {
        return arr_val.Hour() == 0;
    }) - date_arr.begin();
}

size_t Calc::NearestHalfHour(size_t begin) const
{
    return begin + 1;
}

size_t Calc::DemoFullFill(size_t begin, size_t end, size_t m, size_t N)
{
    double exp_val = ExpectedValue(begin, end);
    if (end - begin > m * N)
    {
        ErrorHandler("Помилка: end - begin > m * N");
    }
    while ((end - begin) < m * N)
    {
        arr.push_back(exp_val);
        ++end;
    }
    return end;
}

void Calc::FullFillData()
{
    if (date_arr[0].Time() != date(1, 1, "00:00").Time())
    {
        date_arr.insert(date_arr.begin(), date(1, 1, "00:00"));
        arr.insert(arr.begin(), arr[0]);
    }
    for (size_t i = 1; i < size(); ++i)
    {
        if (date_arr[i] - date_arr[i - 1] != HalfHour && date_arr[i] - date_arr[i - 1] < Day)
        {
            if (date_arr[i].Day() != date_arr[i - 1].Day())
            {
                if (date_arr[i].Time() == "00:00")
                {
                    date_arr.insert(date_arr.begin() + i, date(date_arr[i - 1].Month(), date_arr[i - 1].Day(), (date_arr[i - 1] + HalfHour).Time()));
                }
                else
                {
                    date_arr.insert(date_arr.begin() + i, date(date_arr[i].Month(), date_arr[i].Day(), "00:00"));
                }
            }
            else
            {
                date_arr.insert(date_arr.begin() + i, date(date_arr[i].Month(), date_arr[i].Day(), (date_arr[i - 1] + HalfHour).Time()));
            }
            arr.insert(arr.begin() + i, (arr[i - 1] + arr[i]) / 2);
            --i;
        }
    }
}

