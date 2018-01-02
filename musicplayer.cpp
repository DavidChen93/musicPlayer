#include "musicplayer.h"

#define isdebug  0 // 1为调试，0为release

MusicPlayer::MusicPlayer(QWidget *parent):
    QWidget(parent)
{
    this->setWindowIcon(QIcon(":images/icon.png"));
    this->setFixedSize(600,350);

    /* 全局变量初始化 */
    musicNameLabel = new QLabel();
    //db = QSqlDatabase::addDatabase("QSQLITE");

    /* 界面初始化 */
    this->setInterfaceMenu();
    this->setInterfaceLayout();
    this->setSystemTray();
    this->setAutoFillBackground(true);

    QPalette palette;
#if 0
    QPixmap pix1(":images/4.png");
    pix1 = pix1.scaled(this->width(),this->height());
    palette.setBrush(QPalette::Background, QBrush(QPixmap(pix1)));
#else
    palette.setColor(QPalette::Window,QColor(Qt::white));
#endif
    this->setPalette(palette);

    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player = new QMediaPlayer();
    player->setPlaylist(playlist);
    player->setVolume(50);
    timer = new QTimer(this);
    timer->setInterval(5000);


    connect(timer,SIGNAL(timeout()),this,SLOT(changeSlider()));
    connect(playstyleCombox,SIGNAL(currentIndexChanged(int)),this,SLOT(changePlaystyle(int)));
    connect(skinBtn,SIGNAL(clicked()),this,SLOT(changeSkin()));
    connect(addmusicBtn,SIGNAL(clicked(bool)),this,SLOT(readFile()));
    connect(stopBtn,SIGNAL(clicked(bool)),this,SLOT(playStop()));
    connect(playBtn,SIGNAL(clicked(bool)),this,SLOT(playStart()));
    connect(nextBtn,SIGNAL(clicked(bool)),this,SLOT(nextMusic()));
    connect(lastBtn,SIGNAL(clicked(bool)),this,SLOT(prevMusic()));
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(changeMusic(int,int)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(updateMusicNameLabel(int)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(updateSlider()));
    connect(systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(systemTrayOperation(QSystemTrayIcon::ActivationReason)));
#if isdebug
    connect(testBtn,SIGNAL(clicked(bool)),this,SLOT(testWindow()));
#endif
    autoScanFiles();
}

MusicPlayer::~MusicPlayer()
{
    //delete trayMenu;
}

void MusicPlayer::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dpos);
}

void MusicPlayer::mousePressEvent(QMouseEvent *event)
{
    this->windowPos= this->pos();                // 获得部件当前位置
    this->mousePos = event->globalPos();     // 获得鼠标位置
    this->dpos = mousePos - windowPos;       // 移动后部件所在的位置
}

void MusicPlayer::changeSlider()
{
    int i = slider->value();
    slider->setValue(++i);
}


void MusicPlayer::changePlaystyle(int index)
{
    switch (index)
    {
    case 0:
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 1:
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    case 2:
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case 3:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    default:
        break;
    }


}

void MusicPlayer::playStart()
{
    player->play();
    timer->start();
}

void MusicPlayer::playStop()
{
    player->stop();
    timer->stop();
}

void MusicPlayer::nextMusic()
{
    int i = playlist->currentIndex();
    //QString name=playlist->currentMedia();
    if(++i == playlist->mediaCount())
    {
        i = 0;
    }
    playlist->setCurrentIndex(i);
    //playStart();
    player->play();
    //musicNameLabel->setText(name);
    timer->start();

}


void MusicPlayer::prevMusic()
{
    int i = playlist->currentIndex();
    if(--i < 0)
    {
        i = 0;
    }
    playlist->setCurrentIndex(i);
    player->play();
    timer->start();
}

QStringList MusicPlayer::readFile()
{
    QStringList musicList=QFileDialog::getOpenFileNames(this,tr("选择文件"),QCoreApplication::applicationDirPath() + "/music",tr("music file(*.mp3)"));
    for(int i=0; i < musicList.count(); i++)
    {
        QString musicPath = musicList.at(i);
        //QString musicInfo = musicPath.split("/").last();
        QString musicInfo = getMusicInfo(musicPath);
        playlist->addMedia(QUrl::fromLocalFile(musicPath));
        int rowNum = tableWidget->rowCount();
        tableWidget->insertRow(rowNum);
        tableWidget->setItem(rowNum,0,new QTableWidgetItem(musicInfo.split(".").first()));
        tableWidget->setItem(rowNum,1,new QTableWidgetItem(musicInfo.split(".").last()));
        tableWidget->setRowHeight(rowNum,20);
    }
    return musicList;
}

QString MusicPlayer::getMusicInfo(QString str)
{
    QString str1 = str.split("/").last();
    return str1;
}

void MusicPlayer::changeMusic(int index, int )
{
    timer->stop();
    playlist->setCurrentIndex(index);
    player->play();
    timer->start();
}

void MusicPlayer::changeSkin()
{
    QString imagePath=QFileDialog::getOpenFileName(this,tr("选择图片"),QCoreApplication::applicationDirPath() + "/skins",tr("image file(*.png)"));
    QPixmap pix2(imagePath);
    pix2=pix2.scaled(this->width(),this->height());
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(pix2)));
    this->setPalette(palette);

}

void MusicPlayer::updateMusicNameLabel(int index)
{
    QString musicName = tableWidget->item(index,0)->text();
    musicNameLabel->setText(musicName);
    systemTray->setToolTip(musicName);
}

void MusicPlayer::updateSlider()
{
    slider->setValue(0);
}

void MusicPlayer::setInterfaceMenu()
{
    /* 上一首 */
    lastBtn = new QPushButton();
    lastBtn->setStyleSheet("QPushButton{border-image : url(:images/skipBackward.png);}");
    lastBtn->setToolTip(tr("上一首"));
    lastBtn->setFixedSize(25,25);
    lastBtn->setFlat(true);

    /* 播放 */
    playBtn = new QPushButton();
    playBtn->setStyleSheet("QPushButton{border-image : url(:images/play.png);}");
    playBtn->setFixedSize(25,25);
    playBtn->setFlat(true);
    playBtn->setToolTip(tr("播放"));

    /* 暂停 */
    stopBtn = new QPushButton();
    stopBtn->setStyleSheet("QPushButton{border-image : url(:images/stop.png);}");
    stopBtn->setToolTip(tr("暂停"));
    stopBtn->setFixedSize(25,25);
    stopBtn->setFlat(true);

    /* 下一首 */
    nextBtn = new QPushButton();
    nextBtn->setStyleSheet("QPushButton{border-image : url(:images/skipForward.png);}");
    nextBtn->setToolTip(tr("下一首"));
    nextBtn->setFixedSize(25,25);
    nextBtn->setFlat(true);

    /* 音量 */
    volumeBtn = new QPushButton();
    volumeBtn->setStyleSheet("QPushButton{border-image : url(:images/volume.png);}");
    volumeBtn->setToolTip(tr("音量"));
    volumeBtn->setFixedSize(25,25);
    volumeBtn->setFlat(true);

    /* 打开歌曲 */
    addmusicBtn = new QPushButton();
    addmusicBtn->setStyleSheet("QPushButton{border-image : url(:images/open.png);}");
    addmusicBtn->setToolTip(tr("打开歌曲"));
    addmusicBtn->setFixedSize(25,25);
    addmusicBtn->setFlat(true);

    /* 换皮肤 */
    skinBtn = new QPushButton();\
    skinBtn->setStyleSheet("QPushButton{border-image : url(:images/skin.png);}");
    skinBtn->setToolTip(tr("皮肤"));
    skinBtn->setFixedSize(25,25);
    skinBtn->setFlat(true);

    /* 播放方式 */
    playstyleCombox = new QComboBox();
    playstyleCombox->addItem(tr("循环播放"));
    playstyleCombox->addItem(tr("列表播放"));
    playstyleCombox->addItem(tr("随机播放"));
    playstyleCombox->addItem(tr("单曲循环"));
    playstyleCombox->setFixedSize(75,25);

    /* 播放列表 */
    tableWidget = new QTableWidget();
    tableWidget->setColumnCount(2);
    tableWidget->setFrameShape(QFrame::NoFrame);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //  设置选择的模式为单选择
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置每次选一行
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPalette pale;
    pale.setBrush(QPalette::Base,QColor(255,255,255,0));
    tableWidget->setPalette(pale);

    QStringList stringList;
    stringList<<tr("歌曲")<<tr("类型");
    tableWidget->setHorizontalHeaderLabels(stringList);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->resizeSection(0,500);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView {background-color:255,255,255,0 } "
                                                  "QHeaderView::section {background-color:255,255,255,0;"
                                                  "color: 255,255,255,0;padding-left: 4px;border: 0px solid 255,255,255,0;}");

    /* 滑动条 */
    slider = new QSlider(Qt::Horizontal);
    slider->setValue(0);
    slider->setMaximum(100);

#if isdebug
    /* 测试按钮 */
    testBtn = new QPushButton();
    testBtn->setFlat(true);
    testBtn->setText(tr("测试"));
    testBtn->setFixedSize(25,25);
#endif
}

void MusicPlayer::setInterfaceLayout()
{
    /* 设置垂直布局 */
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(tableWidget);
    vLayout->addWidget(musicNameLabel);
    vLayout->addWidget(slider);

    /* 设置水平布局 */
    hLayout = new QHBoxLayout();
    hLayout->addWidget(lastBtn);
    hLayout->addStretch();
    hLayout->addWidget(playBtn);
    hLayout->addStretch();
    hLayout->addWidget(stopBtn);
    hLayout->addStretch();
    hLayout->addWidget(nextBtn);
    hLayout->addStretch();
    hLayout->addWidget(volumeBtn);
    hLayout->addStretch();
    hLayout->addWidget(addmusicBtn);
    hLayout->addStretch();
    hLayout->addWidget(skinBtn);
    hLayout->addStretch();
    hLayout->addWidget(playstyleCombox);
#if isdebug
    hLayout->addStretch();
    hLayout->addWidget(testBtn);
#endif
    vLayout->addLayout(hLayout);
}

void MusicPlayer::systemTrayOperation(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Context:
        showTrayMenu();
        break;
    case QSystemTrayIcon::DoubleClick:
        showWindow();
        break;
    case QSystemTrayIcon::Trigger:
        break;
    default:
        break;
    }
}

void MusicPlayer::setSystemTray()
{
    systemTray = new QSystemTrayIcon(QIcon(":images/icon.png"));
    trayMenu = new QMenu(this);
    showWin = new QAction(trayMenu);
    prevOne = new QAction(trayMenu);
    nextOne = new QAction(trayMenu);
    startOne = new QAction(trayMenu);
    stopOne = new QAction(trayMenu);
    closeWin = new QAction(trayMenu);

    showWin->setText(tr("显示界面"));
    prevOne->setText(tr("上一首"));
    prevOne->setIcon(QIcon(":images/skipBackward.png"));
    nextOne->setText(tr("下一首"));
    nextOne->setIcon(QIcon(":images/skipForward.png"));
    startOne->setText(tr("开始"));
    startOne->setIcon(QIcon(":images/play.png"));
    stopOne->setText(tr("暂停"));
    stopOne->setIcon(QIcon(":images/pause.png"));
    closeWin->setText(tr("退出"));
    closeWin->setIcon(QIcon(":images/close.png"));

    trayMenu->addAction(showWin);
    trayMenu->addAction(prevOne);
    trayMenu->addAction(startOne);
    trayMenu->addAction(stopOne);
    trayMenu->addAction(nextOne);
    trayMenu->addAction(closeWin);
    systemTray->setContextMenu(trayMenu);

    connect(showWin,SIGNAL(triggered(bool)),this,SLOT(showWindow()));
    connect(prevOne,SIGNAL(triggered(bool)),this,SLOT(prevMusic()));
    connect(startOne,SIGNAL(triggered(bool)),this,SLOT(playStart()));
    connect(stopOne,SIGNAL(triggered(bool)),this,SLOT(playStop()));
    connect(nextOne,SIGNAL(triggered(bool)),this,SLOT(nextMusic()));
    connect(closeWin,SIGNAL(triggered(bool)),this,SLOT(closeWindow()));
}

void MusicPlayer::showTrayMenu()
{
    trayMenu->show();
}

void MusicPlayer::showWindow()
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->show();
}

void MusicPlayer::closeWindow()
{
   this->close();
}

void MusicPlayer::closeEvent(QCloseEvent *event)
{
    if(this->isHidden())
    {
        systemTray->hide();
        delete systemTray;
        event->accept();
    }
    else
    {
        switch( QMessageBox::information(this,tr("提示"),tr("您确定关闭软件吗?\n\n点击\"取消\"将最小化至托盘"),tr("确定"), tr("取消"),0,1))
        {
        case 0:
            systemTray->hide();
            delete systemTray;
            event->accept();
            break;
        case 1:
            event->ignore();
            this->hide();
            systemTray->show();
            break;
        default:
            break;
        }
    }
}

void MusicPlayer::testWindow()
{
    MsgBox *testMsgBox = new MsgBox(this);
    testMsgBox->setInfo(tr("测试"),tr("测试"),QPixmap(":/images/tip.png"),false,false);
    testMsgBox->exec();

    connect(testMsgBox,SIGNAL(okMessageHidden(bool)),this,SLOT());
    connect(testMsgBox,SIGNAL(msgChecked(bool,bool)),this,SLOT());
}

void MusicPlayer::autoScanFiles()
{
    int i = 0;
    QDir dir(QCoreApplication::applicationDirPath()+"/music");//定义一个QDir变量,设置路径为当前目录的music文件夹(这里可用绝对路径也可以用相对路径)
    QStringList songList;//用来保存歌曲名的表
    QFileInfoList fileInfoList;//文件信息表
    QFileInfo fileInfo;//文件信息变量
    QString songInfo;//用于保存文件信息字符串

    songList << "*.mp3";
    dir.setNameFilters(songList);
    fileInfoList = dir.entryInfoList();
    while( i < fileInfoList.length() ){
        fileInfo = fileInfoList.at(i);
        songInfo = fileInfo.filePath();

        playlist->addMedia(QUrl::fromLocalFile(songInfo));
        QString musicInfo = songInfo.split("/").last();
        int rowNum = tableWidget->rowCount();
        tableWidget->insertRow(rowNum);
        tableWidget->setItem(rowNum,0,new QTableWidgetItem(musicInfo.split(".").first()));
        tableWidget->setItem(rowNum,1,new QTableWidgetItem(musicInfo.split(".").last()));
        tableWidget->setRowHeight(rowNum,20);

        songList << songInfo;
        i++;
    }
}

