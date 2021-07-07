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
    QString fias;
};

#endif // FILESDATA_H
