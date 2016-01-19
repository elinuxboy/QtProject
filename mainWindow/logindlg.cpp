#include "logindlg.h"
#include "ui_logindlg.h"

loginDlg::loginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDlg)
{
    ui->setupUi(this);

    myQtAPI::SetUTF8Code();//设置全局使用的是utf-8字符

    myQtAPI::FormOnlyCloseInCenter(this);//窗体居中，并且只有关闭按钮，不能调整大小

    UserInfoStruct userInfo;
    QVariant userInfoVar;
    userInfoVar.setValue(userInfo);
    qRegisterMetaType<QVariant>("QVariant"); //对QVariant进行注册，因为信号和槽的参数类型并不认识QVariant

    //从配置文件取数据
    OptionDataStruct tmpOptionData;
    tmpOptionData = iniConfig::loadOptionFrFile(false);
    qDebug() << "tmpOptionData.VerifiCodeUrl.isEmpty()" << tmpOptionData.VerifiCodeUrl.isEmpty();
    if(tmpOptionData.VerifiCodeUrl.isEmpty())
    {
        tmpOptionData = iniConfig::loadOptionFrFile(true);
    }

    tempUrl = tmpOptionData.webUrl;
    isUsedVerifiCode = tmpOptionData.isUsedVerifiCode;
    tmpVerifiCodeUrl = tmpOptionData.VerifiCodeUrl;
    qDebug() << "tempUrl" << tempUrl << "isUsedVerifiCode" << isUsedVerifiCode << "tmpVerifiCodeUrl" << tmpVerifiCodeUrl;
    isUsedVerifiCodeFun();//是否使用验证码

    manager = NULL;//将manager初始化为NULL
    reply = NULL;//将reply初始化为NULL

    //QNetworkAccessManager 类用于发送网络请求和接受回复，
    //具体的，它是用 QNetworkRequest 类来管理请求，
    //QNetworkReply 类进行接收回复，并对数据进行处理。
    manager = new QNetworkAccessManager(this); //新建 QNetworkAccessManager 对象

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽

    //使用label作为按钮，通过自定义鼠标事件，点击label所在区域实现鼠标单击
    connect(this, SIGNAL(signalVerifiCodeClicked()), this, SLOT(slotVerifiCodeClicked()));   //信号连接

    initLoginDlg();
}

loginDlg::~loginDlg()
{
    delete ui;
}

void loginDlg::initLoginDlg()
{
    ui->lineEditPwd->clear();
    ui->lineEditUserName->clear();
    ui->lineEditVerifiCode->clear();

    ui->lineEditUserName->setFocus();

    getUrlPic();//刷新验证码
}

//从用户登录对话框接收数据处理槽函数
void loginDlg::recvDataFrMainWin()
{
    qDbg("已收到退出登录Web服务器信号");

    initLoginDlg();

    //QUrl tmpUrl("http://192.168.1.222:8088/wlis3/j_acegi_logout.action");

    //从配置文件取数据
    OptionDataStruct tmpOptionData;
    tmpOptionData = iniConfig::loadOptionFrFile(false);
    qDebug() << "tmpOptionData.VerifiCodeUrl.isEmpty()" << tmpOptionData.VerifiCodeUrl.isEmpty();
    if(tmpOptionData.VerifiCodeUrl.isEmpty())
    {
        tmpOptionData = iniConfig::loadOptionFrFile(true);
    }

    tempUrl = tmpOptionData.webUrl;
    isUsedVerifiCode = tmpOptionData.isUsedVerifiCode;
    tmpVerifiCodeUrl = tmpOptionData.VerifiCodeUrl;
    qDebug() << "tempUrl" << tempUrl << "isUsedVerifiCode" << isUsedVerifiCode << "tmpVerifiCodeUrl" << tmpVerifiCodeUrl;

    QUrl urlTmp; //存储网络地址
    urlTmp = tempUrl;

    QFileInfo fileInfo(urlTmp.path());
    QString tmpStr;
    tmpStr.append(urlTmp.scheme());
    tmpStr.append("://");
    tmpStr.append(urlTmp.host());
    tmpStr.append(":");
    tmpStr.append(QString::number(urlTmp.port()));
    tmpStr.append(fileInfo.path());
    tmpStr.append("/j_acegi_logout.action");

    urlTmp = tmpStr;

    qDebug() << "Logout Url" << urlTmp;

    startHttpRequest(urlTmp);//进行请求链接
}

//是否使用验证码处理函数
void loginDlg::isUsedVerifiCodeFun()
{
    if(!isUsedVerifiCode)
    {//不使用验证码登陆
        ui->labelVerifiCode->hide();
        ui->lineEditVerifiCode->hide();
        ui->labelVerifiCodeShow->hide();
    }
    else
    {//使用验证码登陆
        ui->labelVerifiCode->show();
        ui->lineEditVerifiCode->show();
        ui->labelVerifiCodeShow->show();
    }
}

//请求完成处理函数
void loginDlg::replyFinished(QNetworkReply *reply) //当回复结束后
{
    if(NULL != reply)
    {
        //reply->deleteLater(); //最后要释放 reply 对象
        reply = NULL;
    }
}

//http请求链接函数
void loginDlg::startHttpRequest(QUrl url)
{
    reply = NULL;//将reply初始化为NULL

    //返回的是一个QNetworkReply 对象，这里我们获得这个对象，使用它完成显示数据下载进度的功能。
    reply = manager->get(QNetworkRequest(url));

    //当有可用数据时， reply 就会发出 readyRead()信号，我们这时就可以使用这些可用的数据。
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));//有可用数据时
}

//接收到http数据时的处理函数
void loginDlg::httpReadyRead() //有可用数据
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QTextCodec *codec = QTextCodec::codecForName("utf8");//使用 utf8 编码，这样才可以显示中文
        QString all = codec->toUnicode(reply->readAll());//使用 reply->readAll()函数就可以将下载的所有数据读出。
        //QString all = "{'success':'true','url':'/wlis3/do.jsp'}";
        //qDebug() << "all" << all;

        //ui->textBrowser->setText(all);//显示数据到textBrowser

        //判断登陆是否成功,服务器返回登陆成功数据：{'success':true,'url':'/wlis3/do.jsp'}
        //qDebug() << all.mid(11, 4);
        if(!QString::compare("true", all.mid(11, 4)))
        {//登陆成功，进入主界面
            qDebug() << "ui->lineEditUserName->text().trimmed()" << ui->lineEditUserName->text().trimmed();
            UserInfoStruct tmpUserInfo;
            QVariant tmpUserInfoVar;

            tmpUserInfo.curUserName = ui->lineEditUserName->text().trimmed();//当前登陆用户名
            tmpUserInfoVar.setValue(tmpUserInfo);           

            emit sendDataToMainWindow(tmpUserInfoVar);//发送信号给主窗口

            this->close();
        }
        else
        {
            QString errMsg = all.mid(27, all.length() - 29);
            //qDebug() << errMsg;
            //QMessageBox::warning(this, tr("警告"), errMsg, QMessageBox::Yes);//如果不正确，弹出警告对话框
            if(!QString::compare(tr("验证码不能为空！"), errMsg) ||
                    !QString::compare(tr("验证码输入错误！"), errMsg))
            {
                myQtAPI::ShowMessageBoxError(errMsg);//显示错误框，仅确定按钮
                //ui->lineEditVerificationCode->clear();//清空验证码输入框
                ui->lineEditVerifiCode->setFocus();//将光标转到验证码输入框
            }

            if(!QString::compare(tr("用户名或密码错误，请重试！"), errMsg))
            {
                myQtAPI::ShowMessageBoxError(errMsg);//显示错误框，仅确定按钮
                //ui->lineEditUserName->clear();//清空用户名输入框
                //ui->lineEditPwd->clear();//清空密码输入框
                ui->lineEditUserName->setFocus();//将光标转到用户名输入框
            }

            getUrlPic();//登陆失败后重新刷新验证码
        }//end if(!QString::compare("true", all.mid(11, 4)))
    }//end if(reply->error() == QNetworkReply::NoError)
}

//图片验证码请求链接函数
void loginDlg::startPicRequest(QUrl url)
{
    reply = NULL;//将reply初始化为NULL
    //返回的是一个QNetworkReply 对象，这里我们获得这个对象，使用它完成显示数据下载进度的功能。
    reply = manager->get(QNetworkRequest(url));

    //当有可用数据时， reply 就会发出 readyRead()信号，我们这时就可以使用这些可用的数据。
    connect(reply, SIGNAL(readyRead()), this, SLOT(picReadyRead()));//有可用数据时
}

//接收到图片数据时的处理函数
void loginDlg::picReadyRead() //有可用数据
{
    if(reply->error() == QNetworkReply::NoError)
    {
        //获取字节流构造 QPixmap 对象
        currentPicture.loadFromData(reply->readAll());
        QFileInfo info(url.path());//分解出文件名
        //qDebug() << "url.path" << url.path();
        QString fileName(info.fileName());//获取文件名
        //如果文件名为空，则使用“index.html” ，例如使用“http://www.yafeilinux.com” 时，文件名就为空
        if (fileName.isEmpty())
        {
           //fileName = "index.html";//因为地址中可能没有文件名，这时我们就使用一个默认的文件名。
           QDateTime now;
           fileName = now.currentDateTime().toString("yyMMddhhmmss");
        }

        fileName.append(".jpg");
        qDebug() << "fileName" << fileName;
        //QDateTime now;
        //QString filename = now.currentDateTime().toString("yyMMddhhmmss.jpg");
        currentPicture.save(fileName);//保存图片

        currentFileName = fileName;

        //可以在onpaint中 用QPixmap 显示currentPicture，则这种方法则不需要保存

        //显示图片
        QMovie *move = new QMovie(currentFileName);
        //label自动适应图片大小显示
        ui->labelVerifiCodeShow->resize(QPixmap(fileName).width(), QPixmap(fileName).height());
        ui->labelVerifiCodeShow->setMovie(move);
        move->start();
    }
}

void loginDlg::getUrlPic()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    //QString urlTmp = "http://192.168.1.254:8080/wlis3/ValidationCodeServlet?t=";
    QString urlTmp = tmpVerifiCodeUrl;
    urlTmp.append(time.toString());
    qDebug() << "VerifiCodeUrl" << urlTmp;
    url = QUrl(urlTmp);
    startPicRequest(url); //进行请求链接
}

//使用labelVerificationCode作为按钮，通过自定义鼠标事件，点击label所在区域实现鼠标单击
//验证码图片显示区鼠标处理事件
void loginDlg::mousePressEvent(QMouseEvent *e)
{
    //假如在labelVerificationCode这个区域里单击，就发出信号
    if (e->x() > ui->labelVerifiCodeShow->pos().x()
            && e->x() < (ui->labelVerifiCodeShow->width()
                         + ui->labelVerifiCodeShow->pos().x())
            && e->y() > ui->labelVerifiCodeShow->pos().y()
            && e->y() < (ui->labelVerifiCodeShow->height()
                         + ui->labelVerifiCodeShow->pos().y()))
    {
        emit signalVerifiCodeClicked();//发送验证码图片鼠标单击信号
    }
}

//单击验证码图片处理槽函数
void loginDlg::slotVerifiCodeClicked()
{
    getUrlPic();//刷新验证码
}

void loginDlg::on_pBtnLogin_clicked()
{
    if(isUsedVerifiCode)
    {
        tempUrl.clear();

        //从配置文件取数据
        OptionDataStruct tmpOptionData;
        tmpOptionData = iniConfig::loadOptionFrFile(false);
        qDebug() << "tmpOptionData.VerifiCodeUrl.isEmpty()" << tmpOptionData.VerifiCodeUrl.isEmpty();
        if(tmpOptionData.VerifiCodeUrl.isEmpty())
        {
            tmpOptionData = iniConfig::loadOptionFrFile(true);
        }

        tempUrl = tmpOptionData.webUrl;
        isUsedVerifiCode = tmpOptionData.isUsedVerifiCode;
        tmpVerifiCodeUrl = tmpOptionData.VerifiCodeUrl;
        qDebug() << "tempUrl" << tempUrl << "isUsedVerifiCode" << isUsedVerifiCode << "tmpVerifiCodeUrl" << tmpVerifiCodeUrl;

        //拼凑URL,加入用户名部分
        tempUrl.append("&j_username=");
        //加入的这个函数trimmed()的作用就是移除用户名字符串开头和结尾的空白字符。
        tempUrl.append(ui->lineEditUserName->text().trimmed());

        /**********加密开始***********/
        QByteArray temPwdMd5;
        QByteArray PwdMd5;
        QByteArray qByteArrayTem;
        QString tempStr;

        //pwd
        qByteArrayTem = QCryptographicHash::hash(ui->lineEditPwd->text().toLatin1(), QCryptographicHash::Md5 );//密码加密
        temPwdMd5.append(qByteArrayTem.toHex());
        //qDebug() << "密码MD5加密后的密文:" << qByteArrayTem.toHex();

        //验证码
        //qDebug() << "验证码明文:" << ui->lineEditVerificationCode->text();
        qByteArrayTem = QCryptographicHash::hash(ui->lineEditVerifiCode->text().toUpper().toLatin1(),QCryptographicHash::Md5);//验证码加密
        tempStr = "{";
        temPwdMd5.append(tempStr.toLatin1());
        temPwdMd5.append(qByteArrayTem.toHex());
        tempStr = "}";
        temPwdMd5.append(tempStr.toLatin1());
        //qDebug() << "验证码MD5加密后的密文:" << qByteArrayTem.toHex();

        //最后一次加密
        //qDebug() << "最后一次加密前的明文：" << temPwdMd5;
        PwdMd5 = QCryptographicHash::hash(temPwdMd5, QCryptographicHash::Md5);//对密码和验证码加密
        //qDebug()  << "MD5加密结束的密文:" << PwdMd5.toHex();
        /***********加密结束****************/

        //拼凑URL，加入密码部分
        tempUrl.append("&j_password=");
        tempUrl.append(PwdMd5.toHex());//MD5加密结束的密文
        //拼凑URL，加入验证码部分
        tempUrl.append("&j_validation_code=");
        tempUrl.append(ui->lineEditVerifiCode->text());//验证码明文
        //qDebug() << "tempUrl" << tempUrl;

        url = tempUrl;

        qDebug() << "url" << url;

        if(QString::compare(url.scheme(), "http"))
        {
            myQtAPI::ShowMessageBoxError("服务器地址错误，不是http协议！请重试。");
            //ui->textBrowser->setText("URL协议不是http协议！");
            //ui->lineEditUrl->setFocus();//将光标转到服务器地址输入框
            getUrlPic();//登陆失败后重新刷新验证码
            return;
        }

        startHttpRequest(url);//进行请求链接
    }
    else
    {
        myQtAPI::ShowMessageBoxInfo(tr("该功能在开发中"));
        this->close();
    }
}
