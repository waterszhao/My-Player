#include "mainwindow.h"
#include "mydialog.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化基础组件
    initWidgets();
    //初始化数据库配置
    sqlUtil = new SqlUtil("myPlayer.db",this);
    qDebug() << "创建数据库表:" << sqlUtil->createTable();

    loadLastInfo();
    //......给组件赋值

    initConnections();
    //为固定（静态）组件设置信号槽

    setMenuLW();
    queryRecentSongs();
    //设置打开程序主页界面为最近播放

    ui->le_test->setHidden(true);
    ui->btn_test->setHidden(true);
    //test();
}

void MainWindow::initWidgets()
{
    songList =  new QList<Song*>();
    menuList = new QList<Menu*>();

    playListInfo = new QList<Song*>();
    rightMenuForSongLW = new QMenu(this);
    rightMenuForMenuLW = new QMenu(this);
    playList = new QMediaPlaylist(this);
    setting = new Setting(this);

    player = new QMediaPlayer(this);

    info_volume = new QLabel("音量");
    info_time = new QLabel("时长");
    info_status = new QLabel("暂停");
    ui->statusbar->addWidget(info_status);

    ui->statusbar->addPermanentWidget(info_volume);
    info_time->setFixedWidth(350);
    ui->statusbar->addWidget(info_time);

    ui->btn_menuName->setHidden(false);

    //把QAction对象添加到菜单上
    rightMenuForSongLW->addAction("播放");
    rightMenuForSongLW->addAction("下一首播放");
    rightMenuForSongLW->addAction("从该列表中删除");
    subMenuForSongLW = rightMenuForSongLW->addMenu("收藏到歌单");

    rightMenuForMenuLW->addAction("转到");
    rightMenuForMenuLW->addAction("修改歌单名称");
    rightMenuForMenuLW->addAction("删除歌单");

    //    vw = new QVideoWidget(this);
    //    player.setVideoOutput(vw);
    //    vw->show();
}

void MainWindow::setMenuLW()
{
    ui->lw_menu->clear();
    qDebug() << "查询menu";

    menuList = sqlUtil->queryMenu();
    for (int i = 0; i < menuList->size();i++){
        setContentForMenu(menuList->at(i));
    }
}

void MainWindow::modifyMenuName(Menu *menu)
{
    MyDialog dlg(this,"修改歌单");
    dlg.exec();
    if (dlg.ok){
        menu->mname = dlg.menuName;
        qDebug() << "更新menu" << sqlUtil->updateMenu(menu);
        setMenuLW();
        ui->lbl_menu->setText(dlg.menuName);
    }
}

void MainWindow::setContentForMenu(Menu*menu)
{
    MyItem * myitem = setItemForLW(ui->lw_menu,menu->mname);
    connect(myitem,&MyItem::listItemClicked,[=](){
        ui->lw_menu->clearSelection();

        ui->btn_menuName->setHidden(false);
        currentMenu = menu->mid;

        songList = sqlUtil->querySongs(menu->mid);
        ui->lbl_menu->setText(menu->mname);
        setSongLW(3);

        disconnect(ui->btn_menuName,0,0,0);
        connect(ui->btn_menuName,&QPushButton::clicked,[=](){
            modifyMenuName(menu);
        });
    });
    connect(myitem,&MyItem::listItemRightClicked,[=](){
        QList<QAction *> localActions = rightMenuForMenuLW->actions();
        disconnect(localActions[0],0,0,0);
        disconnect(localActions[1],0,0,0);
        disconnect(localActions[2],0,0,0);

        connect(localActions[0],&QAction::triggered,[=](){
            //跳转到
            emit myitem->listItemClicked();
        });
        connect(localActions[1],&QAction::triggered,[=](){
            //修改歌单名称
            modifyMenuName(menu);
        });
        connect(localActions[2],&QAction::triggered,[=](){
            //删除歌单
            qDebug() << "删除menu: " << sqlUtil->deleteMenu(menu->mid) << "删除menusong"<<sqlUtil->deleteMenuSong(menu->mid);
            setMenuLW();
        });
        rightMenuForMenuLW->exec(QCursor::pos());
    });
}

void MainWindow::setSongLW(int choice)
{
    ui->lw_song->clear();
    if (choice == 0){
        for (int i = 0; i < playListInfo->size();i++){
            setContentForSong(playListInfo->at(i),i,choice);
        }
    }
    else{
        for (int i = 0; i < songList->size();i++){
            setContentForSong(songList->at(i),i,choice);
        }
    }
}

void MainWindow::setListAndPlay(int index)
{
    playListInfo = new QList<Song*>(*songList);
    playList->clear();
    for (int j = 0; j < songList->size();j++){
        playList->addMedia(QUrl::fromLocalFile(songList->at(j)->sdirectory));
    }
    player->setPlaylist(playList);
    playList->setCurrentIndex(index);
    song = playListInfo->at(index);
    play(song);
}

void MainWindow::setContentForSong(Song *song, int index,int choice)
{
    MyItem * item = setItemForLW(ui->lw_song,song->sname);

    if(choice != 0){
        connect(item,&MyItem::listItemClicked,[=](){
            ui->lw_song->clearSelection();
            setListAndPlay(index);
        });
    }
    else{
        connect(item,&MyItem::listItemClicked,[=](){
            ui->lw_song->clearSelection();
            playList->setCurrentIndex(index);
            play(song);
        });
    }

    connect(item,&MyItem::listItemRightClicked,[=](){
        qDebug() << "点击了右键,弹出动作框";
        //设置信号槽
        QList<QAction *> localActions = rightMenuForSongLW->actions();
        disconnect(localActions[0],&QAction::triggered,0,0);
        disconnect(localActions[1],&QAction::triggered,0,0);
        disconnect(localActions[2],&QAction::triggered,0,0);
        //播放
        connect(localActions[0],&QAction::triggered,[=](){
            setListAndPlay(index);
        });
        //下一首播放
        connect(localActions[1],&QAction::triggered,[=](){
            playList->insertMedia(playList->currentIndex()+1,QUrl::fromLocalFile(song->sdirectory));
            playListInfo->insert(songCurrentIndex+1,song);

            QMessageBox::information(this,"info","添加成功");
        });
        //删除
        connect(localActions[2],&QAction::triggered,[=](){
            switch (choice) {
            case 0:
                playListInfo->takeAt(index);
                playList->removeMedia(index);
                break;
            case 3:
                qDebug() << "删除menusong: " << sqlUtil->deleteMenuSong(currentMenu,song->sid);
                break;
            case 2:
                qDebug() << "删除本地: " << sqlUtil->deleteSong(song->sid);
                qDebug() << "删除最近: " << sqlUtil->deleteRecentSong(song->sid);
                break;
            case 1:
                qDebug() << "删除最近: " << sqlUtil->deleteRecentSong(song->sid);
                break;
            }
            ui->lw_song->takeItem(index);
        });
        //收藏到
        subMenuForSongLW->clear();
        for (int i = 0; i < menuList->size(); i++){
            QString name = menuList->at(i)->mname;
            int mid = menuList->at(i)->mid;
            QAction*action = subMenuForSongLW->addAction(name);
            connect(action,&QAction::triggered,[=](){
                QList<Song*> * sl = sqlUtil->querySongs(mid);
                for (int i = 0; i < sl->size(); i++){
                    if(sl->at(i)->sid == song->sid){
                        //如果该歌单已经包含这首歌，则不做动作
                        return;
                    }
                }

                qDebug() << "添加menusong" << sqlUtil->addMenuSong(mid,song->sid);
            });
        }
        rightMenuForSongLW->exec(QCursor::pos());
    });
}

MyItem *MainWindow::setItemForLW(QListWidget * widget, QString name)
{
    MyItem * myitem = new MyItem(this);
    QListWidgetItem* item = new QListWidgetItem(name);
    widget->addItem(item);
    widget->setItemWidget(item,myitem);
    return myitem;
}

void MainWindow::play(Song* song)
{
    qDebug() << "开始播放: " << playListInfo->at(songCurrentIndex)->sname ;
    ui->lbl_info->setText(formatLabelText(song->sname));

    qDebug() <<" 插入recentsong "<<sqlUtil->deleteRecentSong(song->sid) << sqlUtil->addRecentSongs(song);
    player->play();
}


void MainWindow::loadLastInfo()
{
    int lastPosition = 0;
    int volume;
    QList<int>sids;
    song = new Song;
    //初始化配置
    volume = setting->initSettings(song,&sids);
    qDebug() << "加载配置文件，初始化设置";

    //加载上次正在播放的歌曲信息
    ui->lbl_info->setText(formatLabelText(song->sname));
    info_time->setText(song->stime);

    //设置播放的位置
    qDebug() << "lastPosition = " << lastPosition << " volume = " << volume << " song = " << song->toString();

    //设置音量
    player->setVolume(volume);
    ui->volumeBar->setValue(volume);
    info_volume->setText(QString("音量：%1").arg(volume));

    //设置播放列表
//    for (int i = 0; i < sids.size();i++){
//        qDebug() << "sids[i] = "<< sids[i];
//        Song *song1 = sqlUtil->querySongByID(sids[i]);
//        playListInfo->append(song);
//        playList->addMedia(QUrl::fromLocalFile(song1->sdirectory));
//        if (song1->sid == song->sid){
//            playList->setCurrentIndex(i);
//        }
//    }

    playListInfo->append(song);
    playList->addMedia(QUrl::fromLocalFile(song->sdirectory));
    player->setPlaylist(playList);
    songCurrentIndex = 0;

    qDebug() << "初始化完成";
}

void MainWindow::test()
{
    //    //    ui->le_test->setHidden(true);
    //        QMediaPlaylist * l = new QMediaPlaylist(this);
    //        l->addMedia(QUrl::fromLocalFile("C:\\CloudMusic\\麻枝准 - 渚～坂の下の別れ.flac"));
    //        l->addMedia(QUrl::fromLocalFile("C:\\CloudMusic\\折戸伸治 - 潮鳴りII.flac"));
    //        l->addMedia(QUrl::fromLocalFile("C:\\CloudMusic\\麻枝准 - 渚～坂の下の別れ.flac"));
    //        l->addMedia(QUrl::fromLocalFile("C:\\CloudMusic\\折戸伸治 - 潮鳴りII.flac"));

    //        QMediaPlayer *p = new QMediaPlayer(this);
    //        p->setVolume(30);
    //        p->setPlaylist(l);
    //        p->play();

    //        connect(ui->btn_pre,&QPushButton::clicked,[=](){
    //            qDebug() << "上一首";
    //            l->previous();
    //        });
    //        connect(ui->btn_post,&QPushButton::clicked,[=](){
    //            qDebug() << "下一首";
    //            l->next();
    //        });

    connect(ui->btn_test,&QPushButton::clicked,[=](){/*
               l->setCurrentIndex(ui->le_test->text().toInt());
               qDebug() << "size = " << l->mediaCount() << "current = " << l->currentIndex();*/
        //            QString sql = ui->le_test->text();
        //            qDebug() << sql << sqlUtil->testSQL(sql);
        //qDebug() << "size = " << playList->mediaCount() << "current = " << playList->currentIndex();
        qDebug() << "songCurrentIndex" << songCurrentIndex;
        qDebug() << "mediaCount" << playList->mediaCount();
        qDebug() << "playListInfo size" << playListInfo->size();
        qDebug() << "currentMenu" << currentMenu;
    });
}

QString MainWindow::formatLabelText(QString text)
{
    QFontMetrics fm = ui->lbl_info->fontMetrics();
    QString fixed = text;
    if (fm.horizontalAdvance(text) > ui->lbl_info->width()){
        fixed = fm.elidedText(text,Qt::ElideRight,ui->lbl_info->width());
    }
    return fixed;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::querySongList()
{
    ui->lw_song->clear();
    ui->lbl_menu->setText("播放列表");
    ui->btn_menuName->setHidden(true);

    setSongLW(0);
}

void MainWindow::queryAllSongs()
{
    ui->lw_song->clear();
    ui->lbl_menu->setText("本地歌曲");
    ui->btn_menuName->setHidden(true);

    songList = sqlUtil->queryAllSongs();
    setSongLW(2);
}

void MainWindow::queryRecentSongs()
{
    ui->lw_song->clear();
    ui->lbl_menu->setText("最近播放");
    ui->btn_menuName->setHidden(true);

    songList = sqlUtil->queryRecentSongs();
    setSongLW(1);
}

void MainWindow::initConnections()
{
    connect(ui->btn_add,&QPushButton::clicked,[=](){
        qDebug() << "添加歌单";
        MyDialog dlg;
        dlg.exec();
        if (dlg.ok){
            qDebug() << "添加menu" << sqlUtil->addMenu(dlg.menuName);
            setMenuLW();
        }
    });
    connect(ui->btn_play,&QPushButton::clicked,[=](){
        if(ui->btn_play->text()=="暂停"){
            ui->btn_play->setText("播放");
            info_status->setText( "暂停");
            player->pause();
        }
        else{
            ui->btn_play->setText("暂停");
            info_status->setText( "正在播放");
            player->play();
        }
    });
    connect(ui->btn_pre,&QPushButton::clicked,[=](){
        qDebug() << "上一首";
        if (playList->currentIndex() <= 0)
            playList->setCurrentIndex(playList->mediaCount()-1);
        else{
            playList->previous();
        }
    });
    connect(ui->btn_post,&QPushButton::clicked,[=](){
        qDebug() << "下一首";
        if (playList->currentIndex() >= playList->mediaCount()-1)
            playList->setCurrentIndex(0);
        else
            playList->next();
    });
    connect(ui->volumeBar,&QSlider::valueChanged,[=](){
        info_volume->setText(QString("音量：%1").arg(ui->volumeBar->value()));
        player->setVolume(ui->volumeBar->value());
    });
    connect(player,&QMediaPlayer::stateChanged,[=](){
        qDebug() << "状态改变" << playList->currentIndex();
        if (player->state()==QMediaPlayer::PausedState){
            ui->btn_play->setText( "播放");
            info_status->setText( "暂停");
        }
        else if(player->state()==QMediaPlayer::PlayingState){

            ui->btn_play->setText( "暂停");
            info_status->setText( "正在播放");
        }
    });

    connect(player,&QMediaPlayer::mediaStatusChanged,[=](){
        //正在加载
        if (player->mediaStatus() == QMediaPlayer::LoadingMedia){
            qDebug() << "正在加载资源";
            info_status->setText( "正在加载资源");
        }
        //加载完成
        else if(player->mediaStatus() == QMediaPlayer::BufferedMedia){
            qDebug() << "加载完成";
            songCurrentIndex = playList->currentIndex();
            qDebug() << "songindex = " << songCurrentIndex;
            ui->lbl_info->setText(formatLabelText(playListInfo->at(songCurrentIndex)->sname));
            info_time->setText(playListInfo->at(songCurrentIndex)->stime);
            info_status->setText( "正在播放");
        }
    });

    MyItem *item0 = setItemForLW(ui->lw_select,"播放列表");
    connect(item0,&MyItem::listItemClicked,[=](){
        ui->btn_menuName->setHidden(true);
        ui->lw_select->clearSelection();
        qDebug()<<"播放列表";
        querySongList();
    });

    MyItem *item1 = setItemForLW(ui->lw_select,"最近的歌");
    connect(item1,&MyItem::listItemClicked,[=](){
        ui->btn_menuName->setHidden(true);
        ui->lw_select->clearSelection();
        qDebug()<<"最近的歌";
        queryRecentSongs();
    });
    MyItem *item2 = setItemForLW(ui->lw_select,"本地歌曲");
    connect(item2,&MyItem::listItemClicked,[=](){
        ui->btn_menuName->setHidden(true);
        ui->lw_select->clearSelection();
        qDebug()<<"本地歌曲";
        queryAllSongs();
    });
    MyItem *item3 = setItemForLW(ui->lw_select,"导入...");
    connect(item3,&MyItem::listItemClicked,[=](){
        QStringList list = QFileDialog::getOpenFileNames(this,"选择文件夹","C:\\CloudMusic","音频文件 (*.mp3 *.flac *wav)");
        QMediaPlayer p;
        QList<Song*> * songs= sqlUtil->queryAllSongs();
        for (int i = 0; i < list.size();i++) {
            bool isContained = false;
            for (int j = 0 ; j < songs->size();j++){
                if (songs->at(j)->sdirectory == list[i]){
                    isContained = true;
                    break;
                }
            }
            if (isContained)
                continue;
            p.setMedia(QUrl::fromLocalFile(list[i]));
            Song song1;
            song1.sname = list[i].midRef(list[i].lastIndexOf('/')+1).toString();
            song1.sdirectory = list[i];
            song1.stime = QString( "4.30");
            qDebug() << "duration: " << song1.stime;
            qDebug() << i << "导入歌曲： " << song1.sname <<sqlUtil->addSong(&song1);
        }
        queryAllSongs();
    });
}

void MainWindow::closeEvent(QCloseEvent *cs)
{
    QMessageBox::StandardButton button;

    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序")),QMessageBox::Yes|QMessageBox::No);

    if(button==QMessageBox::No)
    {
        cs->ignore(); // 忽略退出信号，程序继续进行
    }
    else if(button==QMessageBox::Yes)
    {

        QList<int> * sids = new QList<int>;
        for (int i = 0; i < playListInfo->size();i++){
            sids->append(playListInfo->at(i)->sid);
        }
        song = playListInfo->at(songCurrentIndex);
        setting->saveSettings(song,0,player->volume(),sids);

//        qDebug() << "songCurrentIndex" << songCurrentIndex;
//        qDebug() << "mediaCount" << playList->mediaCount();
//        qDebug() << "playListInfo size" << playListInfo->size();
//        qDebug() << "currentMenu" << currentMenu;
        cs->accept(); // 接受退出信号，程序退出
    }
}


//void MainWindow::clearList(QList<Menu *> *list)
//{
//    for (int i = 0; i < list->size();i++){
//        delete list->at(i);
//    }
//    list->clear();
//    delete list;
//    list = nullptr;
//}

//void MainWindow::clearList(QList<Song *> *list)
//{
//    for (int i = 0; i < list->size();i++){
//        delete list->at(i);
//    }
//    list->clear();
//    delete list;
//    list = nullptr;
//}

