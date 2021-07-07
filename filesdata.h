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
};

#endif // FILESDATA_H
