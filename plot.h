#ifndef PLOT_H
#define PLOT_H

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "errorhandler.h"

struct SinglePlot;

class Plot
{
    std::vector<SinglePlot> plots;
    std::string path;
    std::string filename;
    std::ofstream file;

public:
    enum Interpolation{Linear, Spline1D = 3, Spline2D = 1, HalfCos = 2};

public:
    Plot() = default;
    Plot(const std::string& fullname);
    Plot(const std::string& pathname, const std::string& plotname);
    void setFullName(const std::string& fullname);
    std::string getFullName() const;
    void setPath(const std::string& pth);
    void setFileName(const std::string& fname);
    void addPlot(const std::vector<double>& X, const std::vector<double>& Y);
    void addPlot(const std::vector<double>& X, const std::vector<double>& Y, const std::string& legend);
    void addPlot(const std::vector<double>& X, const std::vector<double>& Y, const std::string& legend, Interpolation intp);
    void makePlot();

private:
    std::string getPath(const std::string& fullname) const;
    std::string getFileName(const std::string& fullname) const;
};

struct SinglePlot
{
    std::vector<double> X;
    std::vector<double> Y;
    std::string Legend;
    Plot::Interpolation interp;
};

#endif // PLOT_H

