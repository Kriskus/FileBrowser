#include "generatefilelist.h"

GenerateFileList::GenerateFileList(QObject *parent)
    : QObject(parent)
{}

void GenerateFileList::startOperation()
{
    if (openFile()) {
        readFilesList(m_rootPath);
    }
    emit sendInfo("Zakończono odczyt dysku!");
    emit finished();
}

void GenerateFileList::readFilesList(const QString &path)
{
    QDir dir(path);
    if (!checkDirExist(dir)) {
        return;
    }
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden);

    foreach (const QFileInfo &fileInfo, list) {
        if (checkPathIsFile(fileInfo)) {
            savePath(fileInfo.absoluteFilePath());
        } else if (checkPathIsDir(fileInfo)) {
            readFilesList(fileInfo.absoluteFilePath());
        }
    }
}

bool GenerateFileList::checkPathIsFile(const QFileInfo &fileInfo)
{
    return fileInfo.isFile();
}

bool GenerateFileList::checkPathIsDir(const QFileInfo &fileInfo)
{
    return fileInfo.isDir();
}

bool GenerateFileList::checkDirExist(const QDir &dir)
{
    if (!dir.exists()) {
        emit sendInfo("Error: Katalog nie istnieje");
        return false;
    }
    return true;
}

bool GenerateFileList::openFile()
{
    m_outputFile.reset(new QFile(m_outputFileName));
    if (!m_outputFile->open(QIODevice::ReadWrite | QIODevice::Text)) {
        emit sendInfo(m_outputFile->errorString());
        return false;
    } else {
        m_outStream.reset(new QTextStream(m_outputFile.data()));
        m_outStream->setCodec("UTF-8"); // Ustawienie kodowania na UTF-8
        return true;
    }
}

void GenerateFileList::savePath(const QString &pathFile)
{
    if (m_outputFile->isOpen()) {
        *m_outStream << pathFile << "\n";
    } else {
        emit sendInfo("Error: Próba zapisu do zamkniętego pliku");
    }
}
