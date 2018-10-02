#include "reader.h"
#include "date.h"
#include "errorhandler.h"
#include <string>
#include <stdexcept>
#include <cstring>

Reader::Reader(const Reader& src): date_arr(src.date_arr), arr(src.arr)
{

}

Reader::Reader(const std::string filename)
{
    Open(filename);
}

void Reader::Open(const std::string filename)
{
    Close();
    file.open(filename);
    std::string extension = GetFileExtension(filename);
    if (extension == ".txt")
    {
        while (ReadCommentary() != "");
        if (file.peek() == 'd')
            ReadEnhancedTXT();
        else
            ReadTXT();
    }
    else
    {
        Close();
        ErrorHandler("Невідомий тип файлу");
    }
}

void Reader::Close()
{
    file.close();
    date_arr.clear();
    arr.clear();
}

const std::vector<date>& Reader::GetDateArr() const
{
    return date_arr;
}
const std::vector<double>& Reader::GetArr() const
{
    return arr;
}

std::string Reader::GetFileExtension(const std::string filename)
{
    if (filename.find(".txt", 0, strlen(".txt")) < filename.size())
    {
        return std::string(".txt");
    }
    else
    {
        return (filename.find(".", 0, strlen(".")) < filename.size()) ? filename.substr(filename.find(".")) : "no_extension";
    }
}

void Reader::ReadTXT()
{
    int      day = 1;
    long     position;

    while (!file.eof())
    {
        position = file.tellg();
        date_arr.push_back(date(1, day++, std::string("00:00")));
        while (ReadCommentary() != "");
        if (file.eof())
            return;
        double tmp = ReadNum();
        arr.push_back(tmp);
        SkipNewLine();
        if (position == file.tellg())
        {
            ErrorHandler("Помилка при зчитуванні файлу. Не вдалося зчитати рядок.");
            return ;
        }
    }
}

void Reader::ReadEnhancedTXT()
{
    long     position;

    file.get(); //d
    SkipNewLine(); //\n
    while (!file.eof())
    {
        position = file.tellg();
        while (ReadCommentary() != "");
        if (file.eof())
            return;
        double tmp = ReadNum();
        arr.push_back(tmp);
        date_arr.push_back(date(ReadStr()));
        if (date_arr.back() == date())
        {
            ErrorHandler("Прочитано невірну дату.");
            return ;
        }
        SkipNewLine();
        if (position == file.tellg())
        {
            ErrorHandler("Помилка при зчитуванні файлу. Не вдалося зчитати рядок.");
            return ;
        }
    }
}

double Reader::ReadNum()
{
    double num;
    if (file.peek() == '+' || file.peek() == ' ')
    {
        file.get();
    }
    if (isdigit(file.peek()) || file.peek() == '-')
    {
        file >> num;
    }
    else
    {
    if (file.peek() == '\n')
        {
            file.get();
        }
        num = double();
    }
    return num;
}

std::string Reader::ReadStr()
{
    std::string input;
    while (file.peek() != delim)
    {
        input += file.get();
    }
    SkipNewLine();
    return input;
}

std::string Reader::ReadCommentary()
{
    std::string input;
    if (file.peek() == '#')
    {
        while (!file.eof() && file.peek() != '\n')
            input += file.get();
    }
    SkipNewLine();
    return input;
}

void Reader::SkipNewLine()
{
    if (!file.eof() && (file.peek() == '\n' || file.peek() == '\r'))
    {
        file.get();
    }
}

