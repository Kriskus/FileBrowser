#ifndef PLAINTEXT_H
#define PLAINTEXT_H

#include <QPlainTextEdit>
#include <QObject>
#include <QMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QList>
#include <QString>

class PlainText : public QPlainTextEdit
{
    Q_OBJECT
public:
    PlainText();

public slots:
    void clearAll();
    void getLine(const QPoint &point);
    void setSelectedLine(int currentLine);
    void getData(const QList<QString>& data);
    void setFilter(const QString &filter);

    void mouseDoubleClickEvent(QMouseEvent *e);

private:
    QList<QString> fileList_{};
    QString newList_{};
    QString filter_{};

    void updateData();

signals:
    void sendCurrentText(QString);
    void showList(QString);
};

#endif // PLAINTEXT_H
