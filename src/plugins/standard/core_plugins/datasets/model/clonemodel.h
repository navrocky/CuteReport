#ifndef CLONEMODEL_H
#define CLONEMODEL_H

#include <QAbstractItemModel>

class CloneModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CloneModel(QObject *parent = 0);
    static CloneModel *instance();

    void populate(QAbstractItemModel *sourceModel);

    bool isPopulated() const { return m_populated; }

    QList< QList <QVariant> > items() const { return m_items; }

signals:

public slots:


    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;


private:
    int m_columnCount;
    bool m_populated;

    QList< QList <QVariant> > m_items;
};

#endif // CLONEMODEL_H
