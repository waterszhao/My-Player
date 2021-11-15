#include "myitem.h"

MyItem::MyItem(QWidget *parent) : QWidget(parent)
{

}

void MyItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        {
            emit this->listItemClicked();
        }

}

void MyItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton){
        emit this->listItemRightClicked();
    }
}
