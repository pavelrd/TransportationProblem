#include <QStyledItemDelegate>
 
class ItemDelegate : public QStyledItemDelegate // QStyledItemDelegate
{
	Q_OBJECT
 
public:

	ItemDelegate(QWidget *parent = 0);

    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
                      const QModelIndex& index) const;

};


/*
   virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;
   virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
   virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const;
 */
