#include "song.h"


Song::Song(QObject *parent) : QObject(parent)
{

}

QString Song::toString()
{
    QString str = QString("sid = %1 sname = %2 stime = %3 sdirectory = %4").arg(sid).arg(sname,stime,sdirectory);
    return str;
}
