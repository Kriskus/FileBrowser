#include "listoperation.h"

#include <QThread>

ListOperation::ListOperation()
{}

QList<QString> ListOperation::prepareList(QList<QString> data)
{
    QList<QString> list{};
    if(data.size() == 0) {
        emit sendInfo("Brak danych do odczytania");
    } else {
        for (auto& el : data) {
            QString modified = el.replace("Z:\\\\", "W:\\");
            if(modified[0] == ',') {
                modified.remove(0, 1);
            }
            list.push_back(modified);
        }
    }
    return list;
}
