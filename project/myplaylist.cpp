#include "myplaylist.h"

MyPlayList::MyPlayList(QWidget *parent)
{

}

QList<Song *> *MyPlayList::getSongListInfo()
{
    return songList;
}

bool MyPlayList::insertMedia(int pos, Song *song)
{
    songList->insert(pos,song);
    return QMediaPlaylist::insertMedia(pos,QUrl::fromLocalFile(song->sdirectory));
}

bool MyPlayList::addMedia(Song *song)
{
    songList->append(song);
    return QMediaPlaylist::addMedia(QUrl::fromLocalFile(song->sdirectory));
}

