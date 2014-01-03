#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "shipwidget.h"

class GLWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend class HyCur;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:

    void on_action_import_triggered();

    void on_action_Fbur_triggered();

    void on_action_Fubowen_triggered();

    void on_action_drawHyCur_triggered();

    void on_action_X_triggered();

    void on_action_Y_triggered();

    void on_action_Z_triggered();

    void on_action_exit_triggered();

    void on_action_ExLP_triggered();

    void on_action_ExHC_triggered();

    void on_action_ExBC_triggered();

    void on_action_drawBC_triggered();

private:
    Ui::MainWindow *ui;
    ShipWidget *ship1;
    QGridLayout *centralLayout;
};

#endif // MAINWINDOW_H
