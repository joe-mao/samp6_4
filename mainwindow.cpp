#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qformdoc.h"
#include <QMdiSubWindow>
#include <QDir>
#include <QFileDialog>
#include <QMdiArea>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(this->ui->mdiArea);
    this->setWindowState(Qt::WindowMaximized);
    this->ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actDoc_New_triggered()
{
    //新建文档
    QFormDoc * formDoc = new QFormDoc(this);
    this->ui->mdiArea->addSubWindow(formDoc);//文档窗口添加到MDI
    formDoc->show();
}

void MainWindow::on_actDoc_Open_triggered()
{
    //打开文件
    bool needNew = false;//是否需要新建子窗口
    QFormDoc * formDoc;
    if(this->ui->mdiArea->subWindowList().count() > 0){
        //有活动的窗口,获取活动的窗口
        formDoc = static_cast<QFormDoc *>(this->ui->mdiArea->activeSubWindow()->widget());
        needNew = formDoc->isFileOpened();//文件已经打开，需要新建窗口(因为考虑到可能窗口中没有打开文件，就不需要新建窗口再打开文件了)
    }else{
        needNew = true;//如果没有活动的窗口，要打开一个文件就必须要新建一个窗口
    }
    QString curPath = QDir::currentPath();
    QString aFileName = QFileDialog::getOpenFileName(this, "打开一个文件", curPath, "C程序文件(*.h *.cpp);;所有文件(*.*)");
    if(aFileName.isEmpty()){
        return;
    }
    if(needNew){
        //需要新建子窗口
        formDoc = new QFormDoc(this);
        this->ui->mdiArea->addSubWindow(formDoc);
    }
    formDoc->loadFromFile(aFileName);//打开文件
    formDoc->show();
    this->ui->actCut->setEnabled(true);
    this->ui->actCopy->setEnabled(true);
    this->ui->actPaste->setEnabled(true);
    this->ui->actFont->setEnabled(true);

}

void MainWindow::on_actCascade_triggered()
{
    //窗口级联展开
    this->ui->mdiArea->cascadeSubWindows();
}


void MainWindow::on_actTile_triggered()
{
    //窗口平铺展开
    this->ui->mdiArea->tileSubWindows();
}

void MainWindow::on_actCloseAll_triggered()
{
    //关闭全部子窗口
    this->ui->mdiArea->closeAllSubWindows();
}


void MainWindow::on_actViewMode_triggered(bool checked)
{
    //MDI显示模式
    if(checked){
        //Tab多页显示模式
        this->ui->mdiArea->setViewMode(QMdiArea::TabbedView);//Tab多页显示模式
        this->ui->mdiArea->setTabsClosable(true);//页面可关闭
        this->ui->actCascade->setEnabled(false);
        this->ui->actTile->setEnabled(false);
    }else{
        //子窗口模式
        this->ui->mdiArea->setViewMode(QMdiArea::SubWindowView);//子窗口模式
        this->ui->actCascade->setEnabled(true);
        this->ui->actTile->setEnabled(true);
    }
}

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    //当前活动子窗口切换时
    QFormDoc * formDoc = nullptr;
    if(this->ui->mdiArea->subWindowList().count() == 0){
        //若子窗口个数为0
        this->ui->actCut->setEnabled(false);
        this->ui->actCopy->setEnabled(false);
        this->ui->actPaste->setEnabled(false);
        this->ui->actFont->setEnabled(false);
        this->ui->statusBar->clearMessage();
    }else{

        if(nullptr !=this->ui->mdiArea->activeSubWindow() ){
       formDoc = static_cast<QFormDoc *>(this->ui->mdiArea->activeSubWindow()->widget());
        this->ui->statusBar->showMessage(formDoc->currentFileName());
        }
    }
}

void MainWindow::on_actCut_triggered()
{
    //cut操作
    QFormDoc * formDoc = (QFormDoc *)this->ui->mdiArea->activeSubWindow()->widget();
    formDoc->textCut();
}

void MainWindow::on_actFont_triggered()
{
    //设置字体
    QFormDoc * formDoc = (QFormDoc *)this->ui->mdiArea->activeSubWindow()->widget();
    formDoc->setEditFont();
}
