#ifndef FILESDATA_H
#define FILESDATA_H
#include <QObject>

class FilesData
{
public:
    FilesData();
    ~FilesData();
    QStringList* dataFiles;
    QString csvFile;
    QString outPath;
    QString defaultFias;
    QString fiasFile;
};

#endif // FILESDATA_H
