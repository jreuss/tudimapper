#include "filemanager.h"

FileManager::FileManager()
{

}

QList<QUrl> FileManager::FileDialog() const
{
    QString selection_filter = "PNG (*.png *.PNG)";
    QString title = "open image(s)";
    QString path = "/home/joachim/Pictures/";

    return file_dialog.getOpenFileUrls(0, title, path, selection_filter);
}

FileManager::~FileManager()
{

}
