//===- Editor.cpp - ART-GUI Editor Tab -------------------------*- C++ -*-===//
//
//                     ANDROID REVERSE TOOLKIT
//
// This file is distributed under the GNU GENERAL PUBLIC LICENSE
// V3 License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "Editor.h"

#include <utils/Configuration.h>
#include "HighLighter/HighLighter.h"

#include <QTextStream>
#include <QtConcurrent/QtConcurrent>
#include <QtWidgets/QGridLayout>
#include <QFileInfo>
#include <QWidget>

void readFileThread(Editor* e, QString filePath);
void saveFileThread(QString text, QString  filePath);

Editor::Editor(QWidget *parent) :
        QWidget(parent)
{
    mFileChangedTimer = new QTimer(this);
    mFileEdit = new TextEditorWidget(this);

    connect(this, SIGNAL(readLine(QString)), this, SLOT(appendLine(QString)));
    connect(this, SIGNAL(readEnd()), this, SLOT(readFileEnd()));

    connect(mFileChangedTimer, SIGNAL(timeout()), this, SLOT(textChangedTimeOut()));

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    mFileEdit->setFont(font);

    loadFromConfig();
}

Editor::~Editor()
{
    closeFile();
    saveToConfig();
}

// TODO this method should be invoked after init.
void Editor::setTextLayout ()
{
    QLayout* layout = new QGridLayout(this);
    layout->setContentsMargins (0,0,0,0);
    layout->addWidget (mFileEdit);
    setLayout (layout);
}

bool Editor::openFile(QString filePath, int iLine)
{
    fp = filePath;
    QFile file(fp);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        fn = QFileInfo(file).fileName ();

        mFileEdit->setReadOnly(true);
        mFileEdit->setDocumentTitle(fn);
        mFileEdit->setDocumentPath(filePath);
        file.close();

        mHighlighter = setHighLighter(mFileEdit->document (), fn);

        mLine = iLine;
        QtConcurrent::run(readFileThread, this, filePath);
        return true;
    }
    return false;
}

bool Editor::saveFile()
{
    mFileChangedTimer->stop();
    if (isUpdated) {
        QtConcurrent::run(saveFileThread, mFileEdit->toPlainText(), fp);
    }

    isUpdated = false;
}

bool Editor::reload()
{
    mFileEdit->clear();
    QtConcurrent::run(readFileThread, this, fp);
}

void Editor::closeFile()
{
    saveFile();
}

void Editor::loadFromConfig()
{
//    Configuration *config = Config();
}

void Editor::saveToConfig()
{
//    Configuration *config = Config();
}

void Editor::gotoLine(int line, int column, bool centerLine)
{
    mFileEdit->gotoLine(1, column, centerLine);
}
int Editor::currentLine ()
{
    return mFileEdit->currentLine ();
}

void Editor::appendLine(QString line)
{
    mFileEdit->appendPlainText(line);
}

void Editor::readFileEnd()
{
    mFileEdit->setReadOnly(false);
    mFileEdit->setFocus();
    connect(mFileEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    mFileEdit->gotoLine(mLine);
}

void Editor::textChanged()
{
    isUpdated = true;
    mFileChangedTimer->start(60*1000);
}

void Editor::textChangedTimeOut()
{
    saveFile();
}

void readFileThread(Editor* e, QString filePath) {
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        while(!file.atEnd()) {
            emit e->readLine(file.readAll());
        }
        file.close();
    }
    emit e->readEnd();
    return ;
}

void saveFileThread(QString text, QString  filePath) {
    QFile file(filePath);
    if (file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        QTextStream out(&file);
        out<<text;
        out.flush();
        file.close();
    }
}
