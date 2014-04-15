#ifndef GLOBALS_H
#define GLOBALS_H

#include <QHash>
#include <QString>
#include "items/abstracttreeitem.h"
#include "compound_widgets/template_selection_widget/folderitem.h"

namespace globs {
    extern QHash<QString, AbstractTreeItem *> templateHash;
    extern QHash<QString, FolderItem *> folderHash;
}


#endif // GLOBALS_H
