#include "listview.h"

#include <QKeyEvent>
#include <QDebug>

ListView::ListView(QWidget* parent)
    : QListView(parent)
{

}

void ListView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        const int row = currentIndex().row();
        qDebug() << "remove row " << row;
        model()->removeRow(row);
    }
}
