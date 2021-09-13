#include "filesdata.h"

FilesData::FilesData()
{
    csvFile = "";
    outPath = "";
    fiasFile = "";
    dataFiles = new QStringList();
}
FilesData::~FilesData()
{
    delete dataFiles;
}
