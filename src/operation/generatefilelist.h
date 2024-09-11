#ifndef GENERATEFILELIST_H
#define GENERATEFILELIST_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QFileInfoList>
#include <QScopedPointer>

#include <QDebug>

class GenerateFileList : public QObject
{
    Q_OBJECT
public:
    explicit GenerateFileList(QObject *parent = nullptr);

public slots:
    void startOperation();
    void readFilesList(const QString& path);

private:
    QScopedPointer<QTextStream> m_outStream;
    QScopedPointer<QFile> m_outputFile;

    QString m_rootPath{"W:\\"};
    QString m_outputFileName{"W:\\newfilelist.txt"};

private slots:
    bool checkPathIsFile(const QFileInfo &fileInfo);
    bool checkPathIsDir(const QFileInfo &fileInfo);
    bool checkDirExist(const QDir &dir);

    bool openFile();
    void savePath(const QString& pathFile);

signals:
    void sendInfo(const QString &);
    void finished();
};

#endif // GENERATEFILELIST_H
