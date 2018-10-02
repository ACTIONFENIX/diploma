#include "errorhandler.h"
#include <stdexcept>
#include <QMessageBox>
#include <QString>

ErrorHandler::ErrorHandler(const std::string& msg, handling_type t): type(t)
{
    ++calls;
    if (calls == 4)
    {
        QMessageBox msgbox;
        msgbox.setText(QString("ErrorHandler був викликаний уже 5 разів. Подальші некритичні виклики не будуть відображатися.") + msg.c_str());
        msgbox.exec();
    }
    if (calls > 1) return;
    if (type == handling_type::message)
    {
        QMessageBox msgbox;
        msgbox.setText(QString("Помилка: ") + msg.c_str());
        msgbox.exec();
    }
    else //exception
    {
        //throw std::logic_error(msg);
        QMessageBox msgbox;
        msgbox.setText(QString("Помилка: ") + msg.c_str());
        msgbox.exec();
        //exit(EXIT_FAILURE);
    }
}

int ErrorHandler::calls = 0;

