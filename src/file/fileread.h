#pragma once

#include <QObject>
#include <QTextStream>
#include <QThread>
#include <QString>
#include <QFile>
#include <QList>

#include "../operation/listoperation.h"

class FileRead : public QObject
{
    Q_OBJECT
public:
    explicit FileRead(const QString &filename = {});
    ~FileRead();

public slots:
    void readFile();

private:
    QScopedPointer<QFile> file_{};
    QScopedPointer<ListOperation> m_operation{};
    QList<QString> fileList_{};

signals:
    void sendData(const QList<QString>&);
    void sendFileError(const QString&);
    void sendInfo(const QString&);
    void started();
    void finished();
};
