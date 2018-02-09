#ifndef COMMON_H
#define COMMON_H

#include <QtCore>

bool isDirExist(QString);
bool isFileExist(QString);
void setFileDir(QString);
void setFile(QString);

/*配置文件操作函数*/
QString readFromConfig(QString);
void writeIntoConfig(QString, QString);

/*template<typename T>
void writeIntoConfig(QString key, const T& value) {
    QString iniFilePath = QCoreApplication::applicationDirPath() + "/config/config.ini";
    QSettings settings(iniFilePath,QSettings::IniFormat);

    settings.setValue("GROUP/" + key, value);
}*/

#endif // COMMON_H
