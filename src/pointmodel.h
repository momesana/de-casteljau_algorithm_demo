#pragma once

#include <QList>
#include <QPointF>
#include <QAbstractListModel>
#include <QObject>

class PointModel : public QAbstractListModel
{
    Q_OBJECT
public:
    PointModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void appendPoint(QPointF point);
    void insertPoint(QPointF point, int index);
    void removePoint(QPointF point, int index);
    void clear();
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    const QList<QPointF> points() const;
private:
    QList<QPointF> m_points;
};
