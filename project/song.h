#ifndef SONG_H
#define SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT
public:
    explicit Song(QObject *parent = nullptr);

    QString toString();
    int sid;
    QString sname;
    QString sdirectory;
    QString stime;
signals:

};

#endif // SONG_H
