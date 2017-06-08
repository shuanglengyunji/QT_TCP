#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);

    //输入初始IP和端口
    ui->IPLineEdit->setText("192.168.3.4");
    ui->portLineEdit->setText("4001");

    //TCP套接字类指针
    tcpSocket = NULL;//使用前先清空
}

MainWindow::~MainWindow()
{
    delete tcpSocket;   //清空tcpSocket指针
    delete ui;
}

//发送数据
void MainWindow::sendMassage(){}

//读取数据
void MainWindow::readMassage()
{
    QByteArray data=tcpSocket->readAll();       //读取数据
    ui->clearLineEdit->setText(QString(data));  //输出
}

//处理错误
void MainWindow::displayError(QAbstractSocket::SocketError)
{
    QMessageBox::warning (this, tr("Warnning"), tcpSocket->errorString ());
    tcpSocket->close ();
    ui->connnectButton->setEnabled (true);
    ui->disconnectButton->setEnabled (false);
    ui->sendButton->setEnabled (false);
}

//发送按钮
void MainWindow::on_sendButton_clicked()
{
    QString sendmessage;
    sendmessage = ui->sendLineEdit->text();
   /* if(sendmessage == NULL) return;
    QByteArray block;//暂时存储我们需要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);//TCP必须和数据流一起使用
    out.setVersion(QDataStream::Qt_5_7);//设置数据流的版本（服务器和主机版本一定相同）
    out << sendmessage;
    tcpSocket->write(block);*/
    QByteArray data;
    data.append(sendmessage);
    tcpSocket->write(data);
}

//清空接收窗口
void MainWindow::on_clearButton_clicked()
{
    ui->clearLineEdit->clear();
}

//连接按钮
void MainWindow::on_connnectButton_clicked()
{
    flag = false;

    //创建TCP类
    if(tcpSocket) delete tcpSocket;//如果有指向其他空间直接删除
    tcpSocket = new QTcpSocket(this);//申请堆空间有TCP发送和接受操作

    //获取IP和Port字符串并判断是否可用
    tcpIp = ui->IPLineEdit->text();
    tcpPort = ui->portLineEdit->text();
    if(tcpIp==NULL||tcpPort==NULL)//判断IP和PORT是否为空
    {
        QMessageBox msgBox;
        msgBox.setText("IP or PORT is Empty");
        msgBox.exec();
        return;
    }

    //根据tcpIp连接主机
    tcpSocket->connectToHost(tcpIp,tcpPort.toInt());//连接主机

    //建立槽

    //错误信号
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    //连接成功信号
    //connected()信号是在成功建立同host的连接后发出的
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectUpdata()));

    //读取信息信号
    //收到信息时发出readyRead()信号
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMassage()));

    //按钮状态改变（放在连接成功后）
//    ui->connnectButton->setEnabled (false);     //连接按钮失能
//    ui->disconnectButton->setEnabled (true);    //端口连接按钮使能

}

//断开连接按钮
void MainWindow::on_disconnectButton_clicked()
{
    tcpSocket->abort();
    tcpSocket->deleteLater();   //这个删除方式更温和一些
//    delete tcpSocket;
    tcpSocket=NULL;
    disconnectUpdata();
}

//连接成功后通过槽调用此函数，处理按钮状态的改变
void MainWindow::connectUpdata()
{
    if(!flag)
    {
//        QMessageBox msgBox;
//        msgBox.setText("TCP connect successful");
//        msgBox.exec();                          //窗口关闭前阻塞程序运行
        ui->connnectButton->setEnabled(false);  //连接
        ui->sendButton->setEnabled(true);       //发送按钮
        ui->disconnectButton->setEnabled(true); //端口连接按钮
        ui->IPLineEdit->setEnabled(false);      //地址框编辑禁止
        ui->portLineEdit->setEnabled(false);    //端口号编辑禁止
    }
    flag=true;
}

//断开连接函数
void MainWindow::disconnectUpdata()
{
    ui->connnectButton->setEnabled(true);
    ui->sendButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->IPLineEdit->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
}

