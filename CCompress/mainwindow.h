#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CompressFile();
private:
    char* QstringToChar(QString qst);
    void appendInfo(QString info);
    QString getFileDictionary(QString path);

private slots:
    void on_compressButton_clicked();

    void on_decompressButton_clicked();


    void on_startButton_clicked();
    void onReadData();

    void on_InfoText_textChanged();

private:
    Ui::MainWindow *ui;
    //记录当前状态，NONE表示默认状态，CP表示压缩，DP表示解压缩,DO表示正在进行cp或dp
    QString CState;
    //记录文件路径
    QString srcFilePath;
    QString desFilePath;
    QProcess cp;

};

#endif // MAINWINDOW_H
