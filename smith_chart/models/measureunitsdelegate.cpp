#include "measureunitsdelegate.h"

#include <QtGlobal>
#include <QComboBox>
#include <QModelIndex>

#include "circuitmodel.h"
#include "metricnotion.h"

//---------------------------------------------------------------------------------------
MeasureUnitsDelegate::MeasureUnitsDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

//---------------------------------------------------------------------------------------
QWidget *MeasureUnitsDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);

    QString baseUnit = index.siblingAtColumn(CircuitModel::ColumnBaseUnit).data().toString();

    for(int i = 0; i < 4; ++i)
    {
        editor->addItem(MetricNotion::getDecPrefix(i) + baseUnit, (i+1)*(-3));
    }

    return editor;
}

//---------------------------------------------------------------------------------------
void MeasureUnitsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    const int currentPowerOf10 = index.siblingAtColumn(CircuitModel::ColumnUnit).data(Qt::EditRole).toInt();
    const int cbIndex = cb->findData(currentPowerOf10);
    if (cbIndex >= 0)
        cb->setCurrentIndex(cbIndex);
}

//---------------------------------------------------------------------------------------
void MeasureUnitsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentData(), Qt::EditRole);
}

//---------------------------------------------------------------------------------------
void MeasureUnitsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

//---------------------------------------------------------------------------------------
