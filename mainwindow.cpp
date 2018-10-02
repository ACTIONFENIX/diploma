#include "mainwindow.h"
#include "errorhandler.h"
#include "regression.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QInputDialog>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Statistics v5.0a");
    load_file = new QPushButton("Відкрити файл");
    setMouseTracking(true);
    load_file->installEventFilter(this);
    choose_action = new QComboBox;
    choose_action->addItem("Очистити екран");
    choose_action->addItem("Показати дані");
    choose_action->addItem("Параметри розподілу");
    choose_action->addItem("Побудувати графік розподілу");
    choose_action->addItem("Перевірка на нормальний закон");
    choose_action->addItem("Відтворюваність вимірів");
    choose_action->addItem("Лінійна регресія");
    choose_action->addItem("Модифікована лінійна регресія");
    choose_action->setCurrentIndex(0);
    button_OK = new QPushButton("OK");
    linear_layout = new QHBoxLayout;
    linear_layout->addWidget(load_file);
    linear_layout->addWidget(choose_action);
    linear_layout->addWidget(button_OK);
    show_text = new QTextEdit;
    left_layout = new QVBoxLayout;
    left_layout->addLayout(linear_layout);
    left_layout->addWidget(show_text);
    right_layout1 = new QVBoxLayout;
    edit_begin_month = new QLineEdit;
    label_begin_month = new QLabel("Місяць");
    edit_begin_day = new QLineEdit;
    label_begin_day = new QLabel("День");
    edit_begin_hour = new QLineEdit;
    label_begin_hour = new QLabel("Година");
    edit_begin_minute = new QLineEdit;
    label_begin_minute = new QLabel("Хвилина");
    right_layout1->addWidget(label_begin_month);
    right_layout1->addWidget(edit_begin_month);
    right_layout1->addWidget(label_begin_day);
    right_layout1->addWidget(edit_begin_day);
    right_layout1->addWidget(label_begin_hour);
    right_layout1->addWidget(edit_begin_hour);
    right_layout1->addWidget(label_begin_minute);
    right_layout1->addWidget(edit_begin_minute);
    right_layout2 = new QVBoxLayout;
    edit_end_month = new QLineEdit;
    label_end_month = new QLabel("Місяць");
    edit_end_day = new QLineEdit;
    label_end_day = new QLabel("День");
    edit_end_hour = new QLineEdit;
    label_end_hour = new QLabel("Година");
    edit_end_minute = new QLineEdit;
    label_end_minute = new QLabel("Хвилина");
    right_layout2->addWidget(label_end_month);
    right_layout2->addWidget(edit_end_month);
    right_layout2->addWidget(label_end_day);
    right_layout2->addWidget(edit_end_day);
    right_layout2->addWidget(label_end_hour);
    right_layout2->addWidget(edit_end_hour);
    right_layout2->addWidget(label_end_minute);
    right_layout2->addWidget(edit_end_minute);
    right_layout3 = new QVBoxLayout;
    edit_step_month = new QLineEdit("0");
    label_step_month = new QLabel("Місяць");
    edit_step_day = new QLineEdit("0");
    label_step_day = new QLabel("День");
    edit_step_hour = new QLineEdit("0");
    label_step_hour = new QLabel("Година");
    edit_step_minute = new QLineEdit("30");
    label_step_minute = new QLabel("Хвилина");
    right_layout3->addWidget(label_step_month);
    right_layout3->addWidget(edit_step_month);
    right_layout3->addWidget(label_step_day);
    right_layout3->addWidget(edit_step_day);
    right_layout3->addWidget(label_step_hour);
    right_layout3->addWidget(edit_step_hour);
    right_layout3->addWidget(label_step_minute);
    right_layout3->addWidget(edit_step_minute);
    main_layout = new QHBoxLayout;
    main_layout->addLayout(left_layout);
    main_layout->addLayout(right_layout1);
    main_layout->addLayout(right_layout2);
    main_layout->addLayout(right_layout3);
    setLayout(main_layout);
    QObject::connect(button_OK, SIGNAL(clicked(bool)), this, SLOT(do_action()));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            QString filename = QFileDialog::getOpenFileName(nullptr, "Відкрити файл", "", "*.txt *.csv");
            if (filename.isEmpty())
            {
                return true;
            }
            open_file(filename);
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::open_file(const QString& file)
{
    calc.SetReader(Reader(file.toStdString()));
}

void MainWindow::do_action()
{
    QString action = choose_action->currentText();
    updateDate();
    if (action == "Очистити екран")
    {
        show_text->clear();
    }
    else if (action == "Показати дані")
    {
        show_text->setText(calc.GetData(calc.FindDate(begin_date), calc.FindDate(end_date)).c_str());
    }
    else if (action == "Параметри розподілу")
    {
        std::ostringstream out;
        out << "N = " << steps(end_date - begin_date, calc.getStep()) << std::endl << std::endl;
        out << "Математичне очікування: " << std::endl;
        out << "M = " << calc.ExpectedValue(begin_date, end_date) << std::endl;
        out << std::endl;
        out << "Дисперсія: " << std::endl;
        out << "D = " << calc.Dispersion(begin_date, end_date) << std::endl;
        out << std::endl;
        out << "С.К.В.: " << std::endl;
        out << "C = " << calc.CKO(begin_date, end_date) << std::endl;
        out << std::endl;
        show_text->setText(out.str().c_str());
    }
    else if (action == "Побудувати графік розподілу")
    {
        calc.MakeInterval(begin_date, end_date);
        calc.Normalize(begin_date, end_date);
        Matrix distrib = calc.GenerateDistribution(begin_date , end_date);
        Matrix hist = calc.DistrToHist(distrib);
        Plot plot;
        plot.setPath("C:\\Users\\ACTIONFENIX\\Desktop\\");
        std::string rand_filename = std::string(tmpnam(nullptr)) + ".grf";
        plot.setFileName(rand_filename);
        plot.addPlot(distrib[0], distrib[1], "Ряд 1", Plot::Spline1D);
        plot.makePlot();
        show_text->setText(("Збережено як " + plot.getFullName()).c_str());
    }
    else if (action == "Перевірка на нормальний закон")
    {
        //ErrorHandler("Перевірка на нормальний закон розподілу на даний момент не робоча.");
        //return;
        show_text->setText((std::string("Збережено як ") + calc.CheckNormalDist(begin_date, end_date, true).c_str()).c_str());
    }
    else if (action == "Відтворюваність вимірів")
    {
        bool bOk;
        int m = QInputDialog::getInt(this, "Кількість інтервалів розбиття:", "m = ", 2, 2, 1000, 1, &bOk);
        if (!bOk)
        {
            return;
        }
        double res = calc.KohrenCheck(begin_date, end_date, m);
        std::ostringstream out;
        out << "m = " << m << std::endl;
        out << "q = " << calc.GetBuffer() << std::endl;
        out << "Kkp = " << res << std::endl;
        show_text->setText(out.str().c_str());
    }
    else if (action == "Лінійна регресія")
    {
        std::vector<double> Tarr = calc.asVector(begin_date, end_date + HalfHour).Vector();
        std::vector<double> FFarr = calc.asVector(begin_date, end_date + HalfHour).Vector();
        bool bOk;
        size_t len = QInputDialog::getInt(this, "Кількість змінних у рівнянні регресії", "Кількість:", 2, 2, sqrt(steps(end_date - begin_date, HalfHour)) + 1, 1, &bOk);
        if (!bOk)
        {
            return;
        }
        Matrix A(len, len);
        for (size_t i = 0; i < A.size(); ++i)
        {
            for (size_t j = 0; j < A.size(); ++j)
            {
                A[i][j] = Tarr[i + j];
            }
        }
        Matrix B(len, 1);
        for (size_t i = 0; i < len; ++i)
        {
            B[i][0] = FFarr[i + len];
        }
        std::ostringstream out;
        out << "Система рівнянь:" << std::endl;
        for (size_t i = 0; i < len; ++i)
        {
            for (size_t j = 0; j < len; ++j)
            {
                out << A[i][j] << "*X" << j + 1;
                if (j != len - 1)
                {
                    out << " + ";
                }
            }
            out << " = " << B[i][0] << std::endl;
        }
        Regression regr;
        regr.setX(A);
        regr.setY(B);
        Matrix X = regr.LinearRegression();
        for (size_t i = 0; i < X.rows(); ++i)
        {
            out << "X" << i + 1 << " = " << X[i][0] << "\n";
        }
        Matrix predictMatrix(1, len);
        for (size_t i = 0; i < len; ++i)
        {
            predictMatrix[0][i] = Tarr[len + i];
            //out << "predictMatrix[0][i] = " << predictMatrix[0][i] << std::endl;
        }
        out << "Передбачення: " << regr.PredictFor(predictMatrix) << std::endl;
        out << "Реальне значення: " << FFarr[len + len - 1] << std::endl;
        show_text->setText(out.str().c_str());
    }
    else if (action == "Модифікована лінійна регресія")
    {
        std::vector<double> Tarr = calc.asVector(begin_date, end_date + HalfHour).Vector();
        std::vector<double> FFarr = calc.asVector(begin_date, end_date + HalfHour).Vector();
        bool bOk;
        int len = QInputDialog::getInt(this, "Кількість змінних у рівнянні регресії", "Кількість:", 2, 2, sqrt(steps(end_date - begin_date, HalfHour)) + 1, 1, &bOk);
        if (!bOk)
        {
            return;
        }
        Matrix A(len, len);
        for (size_t i = 0; i < A.size(); ++i)
        {
            for (size_t j = 0; j < A.size(); ++j)
            {
                A[i][j] = Tarr[i + j];
            }
        }
        Matrix B(len, 1);
        for (size_t i = 0; i < len; ++i)
        {
            B[i][0] = FFarr[i + len];
        }
        std::ostringstream out;
        out << "Система рівнянь:" << std::endl;
        for (size_t i = 0; i < len; ++i)
        {
            for (size_t j = 0; j < len; ++j)
            {
                out << A[i][j] << "*X" << j + 1;
                if (j != len - 1)
                {
                    out << " + ";
                }
            }
            out << " = " << B[i][0] << std::endl;
        }
        Regression regr;
        regr.setX(A);
        regr.setY(B);
        Matrix X = regr.LinearRegression();
        for (size_t i = 0; i < X.rows(); ++i)
        {
            out << "X" << i + 1 << " = " << X[i][0] << "\n";
        }
        Matrix predictMatrix(1, len);
        for (size_t i = 0; i < len; ++i)
        {
            predictMatrix[0][i] = Tarr[len + i];
            //out << "predictMatrix[0][i] = " << predictMatrix[0][i] << std::endl;
        }
        double pred = regr.PredictFor(predictMatrix);
        if (pred < 0 || std::isnan(pred))
        {
            pred = FFarr[len + len - 2];
            out << "Передбачення(виправлено): " << pred << std::endl;
        }
        else
        {
            out << "Передбачення: " << pred << std::endl;
        }
        out << "Реальне значення: " << FFarr[len + len - 1] << std::endl;
        show_text->setText(out.str().c_str());
    }
    else
    {
        ErrorHandler("Операція не визначена.");
        choose_action->setCurrentText("Нічого не робити");
    }
}

void MainWindow::updateDate()
{
    date begin, end, step;
    begin.setMonth(edit_begin_month->text().toInt());
    begin.setDay(edit_begin_day->text().toInt());
    begin.setHour(edit_begin_hour->text().toInt());
    begin.setMinute(edit_begin_minute->text().toInt());
    end.setMonth(edit_end_month->text().toInt());
    end.setDay(edit_end_day->text().toInt());
    end.setHour(edit_end_hour->text().toInt());
    end.setMinute(edit_end_minute->text().toInt());
    step.setMonth(edit_step_month->text().toInt());
    step.setDay(edit_step_day->text().toInt());
    step.setHour(edit_step_hour->text().toInt());
    step.setMinute(edit_step_minute->text().toInt());
    if (step == date())
    {
        step = HalfHour;
    }
    if (begin == date())
    {
        begin = calc.BeginDate();
    }
    if (end == date() || end >= calc.EndDate())
    {
        end = calc.EndDate();
    }
    begin_date = begin;
    end_date = end + HalfHour;
    calc.setStep(step);
}

