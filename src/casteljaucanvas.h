#pragma once

#include <QList>
#include <QPoint>
#include <QWidget>
#include <QBrush>
#include <QPen>

class QMouseEvent;
class QPaintEvent;
class PointModel;

enum Mode {
    DrawPoints,
    DrawTopLevelLines,
    DrawBezierCurve,
    DrawBezierCurveAt,
    AnimateBezierCurve
};

class CasteljauCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit CasteljauCanvas(QWidget *parent = 0);
    void setPointModel(PointModel* model);

protected:
    void mousePressEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent* event);

public slots:
    void updateProgress(qreal progress);
    void clear();
    void clearIntermediates();
signals:
    void pointAdded(QPointF);
private:
    void drawPoints(QPainter &painter);
    void drawTopLevelLines(QPainter &painter);
    void drawBezierCurve(QPainter &painter, qreal progress = 1.0);
    void drawIntermediateLines(QPainter &painter);
    void calculateBezierPoints();
    void calculateIntermediateLines();
    void updateConfiguration();
    void recalculateAndUpdate();

    QList<QPointF> points();

    QColor getPenColor(qreal);

    QList<QPointF> m_calculatedBezierPoints;
    Mode m_mode;
    qreal m_progress;
    QPointF reduceToPoint(const QList<QPointF> &points, qreal progress, bool saveIntermediateLines = false);
    QList<QList<QPointF>> m_intermediates;
    QList<QPointF> reduceByOne(const QList<QPointF> &points, qreal progress);
    PointModel* m_pointModel;
    qreal m_pointRadius;

    bool m_drawPoints;
    QPen m_pointPen;
    QBrush m_pointBrush;
    bool m_connectPoints;
    QPen m_lineBetweenPointsPen;
    bool m_drawBezierCurve;
    QPen m_bezierCurvePen;
    bool m_drawIntermediates;
    bool m_bezierCurveNeedsRecalculation;
    qreal m_animationResolution;

};
