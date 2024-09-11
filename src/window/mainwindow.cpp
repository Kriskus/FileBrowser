#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plainText_(new PlainText())
    , m_progressBarTimer(new QTimer)
    , m_jsonReader(new JSONReader)
{
    ui->setupUi(this);

    qRegisterMetaType<QList<QString>>("QList<QString>");

    connect(m_progressBarTimer.data(), &QTimer::timeout, this, &MainWindow::showProgressBar);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &MainWindow::readFileList);
    connect(ui->pushButtonRead, &QPushButton::clicked, this, &MainWindow::generateNewList);

    connect(ui->lineEditSearch, &QLineEdit::textChanged, plainText_.data(), &PlainText::setFilter);
    connect(plainText_.data(), &PlainText::sendCurrentText, this, &MainWindow::copyToClipboard);

    ui->verticalLayoutPlainText->addWidget(plainText_.data());
    readFileList();

    connect(ui->pushButtonSettings, &QPushButton::clicked, this, &MainWindow::settingsVisibility);
    connect(ui->radioButtonTotalCmd, &QRadioButton::toggled, this, &MainWindow::setButtonAppTypeVisibility);
    connect(ui->checkBoxOpenDirectory, &QCheckBox::toggled, this, &MainWindow::setButtonSideVisibility);

    connect(m_jsonReader.data(), &JSONReader::sendDirectorySettings, this, &MainWindow::setNewDirectoryParameters);
    connect(m_jsonReader.data(), &JSONReader::sendListFont, this, &MainWindow::setNewListFontParameters);
    connect(m_jsonReader.data(), &JSONReader::sendSearchFont, this, &MainWindow::setNewSearchFontParameters);
    connect(this, &MainWindow::sendNewDirectorySettings, m_jsonReader.data(), &JSONReader::getNewDirectoryParam);
    connect(this, &MainWindow::sendNewListFont, m_jsonReader.data(), &JSONReader::getNewListFont);
    connect(this, &MainWindow::sendNewSearchFont, m_jsonReader.data(), &JSONReader::getNewSearchFont);

    settingsVisibility();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::copyToClipboard(QString currentText)
{
    if(currentText[currentText.size()-4] == '.') {
        QStringList temp = currentText.split("/");
        currentText.clear();
        for(auto& el: temp) {
            if(el == temp.last()) {
                break;
            } else {
                currentText.push_back(el + "/");
            }
        }
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(currentText);
    if(ui->checkBoxOpenDirectory->isChecked()) {
        openDirectory(currentText);
    }
}

void MainWindow::readFileList()
{
    progressBarStart();
    QThread* readerThread = new QThread(this);
    FileRead* fileRead = new FileRead("W:\\newfileList.txt");
    ListOperation* operation = new ListOperation();

    fileRead->moveToThread(readerThread);
    operation->moveToThread(readerThread);

    connect(readerThread, &QThread::started, fileRead, &FileRead::readFile);

    connect(fileRead, &FileRead::sendData, plainText_.data(), &PlainText::getData);
    connect(operation, &ListOperation::sendInfo, this, &MainWindow::updateStatusBar);
    connect(fileRead, &FileRead::sendInfo, this, &MainWindow::showQMessageBox);

    connect(fileRead, &FileRead::finished, m_progressBarTimer.data(), &QTimer::stop);
    connect(fileRead, &FileRead::finished, readerThread, &QThread::quit);
    connect(readerThread, &QThread::finished, readerThread, &QThread::deleteLater);
    connect(fileRead, &FileRead::finished, fileRead, &FileRead::deleteLater);

    readerThread->start();
}

void MainWindow::generateNewList()
{
    progressBarStart();
    QThread* generatorThread = new QThread(this);
    GenerateFileList* generator = new GenerateFileList();

    generator->moveToThread(generatorThread);

    connect(generatorThread, &QThread::started, generator, &GenerateFileList::startOperation);

    connect(generator, &GenerateFileList::sendInfo, this, &MainWindow::updateStatusBar);
    connect(generator, &GenerateFileList::sendInfo, this, &MainWindow::showQMessageBox);

    connect(generator, &GenerateFileList::finished, m_progressBarTimer.data(), &QTimer::stop);
    connect(generator, &GenerateFileList::finished, generatorThread, &QThread::quit);
    connect(generatorThread, &QThread::finished, generatorThread, &QThread::deleteLater);
    connect(generator, &GenerateFileList::finished, generator, &GenerateFileList::deleteLater);

    generatorThread->start();
}

void MainWindow::showProgressBar()
{
    if(ui->progressBar->invertedAppearance()) {
        ui->progressBar->setValue(ui->progressBar->value() - 1);
        if(ui->progressBar->value() == 0) {
            ui->progressBar->setInvertedAppearance(0);
        }
    } else {
        ui->progressBar->setValue(ui->progressBar->value() + 1);
        if(ui->progressBar->value() == 1000) {
            ui->progressBar->setInvertedAppearance(1);
        }
    }
}

void MainWindow::progressBarStart()
{
    m_progressBarTimer->start(1);
}

void MainWindow::showQMessageBox(const QString &info)
{
    QMessageBox::information(this, "Info", info, QMessageBox::Ok);
}

void MainWindow::updateStatusBar(const QString &info)
{
    ui->statusbar->showMessage(info, 2500);
}

void MainWindow::openDirectory(const QString &directory)
{
    QProcess cmdProcess;

    // Konwersja ścieżki do formatu natywnego dla Windows
    QString nativeDirectory = QDir::toNativeSeparators(directory);

    // Argumenty do otwarcia katalogu w lewym panelu uruchomionego Total Commandera
    QStringList arguments;
    if(ui->radioButtonLeftSide->isChecked()) {
        arguments << "/O" << "/L=" + nativeDirectory;
    } else {
        arguments << "/O" << "/R=" + nativeDirectory;
    }

    if (!QProcess::startDetached("C:\\Program Files\\totalcmd\\TOTALCMD64.EXE", arguments)) {
        qDebug() << "Nie udało się otworzyć katalogu w Total Commander: " << directory;
    }
}

void MainWindow::settingsVisibility()
{
    m_settingsVisible = !m_settingsVisible;
    ui->checkBoxListBold->setVisible(m_settingsVisible);
    ui->checkBoxListItalic->setVisible(m_settingsVisible);
    ui->checkBoxOpenDirectory->setVisible(m_settingsVisible);
    ui->checkBoxSearchBold->setVisible(m_settingsVisible);
    ui->checkBoxSearchItalic->setVisible(m_settingsVisible);
    ui->spinBoxListSizeFont->setVisible(m_settingsVisible);
    ui->spinBoxSearchSizeFont->setVisible(m_settingsVisible);
    ui->labelListFont->setVisible(m_settingsVisible);
    ui->labelSearchFont->setVisible(m_settingsVisible);
    ui->pushButtonRead->setVisible(m_settingsVisible);
    ui->pushButtonRefresh->setVisible(m_settingsVisible);
    ui->radioButtonWindows->setVisible(m_settingsVisible);
    ui->radioButtonTotalCmd->setVisible(m_settingsVisible);
    if(ui->radioButtonTotalCmd->isChecked() && m_settingsVisible) {
        ui->radioButtonLeftSide->setVisible(true);
        ui->radioButtonRightSide->setVisible(true);
    } else {
        ui->radioButtonLeftSide->setVisible(false);
        ui->radioButtonRightSide->setVisible(false);
    }
}

void MainWindow::setButtonAppTypeVisibility(bool visible)
{
    ui->radioButtonTotalCmd->setVisible(visible);
    ui->radioButtonWindows->setVisible(visible);
    if(!ui->radioButtonTotalCmd->isVisible()) {
        setButtonSideVisibility(false);
    }
}

void MainWindow::setButtonSideVisibility(bool visible)
{
    ui->radioButtonLeftSide->setVisible(visible);
    ui->radioButtonRightSide->setVisible(visible);
}

void MainWindow::setNewSearchFontParameters()
{
    m_searchFont.setPointSize(ui->spinBoxSearchSizeFont->value());
    m_searchFont.setBold(ui->checkBoxSearchBold->isChecked());
    m_searchFont.setItalic(ui->checkBoxSearchItalic->isChecked());
    emit sendNewListFont(m_searchFont);
}

void MainWindow::setNewListFontParameters()
{
    m_listFont.setPointSize(ui->spinBoxListSizeFont->value());
    m_listFont.setBold(ui->checkBoxListBold->isChecked());
    m_listFont.setItalic(ui->checkBoxListItalic->isChecked());
    emit sendNewListFont(m_listFont);
}

void MainWindow::setNewDirectoryParameters()
{
    m_directoryOpen = ui->checkBoxOpenDirectory->isChecked();
    m_totalCMD = ui->radioButtonTotalCmd->isChecked();
    m_leftWindow = ui->radioButtonLeftSide->isChecked();
    emit sendNewDirectorySettings(m_directoryOpen, m_totalCMD, m_leftWindow);
}

void MainWindow::loadSearchFontParameters(const QFont &font)
{
    ui->lineEditSearch->setFont(font);
    ui->labelSearchFont->setFont(font);
}

void MainWindow::loadListFontParameters(const QFont &font)
{
    plainText_->setFont(font);
    plainText_->setFilter(ui->lineEditSearch->text());
    ui->labelSearchFont->setFont(font);
}

void MainWindow::loadDirectoryParameters(bool directoryRead, bool totalCMD, bool leftWindow)
{
    ui->checkBoxOpenDirectory->setChecked(directoryRead);
    ui->radioButtonTotalCmd->setChecked(totalCMD);
    ui->radioButtonLeftSide->setChecked(leftWindow);
}

void MainWindow::loadConfiguration()
{
    m_jsonReader->readFile();
}
