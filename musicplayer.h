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


class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

    QString changeFileName(QString);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

protected:
//    void closeEvent(QCloseEvent *);

public slots:
    void changslider();
    void changplaystyle(int );
    void playstate();
    void playstop();
    void nextplay();
    void lastplay();
    QStringList readfile();
    void changeMusic(int,int);
    void changeSkin();
    void updateMusicNameLabel(int);
    void updateslider();

private:
    QTimer* timer;

    QSystemTrayIcon* trayicon;
    QPoint dpos;
    QPoint windowPos;
    QPoint  mousePos;

    QPushButton* windowminBtn;//最小化
    QPushButton* lastBtn;//上一首
    QPushButton* playBtn;//播放
    QPushButton* stopBtn;//停止
    QPushButton* nextBtn;//下一首
    QPushButton* volumeBtn;//音量
    QPushButton* addmusicBtn;//添加歌曲
    QPushButton* skinBtn;//换肤
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


    void setInterfaceMenu();
    void setInterfaceLayout();
};

#endif // MUSICPLAYER_H
