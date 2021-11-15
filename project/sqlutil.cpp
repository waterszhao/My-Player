#include "sqlutil.h"

#include <QTime>
#include <QDebug>

SqlUtil::SqlUtil(QString dbName, QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    this->dbName = dbName;
    db.setDatabaseName(dbName);
    db.open();
}

bool SqlUtil::createTable()
{
    QString tableMenu = QString("CREATE TABLE IF NOT EXISTS menu("
                          " mid integer PRIMARY KEY AUTOINCREMENT, "
                          " mname varchar(45));");
    QString tableMenuSong = "CREATE TABLE IF NOT EXISTS menusong("
                            " id integer PRIMARY KEY AUTOINCREMENT, "
                            " mid integer, "
                            " sid integer);";
    QString tableSong = QString( "CREATE TABLE IF NOT EXISTS song("
                   " sid integer PRIMARY KEY AUTOINCREMENT, "
                   " sname varchar(45),"
                   " stime varchar(10),"
                   " sdirectory varchar(100))");
    QString recentSong = QString( "CREATE TABLE IF NOT EXISTS recentsong("
                   " id integer PRIMARY KEY AUTOINCREMENT,"
                                  "sid integer,"
                                  "date varchar(20));");
    QString viewMS = "CREATE VIEW IF NOT EXISTS 'ms'"
            " AS SELECT menu.*,song.* FROM menu,song,menusong"
            " WHERE menu.mid = menusong.mid AND song.sid = menusong.sid";
    QString viewRS = "CREATE VIEW IF NOT EXISTS 'rs'"
            " AS SELECT recentsong.*,song.* FROM recentsong,song"
            " WHERE recentsong.sid = song.sid";

    QSqlQuery sq;
    return (sq.exec(tableMenu) && sq.exec(tableSong) && sq.exec(recentSong) && sq.exec(tableMenuSong) && sq.exec(viewMS) && sq.exec(viewRS));

}

bool SqlUtil::addSong(Song *song)
{
    QSqlQuery sq;
    QString sql = QString("insert into song values(NULL,'%1','%2','%3')").arg(song->sname).arg(song->stime).arg(song->sdirectory);
    qDebug() << sql;
    return sq.exec(sql);
}

bool SqlUtil::deleteSong(int sid)
{
    QSqlQuery sq;
    QString sql = QString("delete from song where sid = '%1'").arg(sid);
    return sq.exec(sql);
}

QList<Song *> *SqlUtil::queryAllSongs()
{
    QSqlQueryModel sqm;
    QString sql = QString("select * from song");
    sqm.setQuery(sql);

    QList<Song*>* list = new QList<Song*>;
    for (int i = 0; i < sqm.rowCount();i++){
        Song* song = new Song;
        song->sid = sqm.record(i).value("sid").toInt();
        song->sname = sqm.record(i).value("sname").toString();
        song->stime = sqm.record(i).value("stime").toString();
        song->sdirectory = sqm.record(i).value("sdirectory").toString();
        list->append(song);
    }
    return list;
}

Song *SqlUtil::querySongByID(int sid)
{
    QSqlQueryModel sqm;
    QString sql = QString("select * from song where sid = %1").arg(sid);
    sqm.setQuery(sql);

    Song* song = new Song;
    song->sid = sqm.record(0).value("sid").toInt();
    song->sname = sqm.record(0).value("sname").toString();
    song->stime = sqm.record(0).value("stime").toString();
    song->sdirectory = sqm.record(0).value("sdirectory").toString();
    return song;
}

QList<Song *> *SqlUtil::querySongs(int menuId)
{
    QSqlQueryModel sqm;
    QString sql = QString("select * from ms where mid = %1").arg(menuId);
    sqm.setQuery(sql);

    QList<Song*>* list = new QList<Song*>;
    for (int i = 0; i < sqm.rowCount();i++){
        Song* song = new Song;
        song->sid = sqm.record(i).value("sid").toInt();
        song->sname = sqm.record(i).value("sname").toString();
        song->stime = sqm.record(i).value("stime").toString();
        song->sdirectory = sqm.record(i).value("sdirectory").toString();
        list->append(song);
    }
    return list;
}

bool SqlUtil::addMenuSong(int mid, int sid)
{
    QSqlQuery sq;
    QString sql = QString("insert into menusong values(NULL,'%1','%2')").arg(mid).arg(sid);
    return sq.exec(sql);
}

bool SqlUtil::deleteMenuSong(int mid, int sid)
{
    QSqlQuery sq;
    QString sql = QString("delete from menusong where mid = '%1' and sid = '%2'").arg(mid).arg(sid);
    return sq.exec(sql);
}

bool SqlUtil::deleteMenuSong(int menu)
{
    QSqlQuery sq;
    QString sql = QString("delete from menusong where mid = '%1'").arg(menu);
    return sq.exec(sql);
}

bool SqlUtil::addMenu(QString name)
{
    QSqlQuery sq;
    QString sql = QString("insert into menu values(NULL,'%1')").arg(name);
    return sq.exec(sql);
}

bool SqlUtil::deleteMenu(int mid)
{
    QSqlQuery sq;
    QString sql = QString("delete from menu where mid = '%1'").arg(mid);
    return sq.exec(sql);
}

bool SqlUtil::updateMenu(Menu* menu)
{
    QSqlQuery sq;
    QString sql = QString("update menu set mname = '%1' where mid = '%2'").arg(menu->mname).arg(menu->mid);
    qDebug() << sql;
    return sq.exec(sql);
}

QList<Menu *> *SqlUtil::queryMenu()
{
    QSqlQueryModel sqm;
    QString sql = QString("select * from menu");
    sqm.setQuery(sql);

    QList<Menu*>* list = new QList<Menu*>;
    for (int i = 0; i < sqm.rowCount();i++){
        Menu* menu = new Menu;
        menu->mid = sqm.record(i).value("mid").toInt();
        menu->mname = sqm.record(i).value("mname").toString();
        list->append(menu);
    }

    return list;
}

bool SqlUtil::addRecentSongs(Song * song)
{
    QSqlQuery sq;
    QString sql = QString("insert into recentsong values(NULL,'%1','%2')").arg(song->sid).arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    return sq.exec(sql);
}

bool SqlUtil::deleteRecentSong(int sid)
{
    QSqlQuery sq;
    QString sql = QString("delete from recentsong where sid = '%1'").arg(sid);
    return sq.exec(sql);
}

QList<Song *> *SqlUtil::queryRecentSongs()
{
    QSqlQueryModel sqm;
    QString sql = QString("select * from rs order by date desc");
    sqm.setQuery(sql);

    QList<Song*>* list = new QList<Song*>;
    for (int i = 0; i < sqm.rowCount();i++){
        Song* song = new Song;
        song->sid = sqm.record(i).value("sid").toInt();
        song->sname = sqm.record(i).value("sname").toString();
        song->stime = sqm.record(i).value("stime").toString();
        song->sdirectory = sqm.record(i).value("sdirectory").toString();
        list->append(song);
    }
    return list;
}

bool SqlUtil::testSQL(QString sql)
{
    QSqlQuery sq;
    return sq.exec(sql);
}

QString SqlUtil::getDbName()
{
    return this->dbName;
}
