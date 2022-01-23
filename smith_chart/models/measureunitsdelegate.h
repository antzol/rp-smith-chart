#ifndef MEASUREUNITSDELEGATE_H
#define MEASUREUNITSDELEGATE_H

#include <QStyledItemDelegate>

class MeasureUnitsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    MeasureUnitsDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // MEASUREUNITSDELEGATE_H
