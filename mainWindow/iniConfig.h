#ifndef INICONFIG
#define INICONFIG

#include <QObject>
#include <QSettings>
#include <QMetaType>
#include <QVariant>

#include "lib/myqtapi.h"

#pragma pack(push,1)//原来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐
struct OptionDataStruct
{
    QString webUrl;//服务器URL
    bool isUsedVerifiCode;//是否使用验证码登陆
    QString VerifiCodeUrl;//验证码URL
};
#pragma pack(pop)//恢复对齐状态
Q_DECLARE_METATYPE(OptionDataStruct)

class iniConfig:public QObject
{
public:
    //保存配置数据，将数据写入配置文件
    static void saveOptionToFile(OptionDataStruct &optionData)
    {
        QSettings settings("Option.ini", QSettings::IniFormat); //INI文件

        //服务器设置信息
        settings.beginGroup("WebServerInfo");//Group
        settings.setValue("WebUrl", optionData.webUrl);//key,value
        settings.setValue("isUsedVerifiCode", optionData.isUsedVerifiCode);//key,value
        settings.setValue("VerifiCodeUrl", optionData.VerifiCodeUrl);//key,value
        settings.endGroup();

        //其他设置信息
        settings.beginGroup("OtherInfo");//Group
        //settings.setValue("isUsedVerifiCode", tmpOptionData.isUsedVerifiCode);//key,value
        settings.endGroup();
    }

    //加载配置文件，并设置相关参数。isDefault为true时加载默认配置文件
    static OptionDataStruct loadOptionFrFile(bool isDefault)
    {
        OptionDataStruct tmpOptionData;
        if(isDefault)
        {//加载默认配置文件
            if(!myQtAPI::FileIsExist("OptionDefault.ini"))
            {//如果默认配置文件不存在，则创建默认配置文件，并写入默认数据
                QSettings settings("OptionDefault.ini", QSettings::IniFormat); //INI文件

                //服务器设置信息
                settings.beginGroup("WebServerInfo");
                settings.setValue("WebUrl", "http://192.168.1.254:8080/wlis3/j_acegi_security_check.action?ajax=true");
                settings.setValue("isUsedVerifiCode", true);//key,value
                settings.setValue("VerifiCodeUrl", "http://192.168.1.254:8080/wlis3/ValidationCodeServlet?t=");//key,value
                settings.endGroup();

                //其他设置信息
                settings.beginGroup("OtherInfo");//Group
                //settings.setValue("isUsedVerification", true);//key,value
                settings.endGroup();
            }

            QSettings settings("OptionDefault.ini", QSettings::IniFormat);
            tmpOptionData.webUrl = settings.value("WebServerInfo/WebUrl").toString();//取出数据
            tmpOptionData.isUsedVerifiCode = settings.value("WebServerInfo/isUsedVerifiCode").toBool();//取出数据
            tmpOptionData.VerifiCodeUrl = settings.value("WebServerInfo/VerifiCodeUrl").toString();//取出数据
        }
        else
        {//加载配置文件
            if(!myQtAPI::FileIsExist("Option.ini"))
            {
                //myQtAPI::ShowMessageBoxError(tr("配置文件Option.ini不存在"));
            }
            else
            {
                QSettings settings("Option.ini", QSettings::IniFormat);
                tmpOptionData.webUrl = settings.value("WebServerInfo/WebUrl").toString();//取出数据
                tmpOptionData.isUsedVerifiCode = settings.value("WebServerInfo/isUsedVerifiCode").toBool();//取出数据
                tmpOptionData.VerifiCodeUrl = settings.value("WebServerInfo/VerifiCodeUrl").toString();//取出数据
            }
        }

        return tmpOptionData;
    }
};

#endif // INICONFIG

