#ifndef MENU_H
#define MENU_H

#include <QObject>

class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent = nullptr);

    int mid;
    QString mname;

signals:

};

#endif // MENU_H
