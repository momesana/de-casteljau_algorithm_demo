#pragma once

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class Animation;
class PointModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void progressChanged(qreal progress);

private:
    void readSettings();
    void setupConnections();
    QIcon getIconForColor(const QColor& color, const QSize& size) const;

    Ui::MainWindow *ui;
    Animation *m_animation;
    PointModel* m_pointModel;
};

