#include "casteljaucanvas.h"
#include "pointmodel.h"
#include "settings.h"

#include <QMouseEvent>

#include <QDebug>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QStyleOption>
#include <QModelIndex>
#include <QColor>


CasteljauCanvas::CasteljauCanvas(QWidget *parent)
    : QWidget(parent)
    , m_mode(Mode::DrawPoints)
    , m_progress(0)
{
    setMouseTracking(true);

    connect(Settings::instance(), qOverload<const QString&, const QVariant&>(&Settings::updated), this, &CasteljauCanvas::updateConfiguration);
    updateConfiguration();
}

void CasteljauCanvas::setPointModel(PointModel* model)
{
    m_pointModel = model;

    connect(m_pointModel, &PointModel::dataChanged, this, &CasteljauCanvas::recalculateAndUpdate);
    connect(m_pointModel, &PointModel::rowsInserted, this, &CasteljauCanvas::recalculateAndUpdate);
    connect(m_pointModel, &PointModel::rowsRemoved, this, &CasteljauCanvas::recalculateAndUpdate);
    update();
}


void CasteljauCanvas::mousePressEvent(QMouseEvent *event)
{
    emit pointAdded(QPointF(event->pos()));
}

void CasteljauCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    painter.setRenderHints(QPainter::Antialiasing);

    if (m_drawPoints) {
        drawPoints(painter);
        // it doesn't make sense to draw the lines without the points, so this statement here
        if (m_connectPoints)
            drawTopLevelLines(painter);
    }

    if (m_drawBezierCurve && m_progress > 0) {
        drawBezierCurve(painter, m_progress);
    }

    if (m_drawIntermediates)
        drawIntermediateLines(painter);
}

void CasteljauCanvas::drawPoints(QPainter &painter)
{
    painter.save();
    painter.setPen(m_pointPen);
    painter.setBrush(m_pointBrush);

    for (auto point: points()) {
        painter.drawEllipse(point, m_pointRadius, m_pointRadius);
    }

    painter.restore();
}

void CasteljauCanvas::drawTopLevelLines(QPainter &painter)
{
    drawPoints(painter);
    painter.save();
    painter.setPen(Qt::red);

    for (int i = 0; i < points().length() - 1; ++i) {
        auto p1 = points().at(i);
        auto p2 = points().at(i+1);

        painter.drawLine(p1, p2);
    }

    painter.restore();
}

void CasteljauCanvas::drawBezierCurve(QPainter &painter, qreal progress)
{
    painter.setPen(m_bezierCurvePen);

    if (m_bezierCurveNeedsRecalculation) {
        calculateBezierPoints();
        m_bezierCurveNeedsRecalculation = false;
    }

    if (m_calculatedBezierPoints.size() < 2)
        return;

    QPainterPath path;
    const int lastToConsider = qRound((m_calculatedBezierPoints.size()) * progress);
    path.moveTo(m_calculatedBezierPoints.first());
    for (int i = 1; i < lastToConsider; ++i) {
        path.lineTo(m_calculatedBezierPoints.at(i));
        if (progress == 1.0 && i + 1 == lastToConsider)
            path.lineTo(points().last());
    }

    painter.drawPath(path);
}

void CasteljauCanvas::drawIntermediateLines(QPainter &painter)
{
    calculateIntermediateLines();

    if (m_progress == 0 || m_progress == 1)
        return;

    for (int i = 0; i < m_intermediates.size(); ++i) {
        const auto pointList = m_intermediates.at(i);
        if (pointList.size() == 1) {
            painter.setPen(m_bezierCurvePen);
            painter.setBrush(m_bezierCurvePen.color());
            painter.drawEllipse(pointList.first(), m_pointRadius, m_pointRadius);
        } else {
            const QColor penColor = getPenColor((qreal) (i + 1) / (m_intermediates.size() - 1));
            painter.setPen(QPen(penColor, 1));
            for (int j = 0; j < pointList.size() - 1; ++j) {
                const auto p1 = pointList.at(j);
                const auto p2 = pointList.at(j+1);
                painter.drawLine(p1, p2);
            }
        }
    }
}

void CasteljauCanvas::calculateIntermediateLines() {
    m_intermediates.clear();
    if (points().size() < 3)
        return;
    reduceToPoint(points(), m_progress, true);
}

void CasteljauCanvas::updateConfiguration()
{
    const Settings* settings = Settings::instance();
    m_drawPoints = settings->value("drawPoints", true).toBool();
    const QColor bezierPenColor = settings->value("bezierCurvePenColor", QColor(Qt::darkGray)).value<QColor>();
    const qreal bezierPenWidth = settings->value("bezierCurvePenWidth", 2.0).toDouble();
    m_bezierCurvePen = QPen(bezierPenColor, bezierPenWidth);
    m_drawPoints = settings->value("drawPoints", true).toBool();
    m_connectPoints = settings->value("connectPoints", true).toBool();
    m_drawIntermediates = settings->value("drawIntermediateLines", true).toBool();
    m_pointRadius = settings->value("pointRadius", 2).toDouble();
    const QColor m_pointPenColor = settings->value("pointColor", QColor(Qt::black)).value<QColor>();
    m_pointPen = QPen(m_pointPenColor, 1);
    const QColor m_pointBrushColor = settings->value("pointFill", QColor(Qt::lightGray)).value<QColor>();
    m_pointBrush = QBrush(m_pointBrushColor);

    bool drawBezierCurve = settings->value("drawBezierCurve", true).toBool();
    double animationResolution = settings->value("animationResolution", 0.001).toDouble();
    if (drawBezierCurve != m_drawBezierCurve || animationResolution != m_animationResolution) {
        m_drawBezierCurve = drawBezierCurve;
        m_animationResolution = animationResolution;
        m_bezierCurveNeedsRecalculation = true;
    }

    update();
}

void CasteljauCanvas::recalculateAndUpdate()
{
    if (m_drawBezierCurve) {
        m_bezierCurveNeedsRecalculation = true;
    }

    update();
}

QList<QPointF> CasteljauCanvas::points()
{
    return m_pointModel->points();
}

QColor CasteljauCanvas::getPenColor(qreal value)
{
    const qreal hue = value * 0.7;
    return QColor::fromHslF(hue, 1, 0.5);
}

void CasteljauCanvas::calculateBezierPoints()
{
    clearIntermediates();

    if (points().size() < 3)
        return;

    for (qreal progress = 0.0; progress <= 1.0; progress += m_animationResolution) {
        m_calculatedBezierPoints.append(reduceToPoint(points(), progress));
    }
}

void CasteljauCanvas::updateProgress(qreal progress)
{
    if (m_progress != progress) {// FIXME: introduce safe range
        m_progress = progress;
        update();
    }
}

void CasteljauCanvas::clear()
{
    m_pointModel->clear();
    clearIntermediates();
    update();
}

void CasteljauCanvas::clearIntermediates()
{
    m_calculatedBezierPoints.clear();
    m_intermediates.clear();
}

QPointF CasteljauCanvas::reduceToPoint(const QList<QPointF> &points, qreal progress, bool saveIntermediateLines)
{
    QList<QPointF>intermediatePoints = points;

    while (intermediatePoints.size() > 1) {
        intermediatePoints = reduceByOne(intermediatePoints, progress);
        if (saveIntermediateLines)
            m_intermediates.append(intermediatePoints);
    }

    Q_ASSERT(intermediatePoints.size() == 1);
    return intermediatePoints.at(0);
}

QList<QPointF> CasteljauCanvas::reduceByOne(const QList<QPointF> &points, qreal progress)
{
    QList<QPointF> intermediate;

    for (int i = 0; i < points.length() - 1; ++i) {
        intermediate << QLineF(points.at(i), points.at(i+1)).pointAt(progress);
    }

    Q_ASSERT(intermediate.size() == points.size() - 1);
    return intermediate;
}

