#include "fileread.h"

#include <QThread>

FileRead::FileRead(const QString& filename)
    : file_(new QFile)
    , m_operation(new ListOperation)
{
    file_.data()->setFileName(filename);
}

FileRead::~FileRead()
{}

void FileRead::readFile()
{
    emit started();
    if (file_.data()->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(file_.data());
        in.setCodec("UTF-8");
        while (!in.atEnd()) {
            fileList_.push_back(in.readLine());
        }
        file_.data()->close();
        emit sendData(m_operation->prepareList(fileList_));
    } else {
        emit sendFileError(file_->errorString());
    }
    emit sendInfo("Zakończono odczyt pliku");
    emit finished();
}
