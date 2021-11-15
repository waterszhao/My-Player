#include "setting.h"


Setting::Setting(QObject *parent) : QObject(parent)
{
    QDir dir;
    path = QString("%1/My Player Fixed/settings.ini").arg(dir.homePath());

    settings =new QSettings(path,QSettings::IniFormat);
}

int Setting::initSettings(Song *lastSong, QList<int> *sids)
{
    int id = settings->value("lastSong/id",0).toInt();
    QString name = settings->value("lastSong/name","******").toString();
    QString time = settings->value("lastSong/time","------").toString();
    QString directory = settings->value("lastSong/directory","######").toString();

    lastSong->sid = id;
    lastSong->sname = name;
    lastSong->stime = time;
    lastSong->sdirectory = directory;

    QStringList sid = settings->value("songlist").toString().split(" ");
    for (int i = 0; i <sids->size();i++){
        sids->append(sid[i].toInt());
    }

    return settings->value("volume",30).toInt();
}

void Setting::saveSettings(Song *lastSong, int lastPosition, int volume, QList<int> *sids)
{
    settings->setValue("volume",volume);
    settings->setValue("lastPosition",lastPosition);

    settings->setValue("lastSong/id",lastSong->sid);
    settings->setValue("lastSong/name",lastSong->sname);
    settings->setValue("lastSong/directory",lastSong->sdirectory);
    settings->setValue("lastSong/time",lastSong->stime);

    QString sid;

    for (int i = 0 ; i< sids->size();i++){
        sid.append(QString(sids->at(i)));
    }

    settings->setValue("songList",sid);

}


QString Setting::getSavePath()
{
    return path;
}

