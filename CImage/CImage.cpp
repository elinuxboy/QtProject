#include <QtGui/QtGui>

#include "CImage.h"

CImage::CImage(QWidget  *parent):QDialog(parent)
{
    ImageLabel=new QLabel(tr("加载中."));
	
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(ImageLabel);
    setLayout(vboxLayout);

    setWindowTitle(tr("加载网络图片测试"));
    resize(300, 200);
	 
	currentPicture = new QPixmap;
	
	//获取网络图片
	QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://d4.sina.com.cn/201302/27/478480_14040A.JPG")));
    //QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    //QString urlTmp = "http://192.168.1.222:8088/wlis3/ValidationCodeServlet?t=";
    //urlTmp.append(time.toString());
    //qDebug() << "urlTmp" << urlTmp;
    //manager->get(QNetworkRequest(QUrl(urlTmp)));
}


void CImage::replyFinished(QNetworkReply *reply)
{
	if(reply->error() == QNetworkReply::NoError)
	{
		//获取字节流构造 QPixmap 对象 
		currentPicture->loadFromData(reply->readAll()); 
		QDateTime now;
		QString filename = now.currentDateTime().toString("yyMMddhhmmss.jpg");
		currentPicture->save(filename);//保存图片
		//qDebug()<<"picture saved as "<<filename;
		currentFileName = filename;
		
		//可以在onpaint中 用QPixmap 显示currentPicture，则这种方法则不需要保存
		
		//显示图片，  
		QMovie *move = new QMovie(currentFileName);
		ImageLabel->setMovie(move);
		move->start();
	}
}
