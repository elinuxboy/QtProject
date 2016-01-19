/*
 * 流程说明。
 * 整个程序的流程就和我们实现函数的顺序一样。用户在界面上输入服务器的相关信息 ，
 * 然后我们利用这些信息进行连接并登录服务器，等登录服务器成功时，我们列出服务器上所有的文件。
 * 对于一个目录，我们可以进入其中，并返回上一级目录，我们可以下载文件，并显示下载的进度。
 * 对于 ftp 的操作，全部由那些命令和信号来完成，我们只需要调用相应的命令，并在其发出信号时，
 * 进行对应的处理就可以了。而对于文件的显示，则是视图部分的知识了。
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myQtAPI::SetUTF8Code();

    myQtAPI::FormInCenter(this);

    ui->trWidgetFileList->setEnabled(false);
    ui->pBtnFtpCdToParent->setEnabled(false);
    ui->pBtnFtpDown->setEnabled(false);
    ui->pBtnFtpUp->setEnabled(false);

    ftp = new QFtp(this);//新建了 ftp 对象

    //关联两个信号和槽，用来跟踪命令的执行情况。
    connect(ftp,SIGNAL(commandStarted(int)),
            this,SLOT(ftpCommandStarted(int)));//当每条命令开始执行时发出相应的信号
    connect(ftp,SIGNAL(commandFinished(int,bool)),
            this,SLOT(ftpCommandFinished(int,bool)));//当每条命令执行结束时发出相应的信号

    //listInfo()信号由 ftp->list()函数发射，它将在登录命令完成时调用
    connect(ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(addToList(QUrlInfo)));

    //dataTransferProgress()信号在数据传输时自动发射。
    connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),
            this,SLOT(updateDataTransferProgress(qint64,qint64)));

    //这里我们只是让进度条的值为 0，然后关联了 Tree Widget 的一个信号 itemActivated()。当
    //鼠标双击一个条目时，发射该信号，我们在槽函数中判断该条目是否为目录，如果是则进入该目录。
    ui->progressBar->setValue(0);
    connect(ui->trWidgetFileList,SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this,SLOT(processItem(QTreeWidgetItem*, int)));//鼠标双击列表中的目录时，我们进入该目录
}

MainWindow::~MainWindow()
{
    delete ui;
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
            //ui->statusBar->showMessage(tr(" 登录出现错误： %1").arg(ftp->errorString()));
            myQtAPI::ShowMessageBoxError(tr(" 登录出现错误： %1").arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr(" 登录成功"));
            //在登录命令完成时，我们调用 list()函数
            ftp->list(); //发射 listInfo()信号，显示文件列表
        }
    }

    if (ftp->currentCommand() == QFtp::Get)
    {
        if(error)
        {
            myQtAPI::ShowMessageBoxError(tr("下载出现错误：%1\n详情：\n%2.").arg(file->fileName()).arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr(" 已经完成下载"));
            //在下载命令完成时，我们使下载按钮可用
            //ui->pBtnFtpDown->setEnabled(true);
            file->close();
            //ui->textBrowser->setText(ftp->readAll());

            //delete file;
        }

        ui->pBtnFtpDown->setEnabled(true);
    }

    if (ftp->currentCommand() == QFtp::Put)
    {
        if(error)
        {
            myQtAPI::ShowMessageBoxError(tr(" 上传出现错误：%1\n详情：\n%2.").arg(file->fileName()).arg(ftp->errorString()));
        }
        else
        {
            ui->statusBar->showMessage(tr("成功上传 %1 文件到当前目录。").arg(file->fileName()));

            //ui->pBtnFtpUp->setEnabled(true);//在上传命令完成时，我们使上传按钮可用

            file->close();
            ui->trWidgetFileList->clear();
            isDirectory.clear();

            ftp->list();
            //file->close();
            //ui->textBrowser->setText(ftp->readAll());
            //delete file;
        }

        ui->pBtnFtpUp->setEnabled(true);
    }


    //在 list 命令完成时，判断文件列表是否为空，如果为空，就让 Tree Widget 不可用，
    //并显示“empty” 条目。
    if (ftp->currentCommand() == QFtp::List)
    {
        if (isDirectory.isEmpty())
        { //如果目录为空,显示"empty"
            ui->trWidgetFileList->addTopLevelItem(new QTreeWidgetItem(QStringList()<< tr("<empty>")));
            ui->trWidgetFileList->setEnabled(false);
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

    ui->trWidgetFileList->addTopLevelItem(item);
    if (!ui->trWidgetFileList->currentItem())
    {
        ui->trWidgetFileList->setCurrentItem(ui->trWidgetFileList->topLevelItem(0));
        ui->trWidgetFileList->setEnabled(true);
    }
}

void MainWindow::processItem(QTreeWidgetItem* item,int) //打开一个目录
{
    QString name = item->text(0);
    if (isDirectory.value(name))
    { //如果这个文件是个目录，则打开
        ui->trWidgetFileList->clear();
        isDirectory.clear();
        currentPath += '/';
        currentPath += name;
        ftp->cd(name);
        ftp->list();
        ui->pBtnFtpCdToParent->setEnabled(true);
    }
}

void MainWindow::on_pBtnFtpConnect_clicked()
{
    if(ui->pBtnFtpConnect->text() == "连 接")
    {
        ui->pBtnFtpConnect->setText("断开连接");

        ui->trWidgetFileList->clear();
        currentPath.clear();
        isDirectory.clear();

        ui->trWidgetFileList->setEnabled(true);
        ui->pBtnFtpCdToParent->setEnabled(true);
        ui->pBtnFtpDown->setEnabled(true);
        ui->pBtnFtpUp->setEnabled(true);

        //从界面上获得服务器地址，用户名和密码等信息，并以它们为参数执行连接和登录命令。
        QString ftpServer = ui->lineEditFtpServer->text();
        QString userName = ui->lineEditFtpUserName->text();
        QString passWord = ui->lineEditFtpPwd->text();
        QString ftpPort = ui->lineEditFtpPort->text();

        ftp->connectToHost(ftpServer, myQtAPI::StrDecimalToDecimal(ftpPort)); //连接到服务器,默认端口号是 21

        ftp->login(userName,passWord); //登录
    }
    else
    {
        ui->pBtnFtpConnect->setText("连 接");

        ui->trWidgetFileList->clear();
        currentPath.clear();
        isDirectory.clear();

        ui->trWidgetFileList->setEnabled(false);
        ui->pBtnFtpCdToParent->setEnabled(false);
        ui->pBtnFtpDown->setEnabled(false);
        ui->pBtnFtpUp->setEnabled(false);

        ftp->close();
    }
}

//在返回上一级目录时，我们取当前路径的最后一个“/” 之前的部分，如果此时路径为空了，
//我们就让“ 返回上一级目录” 按钮不可用。
void MainWindow::on_pBtnFtpCdToParent_clicked()
{
    ui->trWidgetFileList->clear();
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

//获取了当前项目的文件名，然后新建文件，使用 get()命令下载服务器上的文件到我们新建的文件中。
void MainWindow::on_pBtnFtpDown_clicked()
{
    QString fileName = ui->trWidgetFileList->currentItem()->text(0);

    QString dir = QFileDialog::getExistingDirectory(this, tr("选择文件下载位置"),
                                            ".",
                                            QFileDialog::ShowDirsOnly
                                            | QFileDialog::DontResolveSymlinks);

    file = new QFile(dir + "/" + fileName);
    if (!file->open(QIODevice::WriteOnly))
    {
        myQtAPI::ShowMessageBoxError(tr("不能打开文件:%1\n详情：\n%2.").arg(file->fileName()).arg(file->errorString()));

        delete file;
        return;
    }

    ui->pBtnFtpDown->setEnabled(false); //下载按钮不可用，等下载完成后才可用

    ftp->get(_ToSpecialEncoding(ui->trWidgetFileList->currentItem()->text(0)), file);
}

//进度条
void MainWindow::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(readBytes);
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

void MainWindow::uploadFile()
{
    QString  fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", tr("所有文件 (*.*)"));
    if(fileName.isEmpty())
    {
        ui->statusBar->showMessage("没有选择要上传的文件");
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

    ui->pBtnFtpUp->setEnabled(false);
}

void MainWindow::on_pBtnFtpUp_clicked()
{
    uploadFile();
}

void MainWindow::on_pBtnExit_clicked()
{
    qDebug() << ftp->state();

    if(ftp->state() == QFtp::Unconnected)
    {
        //delete ftp;
        //delete file;
        this->close();
    }
    else
    {
        myQtAPI::ShowMessageBoxInfo("FTP正在使用中，请先断开连接!");
        return;
    }
}
