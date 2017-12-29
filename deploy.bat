@echo off

echo 该脚本用于自动化发布Release程序
echo 使用说明：
echo 1. 运行该脚本时，需要保证已经正确编译生成release文件夹下的ui.exe文件；
echo 2. 不同版本的Qt在Windows上安装后，对应的QT环境部署批处理文件的路径可能会不同，
echo    使用前需要确认该批处理文件路径是否正确，否则本脚本可能会执行失败；
echo 3. 脚本运行时，会强制删除脚本所在目录下的deploy文件夹，所以必要情况下请注意文
echo    件备份。
:: 请根据实际情况，将该变量修改为对应路径的QT脚本。

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
echo 目录%CUR_PATH%
mkdir %CONFIG_PATH%
mkdir %SKINS_PATH%
mkdir %MUSIC_PATH%
copy /B %CUR_PATH%skins\*.png %SKINS_PATH%