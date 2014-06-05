#include "templatedelegate.h"

#include <QLineEdit>

#include <items/itemtemplate.h>

TemplateDelegate::TemplateDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    folderIcon = QPixmap(":/icons/folder");
    groupOverlay = QPixmap(":/icons/group_overlay");
    spriteOverlay = QPixmap(":/icons/sprite_overlay");
}

QWidget *TemplateDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setFrame(false);

    return editor;
}

void TemplateDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(value);
}


void TemplateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ( option.state & QStyle::State_Selected )
        painter->fillRect(option.rect, option.palette.highlight());
    else if ( option.state & QStyle::State_MouseOver )
        painter->fillRect(option.rect, option.palette.highlight());
    else {
        painter->fillRect(option.rect, option.palette.button());
    }

    ItemTemplate* itm = static_cast<ItemTemplate*>(index.internalPointer());

    int dx = (option.rect.width()-64) / 2;
    QRect iconRect = QRect(option.rect.x()+dx, option.rect.y()+5, 64, 64 );

    if(itm->importType() == 5) {
        painter->drawPixmap(iconRect, folderIcon.scaled(QSize(64,64), Qt::KeepAspectRatio));
    } else {
        QPixmap pixmap = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));

        float width = (float)pixmap.width();
        float height = (float)pixmap.height();

        if(width > height) {
            height = 64* (height/width);
            width = 64;
        } else {
            width = 64* (width/height);
            height = 64;
        }
        dx = (option.rect.width()-width) / 2;
        int dy = (option.rect.height()-height) / 2 - 5;
        pixmap = pixmap.scaled(width,height);

        iconRect = QRect(option.rect.x()+dx, option.rect.y() + dy, pixmap.width(), pixmap.height() );
        painter->drawPixmap(iconRect, pixmap);

        // draw overlays
        if(itm->importType() == ItemTemplate::Split) {
            painter->drawPixmap(option.rect.x()+5, option.rect.y() + 5, 32, 32, groupOverlay);
            qDebug() << "split";
        }

        // draw overlays
        if(itm->importType() == ItemTemplate::SpriteSheet) {
            painter->drawPixmap(option.rect.x()+ 5, option.rect.y() + 5, 32, 32, spriteOverlay);
            qDebug() << "split";
        }
    }

    //painter->drawPixmap(pixmap);


    QString text = qvariant_cast<QString>(index.data(Qt::DisplayRole));
    QFontMetrics fm(option.font);

    QRect rect = fm.boundingRect(text);
    QString txt = fm.elidedText(text, Qt::ElideRight, option.rect.width()-10);
    //QString txt = QFontMetrics::elidedText();
    QRect rect2 = option.rect;
    rect2.setY(option.rect.y() + option.rect.height() - rect.height()-5);
    rect2.setX(option.rect.x() + 5);

    painter->drawText( rect2, txt );
}

void TemplateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    QString value = edit->text();

    model->setData(index, value, Qt::EditRole);
}

void TemplateDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect.x(), option.rect.y() + 64+5,
                        option.rect.width(), option.rect.height()-64-5);
}

