#include "musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent):
    QWidget(parent)
{
    this->setWindowIcon(QIcon(":images/icon.png"));
    this->setFixedSize(600,350);

    /* 全局变量初始化 */
    musicNameLabel = new QLabel();
    trayicon = new QSystemTrayIcon(QIcon(":images/icon.png"));

    /* 界面初始化 */
    this->setInterfaceMenu();
    this->setInterfaceLayout();
    QPalette palette;
    this->setAutoFillBackground(true);
#if 0
    QPixmap pix1(":images/4.png");
    pix1 = pix1.scaled(this->width(),this->height());
    palette.setBrush(QPalette::Background, QBrush(QPixmap(pix1)));
#else
    palette.setColor(QPalette::Window,QColor(Qt::white));
#endif
    this->setPalette(palette);

    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Random);
    player = new QMediaPlayer();
    player->setPlaylist(playlist);
    player->setVolume(50);
    timer = new QTimer(this);
    timer->setInterval(5000);


    connect(timer,SIGNAL(timeout()),this,SLOT(changslider()));
    connect(playstyleCombox,SIGNAL(currentIndexChanged(int)),this,SLOT(changplaystyle(int)));
    connect(skinBtn,SIGNAL(clicked()),this,SLOT(changeSkin()));
    connect(addmusicBtn,SIGNAL(clicked(bool)),this,SLOT(readfile()));
    connect(stopBtn,SIGNAL(clicked(bool)),this,SLOT(playstop()));
    connect(playBtn,SIGNAL(clicked(bool)),this,SLOT(playstate()));
    connect(nextBtn,SIGNAL(clicked(bool)),this,SLOT(nextplay()));
    connect(lastBtn,SIGNAL(clicked(bool)),this,SLOT(lastplay()));
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(changeMusic(int,int)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(updateMusicNameLabel(int)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(updateslider()));
}

MusicPlayer::~MusicPlayer()
{

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

void MusicPlayer::changslider()
{
    int i = slider->value();
    slider->setValue(++i);
}


void MusicPlayer::changplaystyle(int)
{
    int i = playstyleCombox->currentIndex();
    switch (i)
    {
    case 1:
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 2:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 3:
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case 4:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        break;
    default:
        break;
    }


}

void MusicPlayer::playstate()
{
    player->play();
    timer->start();
}

void MusicPlayer::playstop()
{
    player->stop();
    timer->stop();
}

void MusicPlayer::nextplay()
{
    int i=playlist->currentIndex();
    //QString name=playlist->currentMedia();
    if(++i == playlist->mediaCount())
    {
        i = 0;
    }
    playlist->setCurrentIndex(i);
    //playstate();
    player->play();
    //  musicNameLabel->setText(name);

    timer->start();

}


void MusicPlayer::lastplay()
{
    int i = playlist->currentIndex();
    if(--i < 0)
    {
        i = 0;
    }
    playlist->setCurrentIndex(i);
    //playstate();
    player->play();
    timer->start();
}

QStringList MusicPlayer::readfile()
{
    QStringList musicList=QFileDialog::getOpenFileNames(this,tr("选择文件"),QCoreApplication::applicationDirPath() + "/music",tr("music file(*.mp3)"));
    for(int i=0; i < musicList.count(); i++)
    {
        QString musicPath = musicList.at(i);
        QString musicInfo = musicPath.split("/").last();
        playlist->addMedia(QUrl::fromLocalFile(musicPath));
        int rowNum = tableWidget->rowCount();
        tableWidget->insertRow(rowNum);
        tableWidget->setItem(rowNum,0,new QTableWidgetItem(musicInfo.split(".").first()));
        tableWidget->setItem(rowNum,1,new QTableWidgetItem(musicInfo.split(".").last()));
        tableWidget->setRowHeight(rowNum,20);
    }
    return musicList;
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
    musicNameLabel->setText(tableWidget->item(index,0)->text());
}

void MusicPlayer::updateslider()
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
    vLayout->addLayout(hLayout);
}
