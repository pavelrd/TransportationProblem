#include <QtGui>
#include <QPlainTextEdit>
#include <QApplication>
#include <QPalette>

#include "ItemDelegate.hpp"
 
ItemDelegate::ItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
}

void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
            const QModelIndex& index) const
{

    QStringList itemText = index.data().toString().split(" ");

    int len = itemText.length();

    QStyleOptionViewItem optionViewItemLeft;
    optionViewItemLeft.text = itemText.at(0);

    QStyleOptionViewItem optionViewItemRight;

    QRect rightRect = option.rect;

    if( len == 3 )
    {

        optionViewItemRight.text = itemText.at(2);

    }
    else
    {
        optionViewItemRight.text = "";
    }

    QRect leftRect  = option.rect;

    int x1, x2, y1, y2;

    leftRect.getCoords(&x1,&y1,&x2,&y2);

    // qDebug() << x << " " << x1 << " " << y << " " << y1;

    leftRect.setCoords( x1, y1, x2 - ((x2-x1)/2), y2 );

    rightRect.setCoords( x1 + ((x2-x1)/2), y1, x2, y2 );

    optionViewItemLeft.rect  = leftRect;

    optionViewItemRight.rect = rightRect;

    QFont font = painter->font();

    painter->setFont(QFont("MS Shell Dlg 2",-1,QFont::Bold));

    QApplication::style()->drawItemText( painter,
                                          optionViewItemRight.rect,
                                          Qt::AlignCenter,
                                          QApplication::palette(),
                                          true,
                                          optionViewItemRight.text,
                                          QPalette::NoRole);

    painter->setFont(QFont("MS Shell Dlg 2",-1,QFont::Normal));

    QApplication::style()->drawItemText( painter,
                                          optionViewItemLeft.rect,
                                          Qt::AlignCenter,
                                          QApplication::palette(),
                                          true,
                                          optionViewItemLeft.text,
                                          QPalette::NoRole);
    painter->setFont(font);

}

/*
QWidget *ItemDelegate::createEditor(QWidget *parent,
                                   const QStyleOptionViewItem & ,
                                   const QModelIndex &) const

{
   QPlainTextEdit *editor = new QPlainTextEdit(parent);
   return editor;
}

void ItemDelegate::setEditorData(QWidget *editor,
                                const QModelIndex &index) const
{
   QPlainTextEdit *mEditor = qobject_cast<QPlainTextEdit *>(editor);
   mEditor->setPlainText(index.data().toString());
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                               const QModelIndex &index) const
{
   QPlainTextEdit *mEditor = qobject_cast<QPlainTextEdit *>(editor);
   model->setData(index, mEditor->toPlainText());
}
*/

/*

QPainter *painter, const QRect &rect,
                              int flags, const QPalette &pal, bool enabled,
                              const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const;

*/
