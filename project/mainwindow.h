#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "setting.h"
#include "sqlutil.h"
#include "myitem.h"
#include "song.h"

#include <QMainWindow>
#include <QTextLine>
#include <QAction>
#include <QLabel>
#include <QDebug>
#include <QDir>
#include <QListWidget>
#include <QMediaPlayer>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void querySongList();
    void queryAllSongs();
    void queryRecentSongs();

    void modifyMenuName(Menu *menu);

private:
    Ui::MainWindow *ui;

    SqlUtil * sqlUtil;
    Setting * setting;

    QList<Song*> * songList;
    QList<Menu*> * menuList;
    int currentMenu =0;
    int songCurrentIndex = 0;

    Song * song;

    QString path;//--

    QLabel * info_volume;
    QLabel * info_time;
    QLabel * info_status;

    QMenu * rightMenuForSongLW;
    QMenu * subMenuForSongLW;

    QMenu * rightMenuForMenuLW;

    QMediaPlayer * player;
    QMediaPlaylist * playList;
    QList<Song*> * playListInfo;

    //加载上次退出时配置
    void loadLastInfo();
    //初始化组件
    void initWidgets();
    //设置歌单
    void setMenuLW();
    //为歌单监听器
    void setContentForMenu(Menu*menu);
    //设置歌曲列表内容
    //0 列表 1 最近 2 本地 3 歌单
    void setSongLW(int choice);
    //为列表项设置监听器
    void setContentForSong(Song* song, int index,int choice);

    MyItem* setItemForLW(QListWidget*item,QString name);

    //初始化基础配置信息
    void initSettings();
    //初始化按钮事件
    void initConnections();

    //关闭事件
     void closeEvent(QCloseEvent* cs);
     //关闭时保存设置
     void saveSettings();

     void play(Song* song);

     QString formatLabelText(QString text);

     void clearList(QList<Song*> * list);
     void clearList(QList<Menu*> * list);

     void setListAndPlay(int index);

     void test();
};
#endif // MAINWINDOW_H
