#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#include <QAbstractTableModel>
#include <QFile>

class file_handle : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit file_handle(const QString &path);

};

#endif // FILE_HANDLE_H
