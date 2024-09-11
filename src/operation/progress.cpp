#include "progress.h"

#include <QTimer>

Progress::Progress(QObject *parent)
    : QObject{parent}
{
    progress_ << "Trwa operacja .";
    progress_ << "Trwa operacja ..";
    progress_ << "Trwa operacja ...";
    progress_ << "Trwa operacja ....";
    progress_ << "Trwa operacja .....";
    progress_ << "Trwa operacja ......";
    progress_ << "Trwa operacja .......";
    progress_ << "Trwa operacja ........";
    progress_ << "Trwa operacja .........";
    progress_ << "Trwa operacja ..........";
}

void Progress::strtProgress()
{
    short lp = 0;
    while(true) {
        emit sendProgress(progress_[lp++]);
        if(lp == 10) {
            lp = 0;
        }
        QThread::msleep(250);
    }
    emit finished();
}

void Progress::endProgress()
{
    this->deleteLater();
}

