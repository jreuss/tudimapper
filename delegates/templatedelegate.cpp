#include "templatedelegate.h"

#include <QLineEdit>

TemplateDelegate::TemplateDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

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

    QPixmap pixmap("/home/joachim/Pictures/Mario-icon-scaled.png");
    int dx = (option.rect.width()-64) / 2;

    QRect iconRect = QRect(option.rect.x()+dx, option.rect.y()+5, 64, 64 );

    painter->drawPixmap(iconRect, pixmap);

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

