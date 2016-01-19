#include "widget.h"
#include "ui_widget.h"

//调试信息
#define qDbg(format, ...) qDebug("[debug info] "format" File:%s, Line:%d, Function:%s", ##__VA_ARGS__, __FILE__, __LINE__ , __FUNCTION__);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //QNetworkAccessManager 类用于发送网
    //络请求和接受回复，具体的，它是用 QNetworkRequest 类来管理请求， QNetworkReply 类进行
    //接收回复，并对数据进行处理。
    manager = new QNetworkAccessManager(this); //新建 QNetworkAccessManager 对象

    connect(manager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));//关联信号和槽

    //只要发送请求成功，它就会下
    //载数据。而当数据下载完成后， manager 会发出 finished()信号，
    //它返回一个 QNetworkReply 对象，
    //manager->get(QNetworkRequest(QUrl("http://www.yafeilinux.com"))); //发送请求

    getUrlPic();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::replyFinished(QNetworkReply *reply) //当回复结束后
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");//使用 utf8 编码，这样才可以显示中文

    QString all = codec->toUnicode(reply->readAll());//使用 reply->readAll()函数就可以将下载的所有数据读出。

    ui->textBrowser->setText(all);//在 textBrowser 中将数据显示出来。

    reply->deleteLater(); //最后要释放 reply 对象
    reply = NULL;
}

//链接请求函数
void Widget::startRequest(QUrl url) //链接请求
{
    qDebug() << "url" << url;
    //返回的是一个QNetworkReply 对象，这里我们获得这个对象，使用它完成显示数据下载进度的功能。
    reply = manager->get(QNetworkRequest(url));
    //当有可用数据时， reply 就会发出 readyRead()信号，我们这时就可以使用这些可用的数据。
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));//有可用数据时
}

//保存数据函数
void Widget::httpReadyRead() //有可用数据
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QFileInfo info(url.path());//分解出文件名
        qDebug() << "url.path" << url.path();
        QString fileName(info.fileName());//获取文件名
        //如果文件名为空，则使用“index.html” ，例如使用“http://www.yafeilinux.com” 时，文件名就为空
        if (fileName.isEmpty())
        {
           fileName = "index.html";//因为地址中可能没有文件名，这时我们就使用一个默认的文件名。
           QDateTime now;
           QString fileName = now.currentDateTime().toString("yyMMddhhmmss");
        }

        fileName.append(".jpg");
        qDebug() << "fileName" << fileName;

        qDbg();
        //获取字节流构造 QPixmap 对象
        currentPicture->loadFromData(reply->readAll());
        qDbg();
        currentPicture->save(fileName);//保存图片
        qDbg();
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

//获取网络图片
void Widget::getUrlPic()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString urlTmp = "http://192.168.1.222:8088/wlis3/ValidationCodeServlet?t=";
    urlTmp.append(time.toString());
    qDebug() << "urlTmp" << urlTmp;
    url = QUrl(urlTmp);
    startRequest(url); //进行链接请求
}

void Widget::on_pBtnRefresh_clicked()
{
    getUrlPic();
}

void Widget::on_pBtnGet_clicked()
{
    //QUrl url(ui->lineEditUrl->text());
    QString tempUrl = ui->lineEditUrl->text();
    tempUrl.append("&j_username=");
    tempUrl.append(ui->lineEditUserName->text());
    //url.setPort(ui->lineEditPort->text().toInt());//设置端口
    //url.setUserName(ui->lineEditUserName->text());//设置用户名

    /**********加密开始***********/
    QByteArray temPwdMd5;
    QByteArray PwdMd5;
    QByteArray qByteArrayTem;
    QString tempStr;

    //pwd
    qByteArrayTem = QCryptographicHash::hash(ui->lineEditPwd->text().toAscii(), QCryptographicHash::Md5 );//密码加密
    temPwdMd5.append(qByteArrayTem.toHex());
    qDebug() << "密码MD5加密后的密文:" << qByteArrayTem.toHex();

    qDebug() << "验证码明文:" << ui->lineEditVerficationCode->text();
    //验证码
    qByteArrayTem = QCryptographicHash::hash(ui->lineEditVerficationCode->text().toUpper().toAscii(),QCryptographicHash::Md5);//验证码加密
    tempStr = "{";
    temPwdMd5.append(tempStr.toAscii());
    temPwdMd5.append(qByteArrayTem.toHex());
    tempStr = "}";
    temPwdMd5.append(tempStr.toAscii());
    qDebug() << "验证码MD5加密后的密文:" << qByteArrayTem.toHex();
    qDebug() << "最后一次加密前的明文：" << temPwdMd5;

    PwdMd5 = QCryptographicHash::hash(temPwdMd5, QCryptographicHash::Md5);//对密码和验证码加密
    qDebug()  << "MD5加密结束的密文:" << PwdMd5.toHex();
    /***********加密结束****************/

    tempUrl.append("&j_password=");
    tempUrl.append(PwdMd5.toHex());//MD5加密结束的密文
    tempUrl.append("&j_validation_code=");
    tempUrl.append(ui->lineEditVerficationCode->text());//验证码明文

    qDebug() << "tempUrl" << tempUrl;
    url = tempUrl;
    qDebug() << url;

    if(QString::compare(url.scheme(), "http"))
    {
        qDebug() << "URL协议不是http协议！";
        return;
    }

    if(url.host().isEmpty())
    {
        qDebug() << "url.host().isEmpty()！" << url.host().isEmpty();
        return;
    }

   reply = manager->get(QNetworkRequest(url));//返回的是一个QNetworkReply 对象，这里我们获得这个对象，使用它完成显示数据下载进度的功能。
}

void Widget::on_pBtnPost_clicked()
{

}

void Widget::on_pBtnClear_clicked()
{
    ui->textBrowser->clear();
}
