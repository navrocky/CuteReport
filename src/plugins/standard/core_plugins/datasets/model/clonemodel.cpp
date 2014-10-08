#include "clonemodel.h"
#include <QDebug>

static CloneModel *__instance = 0;

CloneModel::CloneModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_columnCount(0),
    m_populated(false)
{
    __instance = this;
    qDebug() << "createClone";
}

CloneModel * CloneModel::instance()
{
    return __instance;
}

void CloneModel::populate(QAbstractItemModel *sourceModel)
{
    m_populated = false;
    m_columnCount = sourceModel->columnCount();

    emit beginResetModel();
    m_items.clear();

    for (int row = 0; row < sourceModel->rowCount(); row++) {
        QList<QVariant> cols;
        for (int col = 0; col < sourceModel->columnCount(); col++) {
            cols.append(sourceModel->index(row, col).data(Qt::DisplayRole));
        }

        m_items.append(cols);
    }

    qDebug() << m_items;
    m_populated = true;
    emit endResetModel();
}


QModelIndex CloneModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex CloneModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int CloneModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int CloneModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant CloneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant(QVariant::Invalid);
    }

    if (role == Qt::DisplayRole) {
        return m_items.at(index.row()).at(index.column());
    }

    return QVariant();
}
