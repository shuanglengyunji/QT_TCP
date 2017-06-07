#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
   void sendMassage();
   void readMassage();
   void displayError(QAbstractSocket::SocketError);
   void connectUpdata();
   void disconnectUpdata();
   void on_sendButton_clicked();
   void on_clearButton_clicked();
   void on_connnectButton_clicked();
   void on_disconnectButton_clicked();

private:
   //QTcpServer *tcpServer;//不用再建立服务器类了，直接建立下面的套接字
   QTcpSocket *tcpSocket;//直接建立TCP套接字类
   QString tcpIp;//存储IP地址
   QString tcpPort;//存储端口地址
   bool flag;
   Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
