//===- FindDialog.cpp - ART-GUI Find ---------------------------*- C++ -*-===//
//
//                     ANDROID REVERSE TOOLKIT
//
// This file is distributed under the GNU GENERAL PUBLIC LICENSE
// V3 License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "Find/FindDialog.h"
#include "ui_FindDialog.h"

#include <utils/ScriptEngine.h>
#include <utils/ProjectInfo.h>
#include <utils/StringUtil.h>


#include "FindConfig.h"
#include "FindResult.h"

FindDialog::FindDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    mFindConfig = new FindConfig(this);
    ui->mHistoryCombobox->addItem (tr("New Search"));
    ui->mSearchStackedWidget->addWidget (mFindConfig);

    ui->mHistoryCombobox->setCurrentIndex (0);

    connect(ui->mClearButton, SIGNAL(clicked(bool)), this, SLOT(closeAll()));
    connect(mFindConfig, SIGNAL(startNewFind(QString,QString,QTextDocument::FindFlags,bool,bool)),
            this, SLOT(onNewFind(QString,QString,QTextDocument::FindFlags,bool,bool)));


    ScriptEngine* script = ScriptEngine::instance();
    connect(script, SIGNAL(projectOpened(QStringList)),
            this, SLOT(onProjectOpened(QStringList)));
    connect(script, SIGNAL(projectClosed(QStringList)),
            this, SLOT(onProjectClosed ()));
}

FindDialog::~FindDialog()
{
    delete ui;
}

bool FindDialog::closeAll ()
{
    while(ui->mHistoryCombobox->count () > 1) {
        FindResult* p = (FindResult*)ui->mSearchStackedWidget->widget (1);
        ui->mSearchStackedWidget->removeWidget (p);
        ui->mHistoryCombobox->removeItem (1);
        delete p;
    }
    return true;
}

void FindDialog::onProjectOpened (QStringList args)
{
    QString projectPath = ProjectInfo::current()->getSourcePath();
    mFindConfig->reset (projectPath, projectPath);
}

void FindDialog::onProjectClosed ()
{
    closeAll ();
    mFindConfig->reset (QString(), QString());
}

void FindDialog::onFindAdvance (const QString& dir)
{
    auto pinfo = ProjectInfo::current();
    if(pinfo == nullptr) {
        return;
    }

    QString projectPath = pinfo->getSourcePath();
    if(dir.isEmpty ()) {
        mFindConfig->reset (projectPath, projectPath);
    } else {
        mFindConfig->reset (projectPath, dir);
    }

}

void FindDialog::onNewFind (const QString &subString,const QString &directory,
                            QTextDocument::FindFlags options,
                            bool useRegexp,bool needReplace)
{
    auto findresult = new FindResult(this);
    findresult->startNewFind (subString,directory,options,useRegexp,needReplace);

    ui->mHistoryCombobox->insertItem (1, subString);
    ui->mSearchStackedWidget->insertWidget (1, findresult);
    ui->mHistoryCombobox->setCurrentIndex (1);
}


