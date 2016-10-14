#include "colortoolbutton.h"

#include <QPaintEvent>
#include <QPainter>

ColorToolButton::ColorToolButton(QWidget* parent)
    : QToolButton(parent)
{
    setColor(Qt::darkGray);
}

void ColorToolButton::setColor(const QColor &color)
{
    if (color.isValid() && m_color != color) {
        m_color = color;
        update();
    }
}

QIcon ColorToolButton::getIconForColor(const QColor &color, const QSize& size) const
{
    QPixmap px(size);
    px.fill(color);
    return QIcon(px);
}

void ColorToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(0.5, 0.5);

    painter.setBrush(m_color);
    painter.setPen(Qt::NoPen);

    const int margin = 3;
    const QRectF cr = this->contentsRect().adjusted(margin,margin,-margin-1,-margin-1);
    const qreal side = qMin(cr.width(), cr.height());
    QRectF r(0, 0, side, side);
    r.moveCenter(cr.center());
    painter.drawRoundedRect(r, 2, 2);
}


