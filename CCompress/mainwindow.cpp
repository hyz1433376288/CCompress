#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QtDebug>
#include <QDebug>
#include <QProgressDialog>
#define W 640
#define H 470
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(W,H);
    this->setMinimumSize(W,H);
    this->setWindowTitle("BandYz");

    QPixmap cmpBtIco("D:\\Qt\\myQt\\demo1\\CCompress\\img\\compress_logo.png");
    QPixmap decmpBtIco("D:\\Qt\\myQt\\demo1\\CCompress\\img\\decompress_logo.png");
    QPixmap startBtIco("D:\\Qt\\myQt\\demo1\\CCompress\\img\\start.png");
    QPixmap windowsIco("D:\\Qt\\myQt\\demo1\\CCompress\\img\\logo.png");


    ui->compressButton->setIconSize(QSize(100,100));
    ui->compressButton->setIcon(cmpBtIco);
    ui->decompressButton->setIconSize(QSize(100,100));
    ui->decompressButton->setIcon(decmpBtIco);
    ui->startButton->setIconSize(QSize(60,60));
    ui->startButton->setIcon(startBtIco);
    setWindowIcon(QIcon("D:\\Qt\\myQt\\demo1\\CCompress\\img\\logo.ico"));
    //记录当前状态，NONE表示无状态，CP表示压缩，DP表示解压缩
    CState = "NONE";
    srcFilePath.clear();
    connect(&cp,SIGNAL(readyReadStandardOutput()),this,SLOT(onReadData()));
    appendInfo(QDir::currentPath());


}
MainWindow::~MainWindow()
{
    delete ui;
}
char* MainWindow::QstringToChar(QString qst)
{
    return qst.toLatin1().data();
}
void MainWindow::appendInfo(QString msg)
{
    ui->InfoText->append(msg+"\n");
}


QString MainWindow::getFileDictionary(QString path){

}

void MainWindow::on_compressButton_clicked()
{
    ui->desLabel->setText("压缩至");
    CState = "CP";

    srcFilePath = QFileDialog::getOpenFileName(this);
    if(!srcFilePath.isEmpty()){
        desFilePath = srcFilePath+".yz";
    }
    ui->srcLineEdit->setText(srcFilePath);
    ui->desLineEdit->setText(desFilePath);

}

void MainWindow::on_decompressButton_clicked()
{
    ui->desLabel->setText("解压至");
    CState = "DP";

    srcFilePath = QFileDialog::getOpenFileName(this);
    desFilePath = srcFilePath.left(srcFilePath.length()-3);
    desFilePath.replace(".","De.");
    ui->srcLineEdit->setText(srcFilePath);
    ui->desLineEdit->setText(desFilePath);


}

void MainWindow::onReadData(){
     // qDebug() << cp.readAllStandardOutput();
      appendInfo(cp.readAllStandardOutput());
}

void MainWindow::on_startButton_clicked(){
    //查看yz.exe是否存在
    QFile file(QDir::currentPath()+"\\yz.exe");
    if(!file.exists())
    {
        QMessageBox::information(this,"提示","请将文件\"yz.exe\"放置于软件的同一目录下");
        return;
    }
    //必须读取控件的路径，因为用户可能改变了路径
    srcFilePath = ui->srcLineEdit->text();
    desFilePath = ui->desLineEdit->text();
    if(srcFilePath.isEmpty() || desFilePath.isEmpty()){
        QMessageBox::information(this,"提示","路径不可为空");
        return;
    }
    QFile file1(srcFilePath);
    if(!file1.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this,"提示","源文件打开失败");
        return;
    }

    //开始压缩or解压

    cp.setReadChannel(QProcess::StandardOutput);
    //调用yz进程
    QString cmd = QDir::currentPath();

    if(CState == "CP"){
        cmd = cmd + "\\yz -e "+srcFilePath+" "+desFilePath;
        //cp.start("yz -e ");
    }
    else if(CState == "DP"){
        cmd = cmd +  "\\yz -d "+srcFilePath+" "+desFilePath;
    }
    else if(CState == "NONE"){
        QMessageBox::information(this,"提示","请选择‘压缩’或‘解压’");
        return ;
    }
    else {
       QMessageBox::information(this,"提示","请等待当前线程结束");
        return ;
    }
    //appendInfo(cmd);

    //禁用start按钮
    ui->startButton->setFocusPolicy(Qt::NoFocus);
    cp.start(cmd);

    char dialogInfo[100];
    if(CState=="CP"){
        strcpy(dialogInfo,"正在压缩");
    }
    else{
        strcpy(dialogInfo,"正在解压");
    }
    QProgressDialog dialog(tr(dialogInfo),tr("取消"),0,50000,this);
    dialog.setWindowTitle(tr("YZ"));

    dialog.setWindowModality(Qt::WindowModal);
    dialog.show();
    for(int i=0;i<=30000;i++)
    {
        dialog.setValue(i);
        //QCoreApplication::processEvents();//防止界面冻结
        if(dialog.wasCanceled()) break; //取消就中断
    }

    ui->startButton->setFocusPolicy(Qt::ClickFocus);

    //解除Cstate

}


void MainWindow::on_InfoText_textChanged()
{
    ui->InfoText->moveCursor(QTextCursor::End);
}
