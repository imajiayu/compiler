#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Console)
{
    ui->setupUi(this);
    setWindowTitle(QString("1950509 马家昱 类C语言编译器"));
    setFixedSize(828,600);
    input_mode=0;//0是手动输入1是文件读入
    file_path="";

    ui->source_browser->clear();
    ui->result_browser->clear();
    //ui->source_browser->setEnabled(true);

    connect(ui->manul_input,SIGNAL(clicked()),this,SLOT(manul_input_clicked()));
    connect(ui->file_input,SIGNAL(clicked()),this,SLOT(file_input_clicked()));
    connect(ui->clear_source,SIGNAL(clicked()),this,SLOT(clear_source_clicked()));
    connect(ui->choose_file,SIGNAL(clicked()),this,SLOT(choose_file_clicked()));
    connect(ui->lexical_but,SIGNAL(clicked()),this,SLOT(lexical_clicked()));
    connect(ui->parse_but,SIGNAL(clicked()),this,SLOT(parse_clicked()));
    connect(ui->intermediate_but,SIGNAL(clicked()),this,SLOT(intermediate_clicked()));
    connect(ui->mips_but,SIGNAL(clicked()),this,SLOT(mips_clicked()));


    ui->manul_input->setChecked(true);
    ui->clear_source->setEnabled(true);
    ui->file_input->setChecked(false);
    ui->choose_file->setEnabled(false);

    to_mips=false;
}

Console::~Console()
{
    delete ui;
}

void Console::manul_input_clicked()
{
    ui->manul_input->setChecked(true);
    ui->clear_source->setEnabled(true);
    ui->file_input->setChecked(false);
    ui->choose_file->setEnabled(false);
    ui->source_browser->setEnabled(true);
    ui->source_browser->setReadOnly(false);
    to_mips=false;
}

void Console::file_input_clicked()
{
    file_path="";
    input_mode=1;
    ui->manul_input->setChecked(false);
    ui->clear_source->setEnabled(false);
    ui->file_input->setChecked(true);
    ui->choose_file->setEnabled(true);
    ui->source_browser->setReadOnly(true);
}

void Console::clear_source_clicked()
{
    ui->clear_source->setText(QString("清空输入框"));
    ui->source_browser->setReadOnly(false);
    ui->source_browser->clear();
    ui->result_browser->clear();
    to_mips=false;
}

void Console::choose_file_clicked()
{
    file_path = QFileDialog::getOpenFileName(this,"选择资源文件","./*.*");
    QFile* file=new QFile;
    file->setFileName(file_path);
    file->open(QIODevice::ReadOnly);
    QTextStream in(file);
    ui->source_browser->setText(in.readAll());
    ui->result_browser->clear();
    file->close();
    to_mips=false;
}

void Console::lexical_clicked()
{
    ui->result_browser->clear();
    if(input_mode&&file_path=="")
    {
        QMessageBox::critical(NULL, QString("错误"), QString("尚未选择文件"), QMessageBox::Yes);
        return;
    }
    if(!input_mode)
    {
        QFile file(".\\source.txt");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out<<ui->source_browser->toPlainText();
        file.close();
        file_path=".\\source.txt";
        ui->source_browser->setReadOnly(true);
        ui->clear_source->setText(QString("重新输入"));
    }

    QProcess a(this);
    QStringList argument;
    argument<<file_path<<"./lexical_result.txt"<<"./statistics.txt";
    a.start(".\\lexical.exe",argument);
    a.waitForFinished(3000);

    QFile* file=new QFile;
    file->setFileName("./statistics.txt");
    file->open(QIODevice::ReadOnly);
    QTextStream in2(file);
    ui->result_browser->append("行数"+in2.readLine());
    ui->result_browser->append("字符数"+in2.readLine());
    ui->result_browser->append("单词数"+in2.readLine());
    file->close();

    file->setFileName(".\\lexical_result.txt");
    file->open(QIODevice::ReadOnly);
    QTextStream in(file);
    ui->result_browser->append(in.readAll());
    file->close();
}

void Console::parse_clicked()
{
    ui->result_browser->clear();
    if(input_mode&&file_path=="")
    {
        QMessageBox::critical(NULL, QString("错误"), QString("尚未选择文件"), QMessageBox::Yes);
        return;
    }
    if(!input_mode)
    {
        QFile file(".\\source.txt");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out<<ui->source_browser->toPlainText();
        file.close();
        file_path=".\\source.txt";
        ui->source_browser->setReadOnly(true);
        ui->clear_source->setText(QString("重新输入"));
    }

    QProcess a(this);
    QStringList argument;
    argument<<file_path<<"./parse_result.txt";
    a.start(".\\parse.exe",argument);
    a.waitForFinished(3000);

    QFile* file=new QFile;
    file->setFileName(".\\parse_result.txt");
    file->open(QIODevice::ReadOnly);
    QTextStream in(file);
    in.setCodec(QTextCodec::codecForName("utf-8") );
    ui->result_browser->setText(in.readAll());
    file->close();
}
void Console::intermediate_clicked()
{
    ui->result_browser->clear();
    if(input_mode&&file_path=="")
    {
        QMessageBox::critical(NULL, QString("错误"), QString("尚未选择文件"), QMessageBox::Yes);
        return;
    }
    if(!input_mode)
    {
        QFile file(".\\source.txt");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out<<ui->source_browser->toPlainText();
        file.close();
        file_path=".\\source.txt";
        ui->source_browser->setReadOnly(true);
        ui->clear_source->setText(QString("重新输入"));
    }

    QProcess a(this);
    QStringList argument;
    argument<<file_path<<"./intermediate_result.txt";
    a.start(".\\intermediate.exe",argument);
    a.waitForFinished(3000);

    QFile* file=new QFile;
    file->setFileName(".\\intermediate_result.txt");
    file->open(QIODevice::ReadOnly);
    QTextStream in(file);
    ui->result_browser->setText(in.readAll());
    file->close();

    to_mips=true;
}
void Console::mips_clicked()
{
    ui->result_browser->clear();
    if(input_mode&&file_path=="")
    {
        QMessageBox::critical(NULL, QString("错误"), QString("尚未选择文件"), QMessageBox::Yes);
        return;
    }
    if(!input_mode)
    {
        QFile file(".\\source.txt");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out<<ui->source_browser->toPlainText();
        file.close();
        file_path=".\\source.txt";
        ui->source_browser->setReadOnly(true);
        ui->clear_source->setText(QString("重新输入"));
    }

    if(!to_mips)
    {
        QMessageBox::critical(NULL, QString("错误"), QString("尚未生成中间代码"), QMessageBox::Yes);
        return;
    }

    QProcess a(this);
    QStringList argument;
    argument<<".\\intermediate_result.txt"<<"./code.txt";
    a.start(".\\GenMips.exe",argument);
    a.waitForFinished(3000);

    QFile* file=new QFile;
    file->setFileName(".\\code.txt");
    file->open(QIODevice::ReadOnly);
    QTextStream in(file);
    ui->result_browser->setText(in.readAll());
    file->close();
}
