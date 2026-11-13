#include "csvtablemodel.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>


// ---------- Required methods ----------

int CsvTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_rows.size();
}

int CsvTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_header.size();
}

// The view asks for the value of each cell here.
QVariant CsvTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return {};
    return m_rows[index.row()][index.column()];
}

// Column titles come from the CSV header. Rows are numbered 1, 2, 3...
QVariant CsvTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};
    if (orientation == Qt::Horizontal)
        return m_header.value(section);
    return section + 1;
}

// ---------- Editing ----------

// Called when the user finishes editing a cell.
bool CsvTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    m_rows[index.row()][index.column()] = value.toString();
    emit dataChanged(index, index);   // tell the views to redraw this cell
    return true;
}

// Without the "editable" flag, the user cannot edit cells.
Qt::ItemFlags CsvTableModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

// Rule: always change the data BETWEEN begin...() and end...().
bool CsvTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_rows.insert(row, QStringList(m_header.size()));  // empty row
    endInsertRows();
    return true;
}

bool CsvTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    m_rows.remove(row, count);
    endRemoveRows();
    return true;
}

// ---------- CSV ----------

// Splits CSV text into rows and fields.
// We do not use split(',') because fields can contain commas or new lines
// inside quotes, for example: "Smith, John"
static QList<QStringList> parseCsv(const QString &text)
{
    QList<QStringList> rows;
    QStringList row;
    QString field;
    bool inQuotes = false;

    for (int i = 0; i < text.size(); ++i) {
        const QChar c = text[i];

        if (c == '"') {
            if (inQuotes && i + 1 < text.size() && text[i + 1] == '"') {
                field += '"';          // "" inside quotes = one quote
                ++i;
            } else {
                inQuotes = !inQuotes;  // opening or closing quote
            }
        } else if (c == ',' && !inQuotes) {
            row << field;              // end of field
            field.clear();
        } else if (c == '\n' && !inQuotes) {
            row << field;              // end of row
            field.clear();
            rows << row;
            row.clear();
        } else if (c == '\r' && !inQuotes) {
            // ignore: Windows files use \r\n
        } else {
            field += c;
        }
    }

    if (!field.isEmpty() || !row.isEmpty()) {   // last line without new line
        row << field;
        rows << row;
    }
    return rows;
}

// A field with a comma, quote or new line must be put in quotes.
static QString escapeField(QString field)
{
    if (field.contains(',') || field.contains('"') || field.contains('\n')) {
        field.replace('"', "\"\"");
        return '"' + field + '"';
    }
    return field;
}

bool CsvTableModel::loadFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QList<QStringList> all = parseCsv(in.readAll());
    if (all.isEmpty())
        return false;

    beginResetModel();                  // "everything changes, reload all"
    m_header = all.takeFirst();         // first line = header
    m_rows = all;
    for (QStringList &r : m_rows) {     // every row must have the same size
        while (r.size() < m_header.size()) r << QString();
        while (r.size() > m_header.size()) r.removeLast();
    }
    endResetModel();
    return true;
}

bool CsvTableModel::saveToFile(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    // Write one row: escape each field, join with commas.
    auto writeRow = [&out](const QStringList &row) {
        QStringList fields;
        for (const QString &f : row)
            fields << escapeField(f);
        out << fields.join(',') << '\n';
    };

    writeRow(m_header);
    for (const QStringList &row : m_rows)
        writeRow(row);
    return true;
}

bool CsvTableModel::createFile(const QString &path, const QStringList &header)
{
    // Create the folder if it does not exist yet
    if (!QDir().mkpath(QFileInfo(path).absolutePath()))
        return false;

    beginResetModel();
    m_header = header.isEmpty() ? QStringList{"Column 1", "Column 2", "Column 3"} : header;
    m_header.resize(3);              // always exactly 3 titles
    m_rows.clear();                 // new file = no data rows
    endResetModel();

    return saveToFile(path);        // writes the header line to the file
}
