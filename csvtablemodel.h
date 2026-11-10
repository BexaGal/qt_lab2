#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QList>

class CsvTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    using QAbstractTableModel::QAbstractTableModel;

    // Read-only part (required)
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Editing part
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = {}) override;
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override;

    // CSV
    bool loadFromFile(const QString &path);
    bool saveToFile(const QString &path) const;

private:
    QStringList m_header;           // column titles
    QList<QStringList> m_rows;      // data: one QStringList per row
};

#endif // CSVTABLEMODEL_H
