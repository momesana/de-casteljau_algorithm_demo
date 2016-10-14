#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

class QKeyEvent;

class ListView : public QListView
{
    Q_OBJECT
public:
    ListView(QWidget* parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // LISTVIEW_H
