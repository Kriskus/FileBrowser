#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include <QProcess>
#include <QThread>
#include <QTimer>

#include "browser/plaintext.h"
#include "file/fileread.h"
#include "file/jsonreader.h"

#include "operation/listoperation.h"
#include "operation/generatefilelist.h"
#include "operation/progress.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QScopedPointer<PlainText> plainText_;
    QScopedPointer<QTimer> m_progressBarTimer{};
    QScopedPointer<JSONReader> m_jsonReader{};

    bool m_settingsVisible{true};

    QFont m_searchFont;
    QFont m_listFont;

    bool m_directoryOpen{false};
    bool m_totalCMD{false};
    bool m_leftWindow{false};

private slots:
    void copyToClipboard(QString currentText);
    void readFileList();
    void generateNewList();
    void showProgressBar();
    void progressBarStart();

    void showQMessageBox(const QString& info);
    void updateStatusBar(const QString& info);

    void openDirectory(const QString& directory);

    void settingsVisibility();
    void setButtonAppTypeVisibility(bool visible);
    void setButtonSideVisibility(bool visible);

    void setNewSearchFontParameters();
    void setNewListFontParameters();
    void setNewDirectoryParameters();

    void loadSearchFontParameters(const QFont& font);
    void loadListFontParameters(const QFont& font);
    void loadDirectoryParameters(bool directoryRead, bool totalCMD, bool leftWindow);

    void loadConfiguration();

signals:
    void sendNewDirectorySettings(bool, bool, bool);
    void sendNewSearchFont(const QFont&);
    void sendNewListFont(const QFont&);
};
#endif // MAINWINDOW_H
