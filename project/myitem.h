#ifndef MYITEM_H
#define MYITEM_H

#include <QWidget>
#include <QMouseEvent>

class MyItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyItem(QWidget *parent = nullptr);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    signals:
    void listItemClicked();
    void listItemRightClicked();
};

#endif // MYITEM_H
