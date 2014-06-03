#include "filemanager.h"

FileManager::FileManager()
{

}

QList<QUrl> FileManager::FileDialog() const
{
    QString selection_filter = "PNG (*.png *.PNG)";
    QString title = "Open image(s)";
    QString path = "/Users/Tossen/Dropbox/med10/Test";

    return file_dialog.getOpenFileUrls(0, title, QUrl::fromLocalFile(path), selection_filter);
}

FileManager::~FileManager()
{

}
