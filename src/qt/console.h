#ifndef CONSOLE_H
#define CONSOLE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>
#include <QByteArray>
#include <qtextcodec.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Console; }
QT_END_NAMESPACE

class Console : public QMainWindow
{
    Q_OBJECT

public:
    Console(QWidget *parent = nullptr);
    ~Console();
    int input_mode;
    QString file_path;
    bool to_mips;
private slots:
    void manul_input_clicked();
    void file_input_clicked();
    void clear_source_clicked();
    void choose_file_clicked();
    void lexical_clicked();
    void parse_clicked();
    void intermediate_clicked();
    void mips_clicked();


private:
    Ui::Console *ui;
};
#endif // CONSOLE_H
