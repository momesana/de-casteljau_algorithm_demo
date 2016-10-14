#ifndef COLORTOOLBUTTON_H
#define COLORTOOLBUTTON_H

#include <QToolButton>

class QPaintEvent;

class ColorToolButton : public QToolButton
{
    Q_OBJECT
public:
    ColorToolButton(QWidget *parent = Q_NULLPTR);
    void setColor(const QColor& color);

private:
    QColor m_color;
    QIcon getIconForColor(const QColor &color, const QSize& size) const;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // COLORTOOLBUTTON_H
