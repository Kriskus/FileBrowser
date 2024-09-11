#ifndef LISTOPERATION_H
#define LISTOPERATION_H

#include <QObject>

class ListOperation : public QObject
{
    Q_OBJECT
public:
    explicit ListOperation();

public slots:
    QList<QString> prepareList(QList<QString> data);

signals:
    void sendInfo(const QString&);
};

#endif // LISTOPERATION_H
