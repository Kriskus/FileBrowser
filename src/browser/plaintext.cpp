#include "plaintext.h"

#include <QStringLiteral>
#include <QTextBlock>

PlainText::PlainText()
{
    setLineWrapMode(QPlainTextEdit::NoWrap);
}

void PlainText::getLine(const QPoint& point)
{
    int currentLine = cursorForPosition(point).blockNumber();
    const QTextDocument *doc = document();
    QString allText = doc->toPlainText();
    QStringList lines = allText.split("\n");
    setSelectedLine(cursorForPosition(point).blockNumber());
    emit sendCurrentText(lines[currentLine]);
}

void PlainText::setSelectedLine(int currentLine)
{
    QTextCursor cursor = this->textCursor();

    cursor.movePosition(QTextCursor::Start);
    for (int i = 0; i < currentLine; ++i) {
        cursor.movePosition(QTextCursor::Down);
    }

    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}

void PlainText::getData(const QList<QString> &data)
{
    fileList_ = data;
    updateData();
}

void PlainText::setFilter(const QString &filter)
{
    filter_ = filter;
    if(filter_.size() > 2) {
        updateData();
    } else {
        clearAll();
    }
}

void PlainText::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        getLine(e->pos());
    }
}

void PlainText::updateData()
{
    newList_.clear();
    if(filter_ != "") {
        for (const auto& el : fileList_) {
            if (el.contains(filter_, Qt::CaseInsensitive)) {
                newList_.append(el + '\n');
            }
        }
    }
    clearAll();
    insertPlainText(newList_);
}

void PlainText::clearAll()
{
    clear();
}
