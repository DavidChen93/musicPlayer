@echo off

echo �ýű������Զ�������Release����
echo ʹ��˵����
echo 1. ���иýű�ʱ����Ҫ��֤�Ѿ���ȷ��������release�ļ����µ�ui.exe�ļ���
echo 2. ��ͬ�汾��Qt��Windows�ϰ�װ�󣬶�Ӧ��QT���������������ļ���·�����ܻ᲻ͬ��
echo    ʹ��ǰ��Ҫȷ�ϸ��������ļ�·���Ƿ���ȷ�����򱾽ű����ܻ�ִ��ʧ�ܣ�
echo 3. �ű�����ʱ����ǿ��ɾ���ű�����Ŀ¼�µ�deploy�ļ��У����Ա�Ҫ�������ע����
echo    �����ݡ�
:: �����ʵ����������ñ����޸�Ϊ��Ӧ·����QT�ű���

set QT_ENV_PATH=E:\Qt\Qt5.5.1\5.5\mingw492_32\bin\qtenv2.bat

set CUR_PATH=%~dp0
set TARGET_NAME=%CUR_PATH%release\Music.exe
set DEPLOY_DIR=%CUR_PATH%deploy\
set RELEASE_NAME=%DEPLOY_DIR%Music.exe
set CONFIG_PATH=%CUR_PATH%deploy\configs\
set SKINS_PATH=%CUR_PATH%deploy\skins\
set MUSIC_PATH=%CUR_PATH%deploy\music\
set DEPLOY_OPTION=-opengl -test -serialport
call %QT_ENV_PATH%

echo Cleaning old deploy stuffs ...
rmdir /S /Q  %DEPLOY_DIR%
echo Creating new deply directory ...
mkdir %DEPLOY_DIR%
echo Copying target program ...
copy /B %TARGET_NAME% %RELEASE_NAME%
echo Deploying ...
windeployqt.exe %DEPLOY_OPTION% %RELEASE_NAME%

echo Copying dependency libraries ...
copy /B %THIRDPARTY_OPENCV_PATH%*.dll %DEPLOY_DIR%
copy /B %THIRDPARTY_USBRELAY_PATH%*.dll %DEPLOY_DIR%
echo Ŀ¼%CUR_PATH%
mkdir %CONFIG_PATH%
mkdir %SKINS_PATH%
mkdir %MUSIC_PATH%
copy /B %CUR_PATH%skins\*.png %SKINS_PATH%