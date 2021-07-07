#include "filesdata.h"

FilesData::FilesData()
{
    csvFile = "";
    outPath = "";
    dataFiles = new QStringList();
}
FilesData::~FilesData()
{
    delete dataFiles;
}
