#ifndef READER_H
#define READER_H

#include <fstream>
#include <vector>
#include "date.h"

class Reader
{
private:
    std::ifstream file;
    std::vector<date> date_arr;
    std::vector<double> arr;
    const char delim = '\n';

public:
    Reader() {}

    Reader(const Reader& src);

    Reader(const std::string filename);

    void Open(const std::string filename);

    void Close();

    const std::vector<date>& GetDateArr() const;

    const std::vector<double>& GetArr() const;

private:
    std::string GetFileExtension(const std::string filename);

    void ReadEnhancedTXT();

    void ReadTXT();

    double ReadNum();

    std::string ReadStr();

    std::string ReadCommentary();

    void SkipSemicolon();

    void SkipNewLine();
};

#endif // READER_H

