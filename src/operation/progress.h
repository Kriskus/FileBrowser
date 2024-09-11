#ifndef PROGRESS_H
#define PROGRESS_H

#include <QObject>
#include <QThread>

class Progress : public QObject
{
    Q_OBJECT
public:
    explicit Progress(QObject *parent = nullptr);

public slots:
    void strtProgress();
    void endProgress();

private:
    QStringList progress_{};
    bool m_threadRunning{true};

signals:
    void sendProgress(QString);
    void finished();
};

#endif // PROGRESS_H
