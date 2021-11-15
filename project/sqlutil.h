#ifndef SQLUTIL_H
#define SQLUTIL_H

#include "menu.h"
#include "song.h"

#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QList>
#include <QSqlRecord>

// table: song menu menusong recentsong
// view: ms rs
class SqlUtil : public QObject
{
    Q_OBJECT
public:
    explicit SqlUtil(QString dbName, QObject *parent = nullptr);

    bool createTable();
    //song
    bool addSong(Song*);
    bool deleteSong(int sid);
    QList<Song*> * queryAllSongs();
    Song* querySongByID(int sid);
    //ms
    QList<Song*> * querySongs(int menuId);
    bool addMenuSong(int mid,int sid);
    bool deleteMenuSong(int menu,int sid);
    bool deleteMenuSong(int menu);
    //menu
    bool addMenu(QString name);
    bool deleteMenu(int mid);
    bool updateMenu(Menu*);
    QList<Menu*> * queryMenu();
    //recentsong
    bool addRecentSongs(Song*);
    bool deleteRecentSong(int sid);
    QList<Song*> * queryRecentSongs();

    bool testSQL(QString sql);

    QString getDbName();
signals:

private:
    QSqlDatabase db;

    QString dbName;
};

#endif // SQLUTIL_H
