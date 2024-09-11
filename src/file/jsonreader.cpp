#include "jsonreader.h"

JSONReader::JSONReader(QObject *parent)
    : QObject{parent}
{

}

void JSONReader::readFile()
{
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit sendError(file.errorString());
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if(!checkDocument(document)) {
        emit sendError("Błąd pliku konfiguracji");
        return;
    }

    QJsonObject rootObject = document.object();
    if(rootObject.isEmpty()) {
        return;
    }

    if (checkDirectorySettingsExist(rootObject)) {
        setDirectoryParam(rootObject.value("Directory").toArray().first().toObject());
    }

    QJsonArray fontsArray = rootObject.value("Fonts").toArray();
    if (checkFontsSettingsExist(fontsArray)) {
        QJsonObject fontsObject = fontsArray.first().toObject();
        setSearchFontParam(fontsObject.value("SearchFont").toObject());
        setListFontParam(fontsObject.value("ListFont").toObject());
    }
}

void JSONReader::saveFile()
{
    QJsonObject rootObject;

        QJsonArray directoryArray;
        QJsonObject directoryObject;
        directoryObject.insert("OpenDirectory", m_openDirectory ? 1 : 0);
        QJsonObject totalCMDObject;
        totalCMDObject.insert("Checked", m_totalCMD ? 1 : 0);
        totalCMDObject.insert("WindowLeft", m_leftWindow ? 1 : 0);
        directoryObject.insert("TotalCMD", totalCMDObject);
        directoryArray.append(directoryObject);
        rootObject.insert("Directory", directoryArray);

        QJsonArray fontsArray;
        QJsonObject fontsObject;
        QJsonObject searchFontObject;
        searchFontObject.insert("FontSize", m_searchFont.pointSize());
        searchFontObject.insert("FontBold", m_searchFont.bold());
        searchFontObject.insert("FontItalic", m_searchFont.italic());
        fontsObject.insert("SearchFont", searchFontObject);
        QJsonObject listFontObject;
        listFontObject.insert("FontSize", m_listFont.pointSize());
        listFontObject.insert("FontBold", m_listFont.bold());
        listFontObject.insert("FontItalic", m_listFont.italic());
        fontsObject.insert("ListFont", listFontObject);
        fontsArray.append(fontsObject);
        rootObject.insert("Fonts", fontsArray);

        QJsonDocument document(rootObject);
        QFile file(m_filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emit sendError(file.errorString());
            return;
        }

        file.write(document.toJson());
        file.close();
}

void JSONReader::getNewDirectoryParam(bool openDirectory, bool totalCMD, bool leftWindow)
{
    m_openDirectory = openDirectory;
    m_totalCMD = totalCMD;
    m_leftWindow = leftWindow;
    saveFile();
}

void JSONReader::getNewSearchFont(const QFont &searchFont)
{
    m_searchFont = searchFont;
    saveFile();
}

void JSONReader::getNewListFont(const QFont &listFont)
{
    m_listFont = listFont;
    saveFile();
}

bool JSONReader::checkDocument(const QJsonDocument& document)
{
    return !document.isNull() && document.isObject();
}

bool JSONReader::checkDirectorySettingsExist(const QJsonObject &rootObj)
{
    return rootObj.contains("Directory") && rootObj.value("Directory").isArray();
}

bool JSONReader::checkFontsSettingsExist(const QJsonArray &fontsArray)
{
    return !fontsArray.isEmpty();
}

void JSONReader::setDirectoryParam(const QJsonObject &directoryObj)
{
    QJsonObject totalCMD = directoryObj.value("TotalCMD").toObject();
    bool openDirectory = directoryObj.value("OpenDirectory").toInt() == 1;
    bool totalCMDChecked = totalCMD.value("Checked").toInt() == 1;
    bool totalCMDWindowLeft = totalCMD.value("WindowLeft").toInt() == 1;

    emit sendDirectorySettings(openDirectory, totalCMDChecked, totalCMDWindowLeft);
}

void JSONReader::setSearchFontParam(const QJsonObject &fontObject)
{
    m_searchFont.setPointSize(fontObject.value("FontSize").toInt());
    m_searchFont.setBold(fontObject.value("FontBold").toBool());
    m_searchFont.setItalic(fontObject.value("FontItalic").toBool());

    emit sendSearchFont(m_searchFont);
}

void JSONReader::setListFontParam(const QJsonObject &fontObject)
{
    m_listFont.setPointSize(fontObject.value("FontSize").toInt());
    m_listFont.setBold(fontObject.value("FontBold").toBool());
    m_listFont.setItalic(fontObject.value("FontItalic").toBool());

    emit sendListFont(m_listFont);
}
