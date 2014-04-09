#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QString>
#include <QDebug>

class FileManager
{
public:
    FileManager();
    QList<QUrl> FileDialog() const;
    ~FileManager();

private:
    QList<QUrl> file_list;
    QFileDialog file_dialog;
};

#endif // FILEMANAGER_H
