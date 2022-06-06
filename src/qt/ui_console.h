/********************************************************************************
** Form generated from reading UI file 'console.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Console
{
public:
    QWidget *centralwidget;
    QTextBrowser *result_browser;
    QPushButton *lexical_but;
    QRadioButton *manul_input;
    QRadioButton *file_input;
    QPushButton *choose_file;
    QPushButton *clear_source;
    QLabel *label;
    QLabel *label_2;
    QTextEdit *source_browser;
    QPushButton *parse_but;
    QPushButton *intermediate_but;
    QPushButton *mips_but;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Console)
    {
        if (Console->objectName().isEmpty())
            Console->setObjectName(QStringLiteral("Console"));
        Console->resize(828, 600);
        centralwidget = new QWidget(Console);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        result_browser = new QTextBrowser(centralwidget);
        result_browser->setObjectName(QStringLiteral("result_browser"));
        result_browser->setGeometry(QRect(450, 90, 371, 461));
        lexical_but = new QPushButton(centralwidget);
        lexical_but->setObjectName(QStringLiteral("lexical_but"));
        lexical_but->setGeometry(QRect(450, 10, 71, 71));
        manul_input = new QRadioButton(centralwidget);
        manul_input->setObjectName(QStringLiteral("manul_input"));
        manul_input->setGeometry(QRect(20, 20, 110, 19));
        file_input = new QRadioButton(centralwidget);
        file_input->setObjectName(QStringLiteral("file_input"));
        file_input->setGeometry(QRect(20, 50, 110, 19));
        choose_file = new QPushButton(centralwidget);
        choose_file->setObjectName(QStringLiteral("choose_file"));
        choose_file->setGeometry(QRect(110, 46, 93, 28));
        clear_source = new QPushButton(centralwidget);
        clear_source->setObjectName(QStringLiteral("clear_source"));
        clear_source->setGeometry(QRect(110, 16, 93, 31));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(280, 49, 181, 41));
        QFont font;
        font.setFamily(QStringLiteral("Adobe Arabic"));
        font.setPointSize(20);
        label->setFont(font);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(750, 50, 71, 41));
        label_2->setFont(font);
        source_browser = new QTextEdit(centralwidget);
        source_browser->setObjectName(QStringLiteral("source_browser"));
        source_browser->setGeometry(QRect(10, 90, 431, 461));
        parse_but = new QPushButton(centralwidget);
        parse_but->setObjectName(QStringLiteral("parse_but"));
        parse_but->setGeometry(QRect(520, 10, 71, 71));
        intermediate_but = new QPushButton(centralwidget);
        intermediate_but->setObjectName(QStringLiteral("intermediate_but"));
        intermediate_but->setGeometry(QRect(590, 10, 71, 71));
        mips_but = new QPushButton(centralwidget);
        mips_but->setObjectName(QStringLiteral("mips_but"));
        mips_but->setGeometry(QRect(660, 10, 71, 71));
        Console->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Console);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 828, 26));
        Console->setMenuBar(menubar);
        statusbar = new QStatusBar(Console);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Console->setStatusBar(statusbar);

        retranslateUi(Console);

        QMetaObject::connectSlotsByName(Console);
    } // setupUi

    void retranslateUi(QMainWindow *Console)
    {
        Console->setWindowTitle(QApplication::translate("Console", "Console", Q_NULLPTR));
        result_browser->setHtml(QApplication::translate("Console", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'SimSun';\"><br /></p></body></html>", Q_NULLPTR));
        lexical_but->setText(QApplication::translate("Console", "\350\257\215\346\263\225\345\210\206\346\236\220", Q_NULLPTR));
        manul_input->setText(QApplication::translate("Console", "\346\211\213\345\212\250\350\276\223\345\205\245", Q_NULLPTR));
        file_input->setText(QApplication::translate("Console", "\346\226\207\344\273\266\350\257\273\345\205\245", Q_NULLPTR));
        choose_file->setText(QApplication::translate("Console", "\351\200\211\346\213\251\346\226\207\344\273\266", Q_NULLPTR));
        clear_source->setText(QApplication::translate("Console", "\346\270\205\347\251\272\350\276\223\345\205\245\346\241\206", Q_NULLPTR));
        label->setText(QApplication::translate("Console", "\345\276\205\345\210\206\346\236\220\347\250\213\345\272\217", Q_NULLPTR));
        label_2->setText(QApplication::translate("Console", "\347\273\223\346\236\234", Q_NULLPTR));
        source_browser->setHtml(QApplication::translate("Console", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'SimSun';\"><br /></p></body></html>", Q_NULLPTR));
        parse_but->setText(QApplication::translate("Console", "\346\226\207\346\263\225\345\210\206\346\236\220", Q_NULLPTR));
        intermediate_but->setText(QApplication::translate("Console", "\345\233\233\345\205\203\345\274\217", Q_NULLPTR));
        mips_but->setText(QApplication::translate("Console", "\346\261\207\347\274\226", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Console: public Ui_Console {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLE_H
