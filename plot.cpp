#include "plot.h"

Plot::Plot(const std::string& fullname)
{
    path = getPath(fullname);
    filename = getFileName(fullname);
}

Plot::Plot(const std::string& pathname, const std::string& plotname)
{
    path = pathname;
    filename = plotname;
}

void Plot::setFullName(const std::string& fullname)
{
    path = getPath(fullname);
    filename = getFileName(fullname);
}

std::string Plot::getFullName() const
{
    return path + filename;
}

void Plot::setPath(const std::string& pth)
{
    path = pth;
}

void Plot::setFileName(const std::string& fname)
{
    filename = fname;
}

void Plot::addPlot(const std::vector<double>& X, const std::vector<double>& Y)
{
    plots.push_back({X, Y, "Ряд " + char(48 + plots.size() + 1), Linear});
}

void Plot::addPlot(const std::vector<double>& X, const std::vector<double>& Y, const std::string& legend)
{
    plots.push_back({X, Y, legend, Linear});
}

void Plot::addPlot(const std::vector<double>& X, const std::vector<double>& Y, const std::string& legend, Interpolation intp)
{
    plots.push_back({X, Y, legend, intp});
}

void Plot::makePlot()
{
    file.open(path + filename);
    if (!file)
        ErrorHandler("Не вдалося створити файл " + path + filename);
    file << ";This file was created by Graph (http://www.padowan.dk)\n";
    file << ";Do not change this file from other programs.\n";
    file << "[Graph]\n";
    file << "Version = 4.4.2.543\n";
    file << "MinVersion = 2.5\n";
    file << "OS = Windows NT 6.2\n\n";

    file << "[Axes]\n";
    file << "xMin = -10\n";
    file << "xMax = 10\n";
    file << "xTickUnit = 1\n";
    file << "xGridUnit = 1\n";
    file << "yMin = -10\n";
    file << "yMax = 10\n";
    file << "yTickUnit = 2\n";
    file << "yGridUnit = 2\n";
    file << "AxesColor = clBlue\n";
    file << "GridColor = 0x00FF9999\n";
    file << "ShowLegend = 1\n";
    file << "Radian = 1\n\n";

    for (size_t i = 0; i < plots.size(); ++i)
    {
        file << "[PointSeries1]\n";
        file << "FillColor = clBlue\n";
        file << "LineColor = clBlue\n";
        file << "Size = 0\n";
        file << "Style = 0\n";
        file << "LineStyle = 0\n";
        if (plots[i].interp != Linear)
        {
            file << "Interpolation = " << plots[i].interp << std::endl;
        }
        file << "LabelPosition = 1\n";
        file << "PointCount = " << plots[i].X.size() << "\n";
        file << "Points = ";
        for (size_t point_i = 0; point_i < plots[i].X.size(); ++point_i)
        {
            file << std::fixed << std::setprecision(3) << plots[i].X[point_i] << "," << std::fixed << std::setprecision(3) << plots[i].Y[point_i] << ";";
        }
        file << "\n";
        file << "LegendText = " << plots[i].Legend << "\n\n";
    }

    file << "[Data]\n";
    file << "TextLabelCount = 0\n";
    file << "FuncCount = 0\n";
    file << "PointSeriesCount = " << plots.size() << "\n";
    file << "ShadeCount = 0\n";
    file << "RelationCount = 0\n";
    file << "OleObjectCount = 0\n";
    file.close();
}

std::string Plot::getPath(const std::string& fullname) const
{
    auto founded_backslash = fullname.find_last_of("\\");
    if (founded_backslash == std::string::npos)
        ErrorHandler("Невірне повне імя файлу.");
    return fullname.substr(0, founded_backslash);
}

std::string Plot::getFileName(const std::string& fullname) const
{
    auto founded_backslash = fullname.find_last_of("\\");
    if (founded_backslash == std::string::npos)
        ErrorHandler("Невірне повне імя файлу.");
    return fullname.substr(founded_backslash + 1);
}

