#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QNetworkAccessManager 类用于发送网络请求和接受回复，
    //具体的，它是用 QNetworkRequest 类来管理请求，
    //QNetworkReply 类进行接收回复，并对数据进行处理。
    manager = new QNetworkAccessManager(this); //新建 QNetworkAccessManager 对象

    //url = "http://192.168.1.254:8080/wlis3/ValidationCodeServlet?t=dddd";
    getUrlPic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//链接请求函数
void MainWindow::startRequest(QUrl url)
{
    qDebug() << url;
    //返回的是一个QNetworkReply 对象，这里我们获得这个对象，使用它完成显示数据下载进度的功能。
    reply = manager->get(QNetworkRequest(url));
    //当有可用数据时， reply 就会发出 readyRead()信号，我们这时就可以使用这些可用的数据。
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));//有可用数据时
}

//请求完成处理函数
void MainWindow::replyFinished(QNetworkReply *reply) //当回复结束后
{
    reply->deleteLater(); //最后要释放 reply 对象
    reply = NULL;
}

//使用数据函数
void MainWindow::httpReadyRead() //有可用数据
{
    if(reply->error() == QNetworkReply::NoError)
    {
        //获取字节流构造 QPixmap 对象
        currentPicture->loadFromData(reply->readAll());
        QFileInfo info(url.path());//分解出文件名
        qDebug() << "url.path" << url.path();
        QString fileName(info.fileName());//获取文件名
        //如果文件名为空，则使用“index.html” ，例如使用“http://www.yafeilinux.com” 时，文件名就为空
        if (fileName.isEmpty())
        {
           fileName = "index.html";//因为地址中可能没有文件名，这时我们就使用一个默认的文件名。
        }
        fileName.append(".jpg");
        qDebug() << "fileName" << fileName;
        //QDateTime now;
        //QString filename = now.currentDateTime().toString("yyMMddhhmmss.jpg");
        currentPicture->save(fileName);//保存图片
        //qDebug()<<"picture saved as "<<filename;
        currentFileName = fileName;

        //可以在onpaint中 用QPixmap 显示currentPicture，则这种方法则不需要保存

        //显示图片，
        QMovie *move = new QMovie(currentFileName);
        //label自动适应图片大小显示
        ui->labelVerificationCode->resize(QPixmap(fileName).width(), QPixmap(fileName).height());
        ui->labelVerificationCode->setMovie(move);
        move->start();
    }
}

//获取网页图片函数
void MainWindow::getUrlPic()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString urlTmp = "http://192.168.1.222:8088/wlis3/ValidationCodeServlet?t=";
    urlTmp.append(time.toString());
    qDebug() << "urlTmp" << urlTmp;
    url = QUrl(urlTmp);
    startRequest(url); //进行链接请求
}

void MainWindow::on_pBtnRefresh_clicked()
{
    getUrlPic();
}

void MainWindow::on_pBtnGet_clicked()
{

}

void MainWindow::on_pBtnPost_clicked()
{

}

void MainWindow::on_pBtnClear_clicked()
{

}
