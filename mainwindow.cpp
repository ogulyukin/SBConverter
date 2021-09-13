#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "csvio.h"
#include <QSettings>

#define version "3.0"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::setWindowTitle("Конвертер формата для Сбербанка");
    filesData = new FilesData();
    loadSettings();
    ui->resultLabel->setText(filesData->outPath);
    ui->fiasLineEdit->setText(defaultFias);
    ui->csvLabel->setText(filesData->csvFile);
    ui->fiasLabel->setText(filesData->fiasFile);
    QString css = QString("background-color : %1").arg(QColor(Qt::darkGreen).name());
    ui->resultButton->setStyleSheet(css);
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
    if(defaultDataPath == "")
        defaultDataPath = QDir::currentPath();
    *filesData->dataFiles = QFileDialog::getOpenFileNames(this, "Open File",
                                                        defaultDataPath,
                                                        tr("Text files (*.txt);;All files (*.*)"));
    ui->filesLabel->setText(fileList(filesData->dataFiles));
    qDebug() << "You choose : " << *filesData->dataFiles;
    QFileInfo file(filesData->dataFiles->at(0));
    QFileInfo fInfo(file);
    defaultDataPath = fInfo.absolutePath();
    qDebug() << "defaulpDataPath" << defaultDataPath;
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


void MainWindow::on_resultButton_clicked()
{
    filesData->defaultFias = ui->fiasLineEdit->text();
    int succes = 0;
    int errors = 0;
    if(filesData->csvFile == "")
    {
        QMessageBox::information(this, "Ошибка", "Не выбран файл с данными по\n Лицевым счетам!");
        return;
    }
    if(filesData->fiasFile == "")
    {
        QMessageBox::information(this, "Предупреждение", "Не выбран файл с объектами учета\nИспользуется ФИАС по умолчанию!");
    }else
    {
        csvIO::loadFiasCodes(fiasCodes, accNumbers, filesData->csvFile ,filesData->fiasFile);
    }
    for (auto it = filesData->dataFiles->begin(); it != filesData->dataFiles->end(); it++)
    {
        //ui->statusbar->showMessage("Обрабатывается: " + *it );
        bool result = convert(*it, filesData->outPath);
        result? succes++ : errors++;
    }
    ui->statusbar->showMessage("Конертация завершина!", 1000);
    QMessageBox::information(this, "Результат",  "Успешно: " + QString::number(succes)
                             + "\nС Ошибкой " + QString::number(errors), QMessageBox::Close);
    QSettings settings("SB", "Path");
        settings.beginGroup("PathToData");
        settings.setValue("csvFile", filesData->csvFile);
        settings.setValue("outPath", filesData->outPath);
        settings.setValue("dataPath", defaultDataPath);
        settings.setValue("fias", ui->fiasLineEdit->text());
        settings.setValue("fiasPath", filesData->fiasFile);
        settings.endGroup();
}

bool MainWindow::convert(QString filename, QString path)
{
    if (filename == "" || path == "")
    {
        QMessageBox::information(this, "Ошибка", "Незаполнены имена файлов или путь к файлам результатов!");
        return false;
    }
    csvIO acc_file(filename);
    QMap<QString, Account> map;
    QList<QString> head;
    QString result = acc_file.getDataFromFiles(&map, &head, filesData->defaultFias, &fiasCodes);
    if(result != "OK")
    {
        QMessageBox::information(this,"Ошибка", result);
        return false;
    }
    for(auto i = map.begin(); i != map.end(); i++)
    {
        QString temp = i.value().getAccountNumber().toUpper();
        QString foundedELS = accNumbers.value(temp);
        if(foundedELS == "")
        {
            QString temp = i.value().getAccountNumber().toLower();
            foundedELS = accNumbers.value(temp);
        }
        i.value().setEls(foundedELS);
        //i.value().setFias(filesData->defaultFias);//remove this
    }
    result = acc_file.saveModifedFile(&map, &head, path);
    map.clear();
    head.clear();
    return true;
}

void MainWindow::loadSettings()
{
    QSettings settings("SB", "Path");
    settings.beginGroup("PathToData");
    filesData->csvFile = settings.value("csvFile", QVariant("")).toString();
    filesData->outPath = settings.value("outPath", QVariant("")).toString();
    defaultDataPath = settings.value("dataPath", QVariant("")).toString();
    defaultFias = settings.value("fias", QVariant("acde8f61-fd56-4cec-a4df-1f033e2a63ee")).toString();
    filesData->fiasFile = settings.value("fiasPath", QVariant("")).toString();
    settings.endGroup();
}

QString MainWindow::fileList(QStringList *list)
{
    QString result = "";
    QStringList::Iterator it;
    for (it = list->begin(); it != list->end(); it++)
        result += ("\n" + *it);
    return result;
}

void MainWindow::on_fiasButton_clicked()
{
    filesData->fiasFile = QFileDialog::getOpenFileName(this, "Open File",
                                                      QDir::currentPath(),
                                                      tr("xlsx files (*.xlsx);;All files (*.*)"));
    ui->fiasLabel->setText(filesData->fiasFile);
    qDebug() << "xlsx file choosen : " << filesData->fiasFile;
}

