#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lib/excelengine/excelengine.h"//操作excelengine所需头文件
#include "lib/json/json.h"//操作json所需头文件
#include <QFileDialog>
#include <QTextCodec>

//调试信息
#define qDbg(format, ...) qDebug("[debug info] "format" File:%s, Line:%d, Function:%s", ##__VA_ARGS__, __FILE__, __LINE__ , __FUNCTION__);

uint gRowCount;//excel表格行数
uint gColumnCount;//excel表格列数
uint gLineNum;//文本文件总行数

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
#if 0
void MainWindow::on_pBtnOpenfile_clicked()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    Json::FastWriter writer;

    //选择文件对话框
    QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "所有文件(*.*)");
    if (!filename.isNull())
    { //用户选择了文件
        // 处理文件
        ui->labelPath->setText(filename);//将路径导入到label中
        QByteArray str(filename.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
        QFileInfo fileInfo(str);
        QString ext = fileInfo.suffix();//获取文件后缀名
        //qDebug() << ext << QString::compare(ext, "xls") << QString::compare(ext, "xlsx");
        if(!(QString::compare(ext, "xls")) || !(QString::compare(ext, "xlsx")))
        {//Excel文件
            ExcelEngine excel(filename);//创建excl对象

            //打开指定的xls文件
            if(!excel.Open())
            {
                qDbg("file %s open failed!", str.data());
            }
            else
            {
                qDbg("file %s open succeed!", str.data());
            }

            //ui->labelPath->setText(filename);//将路径导入到label中

            excel.ReadDataToTable(ui->tableWidget);//导入excel中的所有数据到tablewidget中并显示

            gRowCount = excel.GetRowCount();//获取行数
            gColumnCount = excel.GetColumnCount();//获取列数
            //ui->textBrowser->setText(tr("总行数gRowCount："));//导入到textBrowser中
            //ui->textBrowser->append(QString::number(gRowCount, 10));//添加到textBrowser中
            //ui->textBrowser->append(tr("总列数gColumnCount："));//添加到textBrowser中
            //ui->textBrowser->append(QString::number(gColumnCount, 10));//添加到textBrowser中
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
            ui->textBrowserExcel2Json->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser

            excel.Close();//关闭excel文件
        }
        else
        {//非Excel文件
            QFile qFile(str);
            if(!qFile.open(QIODevice::ReadOnly))
            {
                qDebug()<<"Can't open the file!"<<endl;
            }

            while(!qFile.atEnd())
            {
                gLineNum++;
                QByteArray lineData = qFile.readLine();//按行读取
                QString strLine(lineData);
                ui->textBrowserWenben->append(strLine);//追加数据到textbrowser
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
            ui->textBrowserWenben2Json->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser
            //qDebug() << "总行数：" << gLineNum;
        }
    }
}
#endif
void MainWindow::on_pBtnOpenExcelfile_clicked()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    Json::FastWriter writer;

    ui->textBrowserExcel2Json->clear();
    ui->tableWidget->clear();
    ui->labelExcelPath->clear();

    //选择文件对话框
    QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "Excel file(*.xls *.xlsx)");
    if (!filename.isNull())
    { //用户选择了文件
        // 处理文件
        ui->labelExcelPath->setText(filename);//将路径导入到label中
        //ui->labelExcelPath->adjustSize();
        ui->labelExcelPath->setWordWrap(true);
        ui->labelExcelPath->setAlignment(Qt::AlignTop);
        QByteArray str(filename.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
        ExcelEngine excel(filename);//创建excl对象

        //打开指定的xls文件
        if(!excel.Open())
        {
            qDbg("file %s open failed!", str.data());
        }
        else
        {
            qDbg("file %s open succeed!", str.data());
        }

        //ui->labelPath->setText(filename);//将路径导入到label中

        excel.ReadDataToTable(ui->tableWidget);//导入excel中的所有数据到tablewidget中并显示

        gRowCount = excel.GetRowCount();//获取行数
        gColumnCount = excel.GetColumnCount();//获取列数
        //ui->textBrowser->setText(tr("总行数gRowCount："));//导入到textBrowser中
        //ui->textBrowser->append(QString::number(gRowCount, 10));//添加到textBrowser中
        //ui->textBrowser->append(tr("总列数gColumnCount："));//添加到textBrowser中
        //ui->textBrowser->append(QString::number(gColumnCount, 10));//添加到textBrowser中
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
        ui->textBrowserExcel2Json->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser

        excel.Close();//关闭excel文件
    }
}

void MainWindow::on_pBtnOpenWenbenfile_clicked()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;
    Json::FastWriter writer;

    ui->textBrowserWenben->clear();
    ui->textBrowserWenben2Json->clear();
    ui->labelWenbenPath->clear();

    //选择文件对话框
    QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", "所有文件(*.*)");
    if (!filename.isNull())
    { //用户选择了文件
        // 处理文件
        ui->labelWenbenPath->setText(filename);//将路径导入到label中
        //ui->labelWenbenPath->adjustSize();
        ui->labelWenbenPath->setWordWrap(true);
        ui->labelWenbenPath->setAlignment(Qt::AlignTop);
        QByteArray str(filename.toUtf8());//为了将Qstring类型在控制台输出进行转换，toUtf8为了显示中文
        QFile qFile(str);
        if(!qFile.open(QIODevice::ReadOnly))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }

        gLineNum = 0;

        while(!qFile.atEnd())
        {
            gLineNum++;
            QByteArray lineData = qFile.readLine();//按行读取
            QString strLine(lineData);
            ui->textBrowserWenben->append(strLine);//追加数据到textbrowser
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
        ui->textBrowserWenben2Json->append(QString::fromStdString(styled_writer.write(root)));//追加数据到textbrowser
        //qDebug() << "总行数：" << gLineNum;
    }
}
