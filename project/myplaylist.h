#ifndef MYPLAYLIST_H
#define MYPLAYLIST_H


#include <QObject>
#include <QMediaPlaylist>
#include <song.h>

class MyPlayList : public QMediaPlaylist
{
    Q_OBJECT
public:
    explicit MyPlayList(QWidget *parent = nullptr);

    QList<Song*> * getSongListInfo();

    bool insertMedia(int pos, Song*song);
    bool addMedia(Song*song);

    int songCount();
private:
    QList<Song*> * songList;
signals:

};

#endif // MYPLAYLIST_H
