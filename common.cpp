#include "common.h"

bool isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists()){
      return true;
    }
    return false;
}

bool isFileExist(QString fullFilePath)
{
    QFile file(fullFilePath);
    if(file.exists()){
        return true;
    }
    return false;
}

void setFileDir(QString filePath)
{
    if(!isDirExist(filePath)){
        QDir dir;
        dir.mkpath(filePath);
    }
    return;
}

void setFile(QString fullFileName)
{
    if(!isFileExist(fullFileName)){
        //QString fileName = fullFileName.split("/").back();
        QFile file(fullFileName);
        file.open( QIODevice::ReadWrite | QIODevice::Text );
        file.close();
    }
    return;
}

QString readFromConfig(QString key)
{
    QString iniFilePath = QCoreApplication::applicationDirPath() + "/config/config.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);

    QString value = settings.value("GROUP1/"+key).toString();

    return value;
}

void writeIntoConfig(QString key, QString value)
{
    QString iniFilePath = QCoreApplication::applicationDirPath() + "/config/config.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);

    settings.setValue("GROUP1/" + key, value);
}
