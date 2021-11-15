#ifndef SETTING_H
#define SETTING_H

#include "song.h"

#include <QDir>
#include <QObject>
#include <QSettings>

class Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QObject *parent = nullptr);

    int initSettings(Song* lastSong, QList<int> * sids);
    void saveSettings(Song* lastSong,int lastPosition,int volume,QList<int> * sids);

    QString getSavePath();


    ~Setting(){
        delete settings;
    }
signals:

private:
    QDir dir;
    QString path;
    QSettings * settings;

};

#endif // SETTING_H
