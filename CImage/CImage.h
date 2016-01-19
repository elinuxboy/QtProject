#ifndef CIMAGE_H
#define CIMAGE_H
#include <QtGui/QDialog>
#include <QLabel>
#include <QMovie>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>

class QLabel;
class QLineEdit;
class QPushButton;

class CImage:public QDialog
{
    Q_OBJECT
public:
    CImage(QWidget *parent=0);
    virtual ~CImage() {}
private:
    QLabel *ImageLabel;
    QPushButton *ImageButton;
	QPixmap *currentPicture;
	QString	currentFileName;
public slots:
	void replyFinished(QNetworkReply *reply);
};

#endif // CIMAGE_H
