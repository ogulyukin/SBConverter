#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filesdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_csvButton_clicked();

    void on_filesButton_clicked();

    void on_outButton_clicked();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_resultButton_clicked();

private:
    Ui::MainWindow *ui;
    FilesData *filesData;
    QString fileList(QStringList *list);
    bool convert(QString filename, QString filename2, QString path);
};
#endif // MAINWINDOW_H
