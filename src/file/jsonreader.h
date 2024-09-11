#ifndef JSONREADER_H
#define JSONREADER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFont>

class JSONReader : public QObject
{
    Q_OBJECT
public:
    explicit JSONReader(QObject *parent = nullptr);

public slots:
    void readFile();
    void saveFile();

    void getNewDirectoryParam(bool openDirectory, bool totalCMD, bool leftWindow);
    void getNewSearchFont(const QFont& searchFont);
    void getNewListFont(const QFont& listFont);

private:
    QString m_filename{"setting.json"};
    bool m_openDirectory{};
    bool m_totalCMD{};
    bool m_leftWindow{};
    QFont m_searchFont{};
    QFont m_listFont{};

private slots:
    bool checkDocument(const QJsonDocument &document);
    bool checkDirectorySettingsExist(const QJsonObject &rootObj);
    bool checkFontsSettingsExist(const QJsonArray &fontsArray);

    void setDirectoryParam(const QJsonObject& directoryObject);
    void setSearchFontParam(const QJsonObject& fontObject);
    void setListFontParam(const QJsonObject& fontObject);

signals:
    void sendDirectorySettings(bool, bool, bool);
    void sendSearchFont(const QFont&);
    void sendListFont(const QFont&);

    void sendError(const QString&);
};

#endif // JSONREADER_H
