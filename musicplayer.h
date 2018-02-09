#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

//#include <QWidget>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFrame>
#include <QTableWidget>
#include <QTableView>
#include <QByteArray>
#include <QMouseEvent>
#include <QPoint>
#include <QTreeWidgetItem>
#include <QSystemTrayIcon>
#include <QComboBox>
#include <QTimer>
#include <QSlider>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QPalette>
#include <QPainter>
#include <QColor>
#include <QHeaderView>
#include <QToolBar>
#include <QSpacerItem>
#include <QCoreApplication>
#include <QMediaMetaData>
#include <QMessageBox>
#include <msgbox.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <settingdialog.h>


class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

    QString changeFileName(QString);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    bool isPlaying;//目前歌曲播放状态

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void changeSlider();
    void changePlaystyle(int index);
    void playStart();
    void playStop();
    void nextMusic();
    void prevMusic();
    void changeMusic(int,int);
    void changeSkin();
    void updateMusicNameLabel(int);
    void updateSlider();
    QStringList readFile();

    void systemTrayOperation(QSystemTrayIcon::ActivationReason);
    void showTrayMenu();
    void showWindow();
    void closeWindow();
    void testWindow();

    void changeMusicNameLabel(bool);

private:
    QTimer* timer;

    QSystemTrayIcon* systemTray;
    QPoint dpos;
    QPoint windowPos;
    QPoint  mousePos;

    /* 界面布局相关 */
    QPushButton* testBtn;//测试按钮
    QPushButton* lastBtn;//上一首
    QPushButton* playBtn;//播放
    QPushButton* stopBtn;//停止
    QPushButton* nextBtn;//下一首
    QPushButton* volumeBtn;//音量
    QPushButton* addmusicBtn;//添加歌曲
    QPushButton* skinBtn;//换肤
    QPushButton* settingBtn;//设置
    QSlider* slider;
    QComboBox* playstyleCombox;
    QHBoxLayout* hLayout;
    QVBoxLayout* vLayout;

    QByteArray actionlist;
    QTableWidget* tableWidget;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    QVideoWidget* videoWidget;
    QLabel* musicNameLabel;

    QMenu *trayMenu;//托盘菜单
    QAction *showWin;//显示主页面
    QAction *prevOne;//上一曲
    QAction *nextOne;//下一曲
    QAction *startOne;//开始
    QAction *stopOne;//暂停
    QAction *setting;//设置
    QAction *closeWin;//关闭软件

    /* 数据库相关 */
    QSqlDatabase *db;//数据库

    /* 目录扫描相关 */
    //QStringList songList;//用来保存歌曲名的表

    void setInterfaceMenu();
    void setInterfaceLayout();
    void setSystemTray();
    QString getMusicInfo(QString);
    void autoScanFiles();
    void checkPlayStatus();
    void updateMusicNameLabel();
};

#endif // MUSICPLAYER_H
