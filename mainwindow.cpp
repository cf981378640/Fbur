#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shipwidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowState(Qt::WindowMaximized);

    ui->setupUi(this);
    ship1 = new ShipWidget;

    ui->action_import->setShortcut(QKeySequence::Open);
    ui->action_drawHyCur->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_H));
    ui->action_X->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));
    ui->action_Y->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Y));
    ui->action_Z->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    ui->action_exit->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));

    ui->action_Fbur->setShortcut(QKeySequence::HelpContents);


    QGridLayout *centralLayout = new QGridLayout;

    centralLayout->setMargin (0);
    centralLayout->addWidget(ship1, 0, 0);
//    centralLayout->addWidget(ui->calendarWidget,1, 1);
    ui->centralWidget->setLayout(centralLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_import_triggered()
{
    QMessageBox::StandardButton reply,reply2;
    QString fileName;
    while(reply!=QMessageBox::Yes||reply2!=QMessageBox::Yes)
    {
        fileName = QFileDialog::getOpenFileName(this, tr("导入数据文件"), " ",
                                                        tr("数据文件(*.txt)"));
        if(fileName.isEmpty())break;
        ship1->Init();
        ship1->import(fileName.toStdString());
        QString info="请确认以下参数的正确性：\n"+("\n船舶类型:\t"+QString::number(ship1->cx)+"\n计算横剖面数:\t"+QString::number(ship1->m)+"\n计算水线数:\t"+QString::number(ship1->n)+"\n最大横剖面站号:\t"+QString::number(ship1->ma)+"\n横剖面插入点数:\t"+QString::number(ship1->cs)+"\n水线面插入点数\t"+QString::number(ship1->cw)+"\n有涡尾横剖面站数:\t"+QString::number(ship1->mw)+"\n有涡尾水线根数:\t"+QString::number(ship1->nw));
        QString info2="请确认以下参数的正确性：\n"+("\n船舯站号:\t"+QString::number(ship1->Xm)+"\n垂线间长:\t"+QString::number(ship1->Lpp)+"\n最 大 长:   \t"+QString::number(ship1->Loa)+"\n型     宽:    \t"+QString::number(ship1->B)+"\n设计水线宽:\t"+QString::number(ship1->Bw)+"\n标准站距:\t"+QString::number(ship1->deltaL)+"\n水的重度:\t"+QString::number(ship1->omega)+"\n甲板梁拱:\t"+QString::number(ship1->f)+"\n设计纵倾:\t"+QString::number(ship1->O));
        reply =QMessageBox::question(this,"确认数据！",info,QMessageBox::Yes|QMessageBox::No);
        reply2 =QMessageBox::question(this,"确认数据！",info2,QMessageBox::Yes|QMessageBox::No);
    }

    setWindowTitle(fileName+" - Fbur");
    ship1->calculate();
}

void MainWindow::on_action_Fbur_triggered()
{
    QString aboutF=" <h1>关于Fbur</h1><p>  正如你所看到的一样，Fbur是一款简单的软件，不出意外的话，你应该是在进行船舶静力学计算的时候使用它。如果你在使用Fbur的时候感觉受到了伤害，这确实是我们所不期望的，那么请你反馈给我们。</p>"
            "<p>  同时，Fbur应该是一款免费的软件。我们基于C++编程语言及相应的图形界面库完成了它的开发工作。我们必须感谢Bjarne Stroustrup设计出了如此优秀的编程语言，以至于我们的Fbur得以诞生，虽然在对Fbur进行开发的过程中，它让我们感觉受到了如此多的伤害。</p>"
            "<p>  你可以通过向作者索取或者其他任何合理的途径获得Fbur，当然，我们也允许你在没有征得我们同意的情况下，将它拷贝给他人使用，这是合理的。</p>"
            "<p>  最后，我们不期望你使用任何不合理的方式对Fbur进行商业目的的窜改，除非你的名字叫做付博文。</p>"
            "<p>最后，使用Fbur愉快！</p>";
    QMessageBox::about(this,"关于Fbur",aboutF);
}

void MainWindow::on_action_Fubowen_triggered()
{
    QString aboutF=" <h1>关于作者</h1><p>\t出乎我的意料，真的有人想要了解提供这款软件给我们的到底是什么人。那么我还是先简单讲讲Fbur的开发过程。</p>"
            "<p>\t软件编写于作者的大三学年，2013年。当时刚完成船舶静力学的课程不久，为我们授课的姜大奎老师开始鼓励我们编写自己专业的小软件。这实在是一个很有意思的想法，让我颇为心动。其后我开始频频出现在校图书馆的流通书库。感谢校图书馆提供了如此丰富的资源供我参考。其中对我影响较大的有C++之父的《C++程序设计原理与实践》及《C++ GUI Qt 4编程》及《交互式计算机图形学》。</p>"
            "<p>\t其后即疯狂地编写代码，先是写底层的各计算部分，计算部分大体完成后开始借用图形界面库Qt编写界面，另由出色的openGL完成三维图形的绘制。</p>"
            "<p>\t怎么说呢，编程实在是一件像玩网络游戏一样让人“上瘾的”事情，难以想象的，它们之间竟有如此多的共性！是的，那一堆代码会让你没日没夜地黏在计算机屏幕前面，还是不是要咒骂上几声！</p>"
            "<p>\t好在，那样的日子终于过去了，Fbur带来的最终是甜蜜！</p>"
            "<div align=right><p>2013年12月<br>付博文<br>船海专业201103班<br>船舶与海洋工程学院<br>华中科技大学</p></div>";
    QMessageBox::about(this,"关于作者",aboutF);
}

void MainWindow::on_action_drawHyCur_triggered()
{
    ship1->mode=2;
    ship1->updateGL();
}

void MainWindow::on_action_X_triggered()
{
    ship1->lmode=1;
    ship1->updateGL();
}
void MainWindow::on_action_Y_triggered()
{
    ship1->lmode=2;
    ship1->updateGL();
}
void MainWindow::on_action_Z_triggered()
{
    ship1->lmode=3;
    ship1->updateGL();
}

void MainWindow::on_action_exit_triggered()
{
    close();
}
