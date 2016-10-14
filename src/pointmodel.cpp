#include "pointmodel.h"

#include <QDebug>

PointModel::PointModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PointModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_points.size();
}

QVariant PointModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const QPointF p = m_points.at(index.row());
    return tr("(%1,%2)").arg(p.x()).arg(p.y());
}

QVariant PointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || section != 0 || role != Qt::DisplayRole)
        return QVariant();

    return tr("Points");

}


void PointModel::appendPoint(QPointF point)
{
    beginInsertRows(QModelIndex(), m_points.size(), m_points.size());
    m_points.append(point);
    endInsertRows();
}

// TODO
void PointModel::insertPoint(QPointF, int)
{

}

// TODO
void PointModel::removePoint(QPointF, int)
{

}

void PointModel::clear()
{
    beginResetModel();
    m_points.clear();
    endResetModel();
}

bool PointModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + (count - 1));
    for (int i = 0; i < count; ++i) {
        m_points.removeAt(row);
    }
    endRemoveRows();

    return true;
}

const QList<QPointF> PointModel::points() const
{
    return m_points;
}
