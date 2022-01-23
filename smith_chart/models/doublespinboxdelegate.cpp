#include "doublespinboxdelegate.h"

#include <QDoubleSpinBox>

//---------------------------------------------------------------------------------------
DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

//---------------------------------------------------------------------------------------
QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &/* option */,
                                             const QModelIndex &/*index*/) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setRange(1, 999);
    editor->setSingleStep(0.1);
    editor->setDecimals(2);

    return editor;
}

//---------------------------------------------------------------------------------------
void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

//---------------------------------------------------------------------------------------
void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

//---------------------------------------------------------------------------------------
void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                 const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

//---------------------------------------------------------------------------------------
