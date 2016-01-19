#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myQtAPI::SetUTF8Code();//设置全局使用的是utf-8字符

    mainWinInit();//主界面初始化

    //重启信号连接
    connect(&dlgOption, SIGNAL(signalRebotToMainWin()), this, SLOT(slotReboot()) );

    //用于窗口之间传递参数的信号连接
    connect(&dlgLogin, SIGNAL(sendDataToMainWindow(QVariant)), this, SLOT(recvDataFrLoginDlg(QVariant)));

    //用于窗口之间传递参数的信号连接
    connect(this, SIGNAL(sendDataToLogDlg()), &dlgLogin, SLOT(recvDataFrMainWin()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//重启槽函数
void MainWindow::slotReboot()
{
    qDebug() << "Performing application reboot..";
    if(myQtAPI::ShowMessageBoxQuesion(tr("是否现在重启系统？")))
    {
        return;
    }
    //this->close();
    myQtAPI::Sleep(1000);//延时1秒
    qApp->exit( EXIT_CODE_REBOOT );//调用exit()，退出应用程序。
    qDebug() << "..........";
}

//有时候我们希望在关闭窗口之前做一些操作，例如保存缓存数据或向用户提示是否关闭窗口等等。
//由于一般的窗口都是继承自QWidget, 那么我们可以通过重载QWidget中的虚函数closeEvent(QCloseEvent* event);来达到这个目的。
void MainWindow::closeEvent(QCloseEvent *event)
{
     //TODO: 在退出窗口之前，实现希望做的操作
    if(!myQtAPI::ShowMessageBoxQuesion(tr("您正在退出系统\n\n是否退出？")))
    {
        event->accept();//accept就是让这个关闭事件通过并顺利关闭窗口
        this->close();
    }
}

//从用户登录对话框接收数据处理槽函数
void MainWindow::recvDataFrLoginDlg(QVariant dataVar)
{
    UserInfoStruct tmpUserInfo;
    tmpUserInfo = dataVar.value<UserInfoStruct>();

    qDebug() << "tmpUserInfo.curUserName" << tmpUserInfo.curUserName;

    ui->labelCurrentUser->setText(tr("当前已登录用户：") + tmpUserInfo.curUserName);//将当前登陆的用户名显示在主界面

    ui->pBtnPostWebFrDev->setEnabled(true);
    ui->pBtnPostWebFrFtp->setEnabled(true);
    ui->pBtnPostWebFrLocal->setEnabled(true);

    ui->pBtnLoginWeb->setText(tr("退出Web服务器"));
}

//主界面初始化函数
void MainWindow::mainWinInit()
{
    ui->tabWidget->setCurrentIndex(0);//初始化为打开第一个选项卡

    tabFrLocalInit();//本地选项卡初始化
    tabFrFtpInit();//FTP选项卡初始化
    tabFrDevInit();//串口设备选项卡初始化
}

//本地选项卡初始化
void MainWindow::tabFrLocalInit()
{
    //清除
    ui->textBrlFileShowFrLocal->clear();
    ui->tbWidgetExcelShowFrLocal->clear();
    ui->textBrJsonFrLocal->clear();

    //隐藏
    ui->textBrlFileShowFrLocal->hide();
    ui->tbWidgetExcelShowFrLocal->hide();

    //禁用
    ui->gBoxFileShowFrLocal->setEnabled(false);
    ui->gBoxJsonShowFrLocal->setEnabled(false);
    ui->pBtnPostWebFrLocal->setEnabled(false);
}

//FTP选项卡初始化
void MainWindow::tabFrFtpInit()
{
    //清空
    ui->lineEditFtpHost->clear();
    ui->lineEditFtpPort->clear();
    ui->lineEditFtpUserName->clear();
    ui->lineEditFtpPwd->clear();
    ui->trWidgetFileListFrFtp->clear();
    ui->textBrFileShowFrFtp->clear();
    ui->tbWidgetExcelShowFrFtp->clear();
    ui->textBrJsonShowFrFtp->clear();

    //隐藏
    ui->textBrFileShowFrFtp->hide();
    ui->textBrJsonShowFrFtp->hide();
    ui->gBoxFileShowFrFtp->hide();
    ui->gBoxJsonShowFrFtp->hide();

    //禁用
    ui->trWidgetFileListFrFtp->setEnabled(false);
    ui->pBtnPostWebFrFtp->setEnabled(false);
    ui->pBtnFtpDownload->setEnabled(false);
    ui->pBtnFtpUpload->setEnabled(false);
    ui->pBtnFtpCdToParent->setEnabled(false);
}

//串口设备选项卡初始化
void MainWindow::tabFrDevInit()
{
    InitComForm();//串口初始化
    ui->textBrJsonShowFrDev->clear();
    ui->pBtnPostWebFrDev->setEnabled(false);
}

//串口初始化
void MainWindow::InitComForm()
{
    QStringList comList;//串口号
    QStringList baudList;//波特率
    QStringList parityList;//校验位
    QStringList dataBitsList;//数据位
    QStringList stopBitsList;//停止位

    ReceiveCount = 0;
    SendCount =0;
    IsShow = true;
    IsAutoClear = false;
    IsHexSend = true;
    IsHexReceive = true;
    IsDebug = false;

#ifdef Q_OS_WIN//如果是windows系统
    comList<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"
          <<"COM7"<<"COM8"<<"COM9"<<"COM10"<<"COM11"<<"COM12"
         <<"COM13"<<"COM14"<<"COM15";
#else//如果是unix或者其他系统
    comList<<"ttyUSB0"<<"ttyUSB1"<<"ttyUSB2"<<"ttyUSB3"<<"ttyUSB4"<<"ttyUSB5"
          <<"ttyS0"<<"ttyS1"<<"ttyS2"<<"ttyS3"<<"ttyS4"<<"ttyS5"<<"ttyS6"
         <<"ttyS7"<<"ttyS8"<<"ttyS9";
#endif
    ui->cBoxComPort->addItems(comList);
    ui->cBoxComPort->setCurrentIndex(0);

    baudList<<"50"<<"75"<<"100"<<"134"<<"150"<<"200"<<"300"
           <<"600"<<"1200"<<"1800"<<"2400"<<"4800"<<"9600"
          <<"14400"<<"19200"<<"38400"<<"56000"<<"57600"
         <<"76800"<<"115200"<<"128000"<<"256000";
    ui->cBoxBaudRate->addItems(baudList);
    ui->cBoxBaudRate->setCurrentIndex(12);

    parityList<<"无"<<"奇"<<"偶";
#ifdef Q_OS_WIN//如果是windows系统
    parityList<<"标志";
#endif
    parityList<<"空格";
    ui->cBoxParity->addItems(parityList);
    ui->cBoxParity->setCurrentIndex(0);

    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->cBoxDataBit->addItems(dataBitsList);
    ui->cBoxDataBit->setCurrentIndex(3);

    stopBitsList<<"1";
#ifdef Q_OS_WIN//如果是windows系统
    stopBitsList<<"1.5";
#endif
    stopBitsList<<"2";
    ui->cBoxStopBit->addItems(stopBitsList);
    ui->cBoxStopBit->setCurrentIndex(0);

    //读取数据(采用定时器读取数据，不采用事件，方便移植到linux)
    myReadTimer = new QTimer(this);
    myReadTimer->setInterval(300);
    connect(myReadTimer, SIGNAL(timeout()),this, SLOT(ReadMyCom()));

    //发送数据
    mySendTimer = new QTimer(this);
    mySendTimer->setInterval(5000);
    connect(mySendTimer, SIGNAL(timeout()), this, SLOT(WriteMyCom()));

    for(int i = 1;i <= 60; i++)
    {
        ui->cBoxAutoSendTime->addItem(QString::number(i) + tr("秒"));
    }
    ui->cBoxAutoSendTime->setCurrentIndex(4);
    ui->cBoxAutoSendTime->setEnabled(false);

    ChangeEnableToCom(false);

    ui->textEditDataShowFrDev->installEventFilter(this);//安装监听器监听发送数据框回车响应
}

//串口使能
void MainWindow::ChangeEnableToCom(bool b)
{
    ui->cBoxBaudRate->setEnabled(!b);
    ui->cBoxDataBit->setEnabled(!b);
    ui->cBoxParity->setEnabled(!b);
    ui->cBoxComPort->setEnabled(!b);
    ui->cBoxStopBit->setEnabled(!b);

    ui->gBoxDataShowFrDev->setEnabled(b);
    ui->gBoxDevDataSend->setEnabled(b);
    ui->gBoxJsonShowFrDev->setEnabled(b);
}

//从串口读取数据
void MainWindow::ReadMyCom()
{
    //这个判断尤为重要,否则的话直接延时再接收数据,空闲时和会出现高内存占用
    if (myCom->bytesAvailable() <= 0)
    {
        return;
    }

    myQtAPI::Sleep(100);//延时100毫秒保证接收到的是一条完整的数据,而不是脱节的
    QByteArray buffer = myCom->readAll();

    if(IsShow)
    {
        if (IsHexReceive)
        {
            QString tempDataHex = myQtAPI::ByteArrayToHexStr(buffer);
            ui->textEditDataShowFrDev->append((tr("接收:%1 时间:%2"))
                                   .arg(tempDataHex)
                                   .arg(QTime::currentTime().toString("HH:mm:ss")));

            if (IsDebug)//2013-8-6增加接收数据后转发数据，模拟设备
            {
                foreach(QString tempData, SendDataList)
                {
                    QStringList temp = tempData.split(';');
                    if (tempDataHex == temp[0])
                    {
                        //这里没有跳出循环，有可能一条数据会对应多条数据需要转发
                        myCom->write(myQtAPI::HexStrToByteArray(temp[1]));
                    }
                }
            }
        }
        else
        {
            QString tempDataNormal = QString(buffer);
            ui->textEditDataShowFrDev->append(tr("接收:%1 时间:%2")
                                   .arg(tempDataNormal)
                                   .arg(QTime::currentTime().toString("HH:mm:ss")));

            if (IsDebug)//2013-8-6增加接收数据后转发数据，模拟设备
            {
                foreach(QString tempData, SendDataList)
                {
                    QStringList temp = tempData.split(';');
                    if (tempDataNormal == temp[0])
                    {
                        //这里没有跳出循环，有可能一条数据会对应多条数据需要转发
                        myCom->write(temp[1].toLatin1());
                    }
                }
            }
        }

        ReceiveCount = ReceiveCount + buffer.size();
        ui->labelRecvCountFrDev->setText(tr("接收:%1 字节").arg(ReceiveCount));
    }
}

//写入数据到串口
void MainWindow::WriteMyCom()
{
    QString str = ui->cBoxDevSendData->currentText();
    if (str == "")
    {//发送数据为空
        ui->cBoxDevSendData->setFocus();
        return;
    }

    if (!myCom->isOpen())
    {//串口没有打开
        return;
    }

    QByteArray outData = str.toLatin1();
    int size = outData.size();

    if (IsHexSend)//转化为16进制发送
    {
        outData = myQtAPI::HexStrToByteArray(str);
        size = outData.size();
        myCom->write(outData);
    }
    else
    {
        size = outData.size();
        myCom->write(outData);
    }

    ui->textEditDataShowFrDev->append(tr("发送:%1 时间:%2")
                           .arg(str)
                           .arg(QTime::currentTime().toString("HH:mm:ss")));

    SendCount = SendCount + size;
    ui->labelSendCountFrDev->setText(tr("发送:%1 字节").arg(SendCount));

    if (IsAutoClear)
    {
        ui->cBoxDevSendData->setCurrentIndex(-1);
        ui->cBoxDevSendData->setFocus();
    }
}

void MainWindow::on_pBtnOption_clicked()
{
    if(dlgOption.exec() == QDialog::Accepted)
    {
        dlgOption.showNormal();//正常显示选项设置对话框
    }
}

void MainWindow::on_pBtnLoginWeb_clicked()
{
    dlgLogin.getUrlPic();//刷新验证码

    if(ui->pBtnLoginWeb->text() == tr("登陆Web服务器"))
    {
        if(dlgLogin.exec() == QDialog::Accepted)
        {
            dlgLogin.showNormal();//正常显示登陆对话框
        }
    }
    else
    {
        ui->pBtnLoginWeb->setText(tr("登陆Web服务器"));
        ui->labelCurrentUser->clear();

        ui->pBtnPostWebFrDev->setEnabled(false);
        ui->pBtnPostWebFrFtp->setEnabled(false);
        ui->pBtnPostWebFrLocal->setEnabled(false);

        ui->labelCurrentUser->setText(tr("注意：没登录Web服务器时无法与Web服务器通讯的相关功能！"));

        //dlgLogin.getUrlPic();//刷新验证码

        emit sendDataToLogDlg();//发送退出登录信号给登陆对话框
    }

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    ui->statusBar->showMessage(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
}

void MainWindow::on_pBtnOpenFrLocal_clicked()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    Json::FastWriter writer;

    ui->statusBar->showMessage(tr("正在运行，请稍后···"));

    //选择文件对话框
    QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "所有文件(*.*)");
    if (!filename.isNull())
    { //用户选择了文件
        ui->tbWidgetExcelShowFrLocal->clear();
        ui->tbWidgetExcelShowFrLocal->setColumnCount(0);
        ui->tbWidgetExcelShowFrLocal->setRowCount(0);
        ui->textBrlFileShowFrLocal->clear();
        ui->textBrJsonFrLocal->clear();
        ui->labelPathFileFrLocal->clear();

        // 处理文件
        ui->labelPathFileFrLocal->setText(filename);//将路径导入到label中
        QByteArray str(filename.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
        QFileInfo fileInfo(str);
        QString ext = fileInfo.suffix();//获取文件后缀名

        if(!(QString::compare(ext, "xls")) || !(QString::compare(ext, "xlsx")))
        {//Excel文件
            ui->textBrlFileShowFrLocal->hide();
            ui->tbWidgetExcelShowFrLocal->show();
            ui->textBrJsonFrLocal->show();

            ui->gBoxFileShowFrLocal->setEnabled(true);
            ui->gBoxJsonShowFrLocal->setEnabled(true);

            ExcelEngine excel(filename);//创建excl对象

            //打开指定的xls文件
            if(!excel.Open())
            {
                myQtAPI::ShowMessageBoxError(tr("文件%1打开失败，请重试！").arg(filename));
            }
            else
            {
                ui->statusBar->showMessage(filename);
                ui->labelPathFileFrLocal->setText(filename);//将路径导入到label中
            }

            excel.ReadDataToTable(ui->tbWidgetExcelShowFrLocal);//导入excel中的所有数据到tablewidget中并显示

            gRowCount = excel.GetRowCount();//获取行数
            gColumnCount = excel.GetColumnCount();//获取列数
    #if 0
            for(uint column = 1; column <= gColumnCount; column++)
            {//列
                for(uint row = 2; row <= gRowCount; row++)
                {//行
                    QVariant keyJson = excel.GetCellData(1, column); //访问第一行的单元格数据
                    //QString key = fromUtf8(keyJson.toString());                //qDbg("row=%d, column=%d", row, column);
                    QVariant valueJson = excel.GetCellData(row, column);//访问指定单元格数据
                    root[keyJson.toString().toStdString()].append(valueJson.toString().toStdString()); //访问指定单元格数据
                 }
            }
    #else
            for(uint row = 1; row <= gRowCount; row++)
            {//行
                for(uint column = 1; column <= gColumnCount; column++)
                {//列
                    QVariant valueJson = excel.GetCellData(row, column);//访问指定单元格数据
                    QString key = QString::number(row, 10);
                    root[key.toStdString()].append(valueJson.toString().toStdString()); //访问指定单元格数据
                 }
            }
    #endif
            //Json::FastWriter fast_writer;
            //ui->textBrowser->append(QString::fromStdString(fast_writer.write(root)));//导出数据到textbrowser

            Json::StyledWriter styled_writer;//格式化
            ui->textBrJsonFrLocal->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser

            excel.Close();//关闭excel文件
        }
        else
        {//非Excel文件
            ui->tbWidgetExcelShowFrLocal->hide();
            ui->textBrlFileShowFrLocal->show();
            ui->textBrJsonFrLocal->show();

            ui->gBoxFileShowFrLocal->setEnabled(true);
            ui->gBoxJsonShowFrLocal->setEnabled(true);

            QFile qFile(str);
            if(!qFile.open(QIODevice::ReadOnly))
            {
                myQtAPI::ShowMessageBoxError(tr("文件%1打开失败，请重试！").arg(filename));
            }
            else
            {
                ui->statusBar->showMessage(filename);
                ui->labelPathFileFrLocal->setText(filename);//将路径导入到label中
            }

            gLineNum = 0;

            while(!qFile.atEnd())
            {
                gLineNum++;
                QByteArray lineData = qFile.readLine();//按行读取
                QString strLine(lineData);
                ui->textBrlFileShowFrLocal->append(strLine);//追加数据到textbrowser
                uint spaceNum = 0;//空格回车数
                QString noSpaceData;//非空格非回车数据
                for(int i = 0; i < lineData.length(); i++)
                {//一行数据里循环取一个字符
                    QString charPreLine = strLine.mid(i, 1);//依次取1个字符
                    if(!QString::compare(charPreLine, " ") ||
                            !QString::compare(charPreLine, "\r") ||
                            !QString::compare(charPreLine, "\n"))
                    {//空格，回车
                        spaceNum++;//空格回车数自增
                        if(!noSpaceData.isEmpty())
                        {//输出数据
                            //qDebug() << "非空格数据" << noSpaceData;
                            QString key = QString::number(gLineNum, 10);
                            root[key.toStdString()].append(noSpaceData.toStdString()); //访问指定单元格数据
                            noSpaceData.clear();//取完本次数据后清空
                        }
                    }
                    else
                    {//非空格，非回车
                        spaceNum = 0;//空格回车数清零
                        noSpaceData.append(charPreLine);//取数据
                    }
                }//end for(int i = 0; i < lineData.length(); i++)
            }//end while(!qFile.atEnd())
            Json::StyledWriter styled_writer;//格式化
            ui->textBrJsonFrLocal->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser
            //qDebug() << "总行数：" << gLineNum;
        }
    }//end if (!filename.isNull())
    else
    {
        ui->statusBar->showMessage(tr("没有选择文件！"));
    }
}

void MainWindow::on_pBtnPostWebFrLocal_clicked()
{
    myQtAPI::ShowMessageBoxInfo(tr("该功能暂时不能使用，Web服务器程序未开发完"));
}

//每当命令执行时，都会执行 ftpCommandStarted()函数，它有一个参数 int id，这个 id 就是
//调用命令时返回的 id，如 int loginID = ftp->login(); 这时，就可以用 if(id == loginID)
//来判断执行的是否是 login()函数。但是，不想为每个命令都设置一个变量来存储其返回
//值，所以，这里使用了 ftp->currentCommand() ，它也能获取当前执行的命令的类型。
//在这个函数里让开始不同的命令时显示不同的状态信息。
void MainWindow::ftpCommandStarted(int)
{
    if(ftp->currentCommand() == QFtp::ConnectToHost)
    {
        ui->statusBar->showMessage(tr(" 正在连接到服务器…"));
    }

    if (ftp->currentCommand() == QFtp::Login)
    {
        ui->statusBar->showMessage(tr(" 正在登录…"));
    }

    if (ftp->currentCommand() == QFtp::Get)
    {
        ui->statusBar->showMessage(tr(" 正在下载…"));
    }

    if (ftp->currentCommand() == QFtp::Put)
    {
        ui->statusBar->showMessage(tr(" 正在上传…"));
    }

    if (ftp->currentCommand() == QFtp::Close)
    {
        ui->statusBar->showMessage(tr(" 正在关闭连接…" ));
    }
}

//在完成一条命令时显示不同的状态信息，并显示可能的出错信息。
//int id，就是调用命令时返回的编号
//bool error，它标志现在执行的命令是否出现了错误。如果出现了错误，那么 error 为 true ，否则为 false。
//我们可以利用它来输出错误信息。
void MainWindow::ftpCommandFinished(int,bool error)
{
    if(ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if(error)
        {
            //ui->statusBar->showMessage(tr(" 连接服务器出现错误： %1").arg(ftp->errorString()));
            myQtAPI::ShowMessageBoxError(tr(" 连接服务器出现错误： %1").arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr(" 连接到服务器成功"));
        }
    }

    if (ftp->currentCommand() == QFtp::Login)
    {
        if(error)
        {
            //ui->gBoxJsonFrFtp->setEnabled(false);
            //ui->trWidgetFileList->setEnabled(false);
            //ui->pBtnFtpCdToParent->setEnabled(false);
            //ui->pBtnFtpDown->setEnabled(false);
            //ui->pBtnFtpUp->setEnabled(false);
            //ui->statusBar->showMessage(tr(" 登录出现错误： %1").arg(ftp->errorString()));
            myQtAPI::ShowMessageBoxError(tr(" 登录出现错误： %1").arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr(" 登录成功"));

            //ui->gBoxFileShowFrFtp->show();
            //ui->gBoxJsonShowFrFtp->show();
            ui->trWidgetFileListFrFtp->setEnabled(true);
            ui->pBtnFtpCdToParent->setEnabled(true);
            ui->pBtnFtpDownload->setEnabled(true);
            ui->pBtnFtpUpload->setEnabled(true);

            //在登录命令完成时，我们调用 list()函数
            ftp->list(); //发射 listInfo()信号，显示文件列表
        }
    }

    if (ftp->currentCommand() == QFtp::Get)
    {
        if(error)
        {
            myQtAPI::ShowMessageBoxError(tr("文件%1下载出现错误。\n详情：\n%2.").arg(file->fileName()).arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr(" 已经完成下载"));
            myQtAPI::ShowMessageBoxInfo(tr("文件 %1 已经完成下载").arg(file->fileName()));

            QString filename = file->fileName();

            //在下载命令完成时，我们使下载按钮可用
            //ui->pBtnFtpDown->setEnabled(true);
            file->close();

            ui->gBoxFileShowFrFtp->show();
            ui->gBoxJsonShowFrFtp->show();

            ftpFiletoJson(filename);//这里只能先关闭文件后在调用，否则打开文件可能出错。
            //ui->textBrowser->setText(ftp->readAll());

            //delete file;
        }

        ui->pBtnFtpDownload->setEnabled(true);
    }

    if (ftp->currentCommand() == QFtp::Put)
    {
        if(error)
        {
            myQtAPI::ShowMessageBoxError(tr(" 文件%1上传出现错误。\n详情：\n%2.").arg(file->fileName()).arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr("成功上传 %1 文件到当前目录。").arg(file->fileName()));
            myQtAPI::ShowMessageBoxInfo(tr("文件 %1 已经完成上传").arg(file->fileName()));

            //ui->pBtnFtpUp->setEnabled(true);//在上传命令完成时，我们使上传按钮可用

            file->close();
            ui->trWidgetFileListFrFtp->clear();
            isDirectory.clear();

            ftp->list();
            //file->close();
            //ui->textBrowser->setText(ftp->readAll());
            //delete file;
        }

        ui->pBtnFtpUpload->setEnabled(true);
    }


    //在 list 命令完成时，判断文件列表是否为空，如果为空，就让 Tree Widget 不可用，
    //并显示“empty” 条目。
    if (ftp->currentCommand() == QFtp::List)
    {
        if (isDirectory.isEmpty())
        { //如果目录为空,显示"empty"
            ui->trWidgetFileListFrFtp->addTopLevelItem(new QTreeWidgetItem(QStringList()<< tr("<empty>")));
            ui->trWidgetFileListFrFtp->setEnabled(false);
            ui->statusBar->showMessage(tr(" 该目录为空"));
        }
    }

    if (ftp->currentCommand() == QFtp::Close)
    {
        ui->statusBar->showMessage(tr(" 已经关闭连接"));
    }

    //delete ftp;
}

//添加文件列表函数
//当ftp->list()函数执行时会发射 listInfo()信号，此时就会执行 addToList()函数，在这里
//我们将文件信息显示在 Tree Widget 上，并在 isDirectory 中存储该文件的路径及其是否为目
//录的信息。为了使文件与目录进行区分，我们使用了不同的图标 file.png 和 dir.png 来表示
//它们，这两个图标放在了工程文件夹中。
void MainWindow::addToList(const QUrlInfo &urlInfo) //添加文件列表
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, _FromSpecialEncoding(urlInfo.name()));
    item->setText(1, QString::number(urlInfo.size()));
    item->setText(2, urlInfo.owner());
    item->setText(3, urlInfo.group());
    item->setText(4, urlInfo.lastModified().toString("MMM dd yyyy"));
    QPixmap pixmap(urlInfo.isDir() ? ":/dir.png" : ":/file.png");
    item->setIcon(0, pixmap);
    isDirectory[_FromSpecialEncoding(urlInfo.name())] = urlInfo.isDir();//存储该路径是否为目录的信息

    ui->trWidgetFileListFrFtp->addTopLevelItem(item);
    if (!ui->trWidgetFileListFrFtp->currentItem())
    {
        ui->trWidgetFileListFrFtp->setCurrentItem(ui->trWidgetFileListFrFtp->topLevelItem(0));
        ui->trWidgetFileListFrFtp->setEnabled(true);
    }
}

//打开一个目录
void MainWindow::processItem(QTreeWidgetItem* item,int)
{
    QString name = item->text(0);
    if (isDirectory.value(name))
    { //如果这个文件是个目录，则打开
        ui->trWidgetFileListFrFtp->clear();
        isDirectory.clear();
        currentPath += '/';
        currentPath += name;
        ftp->cd(name);
        ftp->list();
        ui->pBtnFtpCdToParent->setEnabled(true);
    }
}

QString MainWindow::_ToSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
   return QString::fromLatin1(InputStr.toLocal8Bit());
#else
   QTextCodec *codec= QTextCodec::codecForName("UTF-8");
   if (codec)
   {
       return QString::fromLatin1(codec->fromUnicode(InputStr));
   }
   else
   {
       return QString("");
   }
#endif
}

QString MainWindow::_FromSpecialEncoding(const QString &InputStr)
{
#ifdef Q_OS_WIN
   return  QString::fromLocal8Bit(InputStr.toLatin1());
#else
   QTextCodec *codec = QTextCodec::codecForName("UTF-8");
   if (codec)
   {
       return codec->toUnicode(InputStr.toLatin1());
   }
   else
   {
       return QString("");
   }
#endif
}

void MainWindow::on_pBtnFtpConnect_clicked()
{
    if(ui->pBtnFtpConnect->text() == tr("连接FTP服务器"))
    {
        ui->pBtnFtpConnect->setText(tr("断开FTP服务器"));

        ui->trWidgetFileListFrFtp->clear();
        currentPath.clear();
        isDirectory.clear();
        ui->textBrJsonShowFrFtp->clear();
        ui->textBrFileShowFrFtp->clear();
        ui->tbWidgetExcelShowFrFtp->clear();
        ui->tbWidgetExcelShowFrFtp->setColumnCount(0);
        ui->tbWidgetExcelShowFrFtp->setRowCount(0);

        //ui->gBoxJsonFrFtp->setEnabled(true);
        //ui->trWidgetFileList->setEnabled(true);
        //ui->pBtnFtpCdToParent->setEnabled(true);
        //ui->pBtnFtpDown->setEnabled(true);
        //ui->pBtnFtpUp->setEnabled(true);

        ftp = new QFtp(this);//新建了 ftp 对象

        //关联两个信号和槽，用来跟踪命令的执行情况。
        connect(ftp,SIGNAL(commandStarted(int)),
                this,SLOT(ftpCommandStarted(int)));//当每条命令开始执行时发出相应的信号
        connect(ftp,SIGNAL(commandFinished(int,bool)),
                this,SLOT(ftpCommandFinished(int,bool)));//当每条命令执行结束时发出相应的信号

        //listInfo()信号由 ftp->list()函数发射，它将在登录命令完成时调用
        connect(ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(addToList(QUrlInfo)));

        //关联了 Tree Widget 的一个信号 itemActivated()。当鼠标双击一个条目时，发射该信号，
        //我们在槽函数中判断该条目是否为目录，如果是则进入该目录。
        connect(ui->trWidgetFileListFrFtp,SIGNAL(itemActivated(QTreeWidgetItem*,int)),
                this,SLOT(processItem(QTreeWidgetItem*, int)));//鼠标双击列表中的目录时，我们进入该目录

        //从界面上获得服务器地址，用户名和密码等信息，并以它们为参数执行连接和登录命令。
        QString ftpServer = ui->lineEditFtpHost->text();
        QString userName = ui->lineEditFtpUserName->text();
        QString passWord = ui->lineEditFtpPwd->text();
        QString ftpPort = ui->lineEditFtpPort->text();

        ftp->connectToHost(ftpServer, myQtAPI::StrDecimalToDecimal(ftpPort)); //连接到服务器,默认端口号是 21

        ftp->login(userName, passWord); //登录
    }
    else
    {
        ui->pBtnFtpConnect->setText(tr("连接FTP服务器"));

        ui->trWidgetFileListFrFtp->clear();
        currentPath.clear();
        isDirectory.clear();
        ui->textBrJsonShowFrFtp->clear();
        ui->tbWidgetExcelShowFrFtp->clear();
        ui->tbWidgetExcelShowFrFtp->setColumnCount(0);
        ui->tbWidgetExcelShowFrFtp->setRowCount(0);

        ui->gBoxFileShowFrFtp->hide();
        ui->gBoxJsonShowFrFtp->hide();
        ui->trWidgetFileListFrFtp->setEnabled(false);
        ui->pBtnFtpCdToParent->setEnabled(false);
        ui->pBtnFtpDownload->setEnabled(false);
        ui->pBtnFtpUpload->setEnabled(false);

        ftp->abort();
        ftp->close();
    }
}

void MainWindow::on_pBtnPostWebFrFtp_clicked()
{
    myQtAPI::ShowMessageBoxInfo(tr("该功能暂时不能使用，Web服务器程序未开发完"));
}

void MainWindow::on_pBtnFtpCdToParent_clicked()
{
    ui->trWidgetFileListFrFtp->clear();
    isDirectory.clear();

    currentPath = currentPath.left(currentPath.lastIndexOf('/'));
    if (currentPath.isEmpty())
    {
        ui->pBtnFtpCdToParent->setEnabled(false);

        //ui->pBtnFtpDown->setEnabled(false);
        //ui->pBtnFtpUp->setEnabled(false);

        ui->statusBar->showMessage(tr("当前已经是最顶层目录。"));
        ftp->cd("/");
    }
    else
    {
        ftp->cd(currentPath);
    }

    ftp->list();
}

void MainWindow::on_pBtnFtpDownload_clicked()
{
    QString fileName = ui->trWidgetFileListFrFtp->currentItem()->text(0);

    if (isDirectory.value(fileName))
    {
        myQtAPI::ShowMessageBoxError(tr(" %1 是目录，下载目标不能是目录").arg(fileName));
        return;
    }

    QString dir = QFileDialog::getExistingDirectory(this, tr("选择下载位置"),
                                            ".",
                                            QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty())
    {
        ui->statusBar->showMessage(tr("下载已取消"));
        myQtAPI::ShowMessageBoxInfo(tr("下载已取消"));
        return;
    }

    file = new QFile(dir + "/" + fileName);
    if (!file->open(QIODevice::WriteOnly))
    {
        myQtAPI::ShowMessageBoxError(tr("不能打开文件:%1\n详情：\n%2.").arg(file->fileName()).arg(file->errorString()));

        delete file;
        return;
    }

    ui->pBtnFtpDownload->setEnabled(false); //下载按钮不可用，等下载完成后才可用

    ftp->get(_ToSpecialEncoding(ui->trWidgetFileListFrFtp->currentItem()->text(0)), file);
}

void MainWindow::on_pBtnFtpUpload_clicked()
{
    QString  fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", tr("所有文件 (*.*)"));
    if(fileName.isEmpty())
    {
        ui->statusBar->showMessage(tr("没有选择要上传的文件"));
        myQtAPI::ShowMessageBoxInfo(tr("没有选择要上传的文件"));
        return;
    }

    file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly))
    {
        myQtAPI::ShowMessageBoxError(tr("不能打开文件:%1\n详情：\n%2.").arg(file->fileName()).arg(file->errorString()));

        delete file;
        return;
    }

    QFileInfo info(fileName);

    ftp->put(file,_ToSpecialEncoding(info.fileName()));

    ui->pBtnFtpUpload->setEnabled(false);
}

//FTP下载的文件转换成JSON数据，并预览
void MainWindow::ftpFiletoJson(QString fileName)
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    Json::FastWriter writer;

    QByteArray str(fileName.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
    QFileInfo fileInfo(str);
    //qDebug() << "fileInfo.isReadable" << fileInfo.isReadable();
    QString ext = fileInfo.suffix();//获取文件后缀名
    //qDebug() << ext << QString::compare(ext, "xls") << QString::compare(ext, "xlsx");

    ui->statusBar->showMessage(tr("正在运行，请稍后···"));

    if(!(QString::compare(ext, "xls")) || !(QString::compare(ext, "xlsx")))
    {//Excel文件
        ui->tbWidgetExcelShowFrFtp->clear();
        ui->tbWidgetExcelShowFrFtp->setColumnCount(0);
        ui->tbWidgetExcelShowFrFtp->setRowCount(0);
        ui->textBrJsonShowFrFtp->clear();

        ui->textBrFileShowFrFtp->hide();
        ui->tbWidgetExcelShowFrFtp->show();
        ui->textBrJsonShowFrFtp->show();

        ui->gBoxFileShowFrFtp->setEnabled(true);
        ui->gBoxJsonShowFrFtp->setEnabled(true);//Ftp转JSON数据显示区

        // 处理文件
        QByteArray str(fileName.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
        //qDebug() << "str" << str << "fileName" << fileName;
        ExcelEngine excel(str);//创建excl对象

        //打开指定的xls文件
        if(!excel.Open())
        {
            myQtAPI::ShowMessageBoxError(tr("文件%1打开失败，请重试！").arg(fileName));
        }
        else
        {
            ui->statusBar->showMessage(fileName);
            ui->labelPathFrFtp->setText(fileName);//将路径导入到label中
        }

        excel.ReadDataToTable(ui->tbWidgetExcelShowFrFtp);//导入excel中的所有数据到tablewidget中并显示

        gRowCount = excel.GetRowCount();//获取行数
        gColumnCount = excel.GetColumnCount();//获取列数

        for(uint row = 1; row <= gRowCount; row++)
        {//行
            for(uint column = 1; column <= gColumnCount; column++)
            {//列
                QVariant valueJson = excel.GetCellData(row, column);//访问指定单元格数据
                QString key = QString::number(row, 10);
                root[key.toStdString()].append(valueJson.toString().toStdString()); //访问指定单元格数据
             }
        }

        //Json::FastWriter fast_writer;
        //ui->textBrowser->append(QString::fromStdString(fast_writer.write(root)));//导出数据到textbrowser

        Json::StyledWriter styled_writer;//格式化
        ui->textBrJsonShowFrFtp->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser

        excel.Close();//关闭excel文件
    }//end if(!(QString::compare(ext, "xls")) || !(QString::compare(ext, "xlsx")))
    else
    {//非Excel文件
        ui->textBrFileShowFrFtp->clear();
        ui->textBrJsonShowFrFtp->clear();

        ui->tbWidgetExcelShowFrFtp->hide();
        ui->textBrFileShowFrFtp->show();
        ui->textBrJsonShowFrFtp->show();

        ui->gBoxFileShowFrFtp->setEnabled(true);
        ui->gBoxJsonShowFrFtp->setEnabled(true);//文本文件转JSON数据显示区.
        // 处理文件
        QByteArray str(fileName.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
        QFile qFile(str);
        if(!qFile.open(QIODevice::ReadOnly))
        {
            myQtAPI::ShowMessageBoxError(tr("文件%1打开失败，请重试！").arg(fileName));
        }
        else
        {
            ui->statusBar->showMessage(fileName);
            ui->labelPathFrFtp->setText(fileName);//将路径导入到label中
        }

        gLineNum = 0;

        while(!qFile.atEnd())
        {
            gLineNum++;
            QByteArray lineData = qFile.readLine();//按行读取
            QString strLine(lineData);
            ui->textBrFileShowFrFtp->append(strLine);//追加数据到textbrowser
            //qDebug() << "lineData.length" << lineData.length();
            uint spaceNum = 0;//空格回车数
            QString noSpaceData;//非空格非回车数据
            for(int i = 0; i < lineData.length(); i++)
            {//一行数据里循环取一个字符
                QString charPreLine = strLine.mid(i, 1);//依次取1个字符
                if(!QString::compare(charPreLine, " ") ||
                        !QString::compare(charPreLine, "\r") ||
                        !QString::compare(charPreLine, "\n"))
                {//空格，回车
                    spaceNum++;//空格回车数自增
                    if(!noSpaceData.isEmpty())
                    {//输出数据
                        //qDebug() << "非空格数据" << noSpaceData;
                        QString key = QString::number(gLineNum, 10);
                        root[key.toStdString()].append(noSpaceData.toStdString()); //访问指定单元格数据
                        noSpaceData.clear();//取完本次数据后清空
                    }
                }
                else
                {//非空格，非回车
                    spaceNum = 0;//空格回车数清零
                    noSpaceData.append(charPreLine);//取数据
                }
            }//end for(int i = 0; i < lineData.length(); i++)
        }//end while(!qFile.atEnd())

        Json::StyledWriter styled_writer;//格式化
        ui->textBrJsonShowFrFtp->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser
    }//end 非Excel文件
}

void MainWindow::on_pBtnOpenFrDev_clicked()
{
    if (ui->pBtnOpenFrDev->text() == tr("打开串口"))
    {
        QString portName = ui->cBoxComPort->currentText();

#ifdef Q_OS_WIN//如果是windows系统
        myCom = new QextSerialPort(portName);
#else
        myCom = new QextSerialPort("/dev/" + portName);
#endif

        if (myCom->open(QIODevice::ReadWrite))
        {
            isMyComOpened = true;//标记串口是否打开

            ui->gBoxJsonShowFrDev->setEnabled(true);//串口转JSON数据显示区使能
            //清空缓冲区
            myCom->flush();
            //设置波特率
            myCom->setBaudRate((BaudRateType)ui->cBoxBaudRate->currentText().toInt());
            //设置数据位
            myCom->setDataBits((DataBitsType)ui->cBoxDataBit->currentText().toInt());
            //设置校验位
            myCom->setParity((ParityType)ui->cBoxParity->currentIndex());
            //设置停止位
            myCom->setStopBits((StopBitsType)ui->cBoxStopBit->currentIndex());
            myCom->setFlowControl(FLOW_OFF);
            myCom->setTimeout(10);

            ChangeEnableToCom(true);

            ui->pBtnOpenFrDev->setText(tr("关闭串口"));
            ui->labelIsOpenFrDev->setText(tr("串口状态：打开"));
            this->myReadTimer->start();
            ui->statusBar->showMessage(tr("串口已打开！"));
        }
        else
        {
            isMyComOpened = false;//标记串口是否打开
            myQtAPI::ShowMessageBoxError(tr("串口打开失败!\n该串口设备不存在或已被占用!\n请重试！"));
        }
    }
    else
    {
        ui->gBoxJsonShowFrDev->setEnabled(false);//串口转JSON数据显示区禁用
        myCom->close();
        isMyComOpened = false;//标记串口是否打开
        ChangeEnableToCom(false);
        ui->pBtnOpenFrDev->setText(tr("打开串口"));
        ui->labelIsOpenFrDev->setText(tr("串口状态：关闭"));
        this->myReadTimer->stop();
        ui->statusBar->showMessage(tr("串口未打开！"));

        //这样的话保证每次关闭串口后,自动发送和自动保存定时器不会空转
        ui->checkIsAutoSend->setChecked(false);
    }
}

void MainWindow::on_pBtnClearSendFrDev_clicked()
{
    ui->cBoxDevSendData->clearEditText();//发送区清空

    //发送计数器清空
    SendCount = 0;
    ui->labelSendCountFrDev->setText(tr("发送:0 字节"));
}

void MainWindow::on_pBtnClearRecvFrDev_clicked()
{
    ui->textEditDataShowFrDev->clear();//接收区清空

    //接收计数器清空
    ReceiveCount = 0;
    ui->labelRecvCountFrDev->setText(tr("接收:0 字节"));
}

void MainWindow::on_pBtnPostWebFrDev_clicked()
{
    myQtAPI::ShowMessageBoxInfo(tr("该功能暂时不能使用，Web服务器程序未开发完"));
}

void MainWindow::on_pBtnDevSend_clicked()
{
    WriteMyCom();//发送数据
}

void MainWindow::on_checkIsDebugSimulate_stateChanged(int arg1)
{
    IsDebug = (arg1 == 0 ? false : true);//开启调试复选框状态改变，模拟接收
}

void MainWindow::on_checkHexReceive_stateChanged(int arg1)
{
    IsHexReceive = (arg1 == 0 ? false : true);//十六进制接收复选框状态改变
}

void MainWindow::on_checkHexSend_stateChanged(int arg1)
{
    IsHexSend = (arg1 == 0 ? false : true);//十六进制复选框状态改变
}

void MainWindow::on_checkIsAutoSend_stateChanged(int arg1)
{//自动发送复选框状态改变
    bool IsAutoSend = (arg1 == 0 ? false : true);
    if (IsAutoSend)
    {
        this->mySendTimer->start();
    }
    else
    {
        this->mySendTimer->stop();
    }
    ui->cBoxAutoSendTime->setEnabled(IsAutoSend);
}

void MainWindow::on_cBoxAutoSendTime_currentIndexChanged(int index)
{
    mySendTimer->setInterval((index + 1) * 1000);//自动发送计时区组合框改变
}
