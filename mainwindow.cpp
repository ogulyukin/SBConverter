#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "csvio.h"

#define version "2.0"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::setWindowTitle("Конвертер формата Сбербанка");
    filesData = new FilesData();
    ui->fiasLineEdit->setText("acde8f61-fd56-4cec-a4df-1f033e2a63ee");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_csvButton_clicked()
{
    filesData->csvFile = QFileDialog::getOpenFileName(this, "Open File",
                                                        QDir::currentPath(),
                                                        tr("xlsx files (*.xlsx);;All files (*.*)"));
    ui->csvLabel->setText(filesData->csvFile);
    qDebug() << "xlsx file choosen : " << filesData->csvFile;
}


void MainWindow::on_filesButton_clicked()
{
    *filesData->dataFiles = QFileDialog::getOpenFileNames(this, "Open File",
                                                        QDir::currentPath(),
                                                        tr("Text files (*.txt);;All files (*.*)"));
    ui->filesLabel->setText(fileList(filesData->dataFiles));
    qDebug() << "You choose : " << *filesData->dataFiles;
}


void MainWindow::on_outButton_clicked()
{
    filesData->outPath = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::currentPath(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->resultLabel->setText(filesData->outPath);
    qDebug() << "Out path choosen : " << filesData->outPath;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "О Программе", "Гулюкин О.В. 2021 \nПрограмма для конвертации начислений\nпо новым требованиям сбербанка\nВерсия " + QString(version));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

bool MainWindow::convert(QString filename, QString filename2, QString path)
{
    if (filename == "" || filename2 == "" || path == "")
    {
        QMessageBox::information(this, "Ошибка", "Незаполнены имена файлов или путь к файлам результатов!");
        return false;
    }
    csvIO acc_file(filename, filename2);
    QMap<QString, Account> map;
    QMap<QString, QString> accNumbers;
    QList<QString> head;
    QString result = acc_file.getDataFromCsv(&map, &accNumbers, &head);
    if(result != "OK")
    {
        QMessageBox::information(this,"Ошибка", result);
        return false;
    }
    QMap<QString, Account>::Iterator i;
    for(i = map.begin(); i != map.end(); i++)
    {
        i.value().setEls(accNumbers.value(i.key()));
        i.value().setFias(filesData->fias);
    }
    result = acc_file.saveModifedFile(&map, &head, path);
    map.clear();
    head.clear();
    accNumbers.clear();
    return true;

}

QString MainWindow::fileList(QStringList *list)
{
    QString result = "";
    QStringList::Iterator it;
    for (it = list->begin(); it != list->end(); it++)
        result += ("\n" + *it);
    return result;
}


void MainWindow::on_resultButton_clicked()
{
    filesData->fias = ui->fiasLineEdit->text();
    QStringList::Iterator it;
    int succes = 0;
    int errors = 0;
    for (it = filesData->dataFiles->begin(); it != filesData->dataFiles->end(); it++)
    {
        ui->statusbar->showMessage("Обрабатывается: " + *it );
        bool result = convert(filesData->csvFile, *it, filesData->outPath);
        result? succes++ : errors++;
    }
    ui->statusbar->showMessage("Конертация завершина!", 1000);
    QMessageBox::information(this, "Результат",  "Успешно: " + QString::number(succes)
                             + "\nС Ошибкой " + QString::number(errors), QMessageBox::Close);
}

