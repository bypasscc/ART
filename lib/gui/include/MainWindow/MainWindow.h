//===- MainWindow.h - ART-GUI MainWindow -----------------------*- C++ -*-===//
//
//                     ANDROID REVERSE TOOLKIT
//
// This file is distributed under the GNU GENERAL PUBLIC LICENSE
// V3 License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines ART-GUI MainWindow's behaviour.
//
//===----------------------------------------------------------------------===//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WorkSpace/WorkSpace.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    // File Menu
    void actionExit();
    // Help Menu
    void actionAboutArt();
private:
    Ui::MainWindow *ui;

    WorkSpace* mCenterWidget;
};

#endif // MAINWINDOW_H