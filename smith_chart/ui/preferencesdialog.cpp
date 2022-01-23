#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"


#include <QColorDialog>

#include "config.h"
#include "doublelistmodel.h"
#include "metricnotion.h"

static constexpr char PropertyColor[] = "Color";

//---------------------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    createSwitchList();

    ui->smithChartTabWidget->setCurrentIndex(0);
    ui->defaultInitialDataTabWidget->setCurrentIndex(0);

    m_circlesValuesModel = new DoubleListModel(this);
    ui->circlesValuesListView->setModel(m_circlesValuesModel);
    m_doubleArraysModels[m_circlesValuesModel] = Config::ChartCirclesValues;

    m_arcsValuesModel = new DoubleListModel(this);
    ui->arcsValuesListView->setModel(m_arcsValuesModel);
    m_doubleArraysModels[m_arcsValuesModel] = Config::ChartArcsValues;

    m_colorButtons[ui->resistanceCircleColorButton]  = Config::ColorResistanceCircle;
    m_colorButtons[ui->reactanceArcColorButton]      = Config::ColorReactanceArc;
    m_colorButtons[ui->conductanceCircleColorButton] = Config::ColorConductanceCircle;
    m_colorButtons[ui->susceptanceArcColorButton]    = Config::ColorSusceptanceArc;

    m_colorButtons[ui->seriesCapacitorColorButton]  = Config::ColorSeriesCapacitor;
    m_colorButtons[ui->seriesInductanceColorButton] = Config::ColorSeriesInductance;
    m_colorButtons[ui->shuntCapacitorColorButton]   = Config::ColorParallelCapacitor;
    m_colorButtons[ui->shuntInductanceColorButton]  = Config::ColorParallelInductance;

    m_colorButtons[ui->sourceImpedancePointColorButton] = Config::ColorSourceImpedancePoint;
    m_colorButtons[ui->sourceImpedanceConjugatePointColorButton] = Config::ColorSourceImpedanceConjugatePoint;
    m_colorButtons[ui->loadImpedancePointColorButton]   = Config::ColorLoadImpedancePoint;

    m_intSpinBoxes[ui->chartLinesWidthSpinBox]    = Config::WidthChartLine;
    m_intSpinBoxes[ui->componentLineWidthSpinBox] = Config::WidthComponentLine;
    m_intSpinBoxes[ui->impedancePointSizeSpinBox] = Config::ImpedancePointSize;

    m_intSpinBoxes[ui->manualZ0SpinBox] = Config::ImpedanceZ0;

    m_doubleSpinBoxes[ui->sourceReSpinBox] = Config::ImpedanceReSource;
    m_doubleSpinBoxes[ui->sourceImSpinBox] = Config::ImpedanceImSource;
    m_doubleSpinBoxes[ui->loadReSpinBox]   = Config::ImpedanceReLoad;
    m_doubleSpinBoxes[ui->loadImSpinBox]   = Config::ImpedanceImLoad;

    // frequencies
    m_metricSpinBoxes[ui->freqCenterSpinBox] = std::pair(ui->freqCenterUnitsComboBox, Config::FrequencyCenter);
    m_metricSpinBoxes[ui->freqSpanSpinBox]   = std::pair(ui->freqSpanUnitsComboBox, Config::FrequencySpan);
    m_metricSpinBoxes[ui->freqStepSpinBox]   = std::pair(ui->freqStepUnitsComboBox, Config::FrequencyStep);

    m_metricComboBoxes[ui->freqCenterUnitsComboBox] = std::pair(ui->freqCenterSpinBox, Config::FrequencyCenter);
    m_metricComboBoxes[ui->freqSpanUnitsComboBox]   = std::pair(ui->freqSpanSpinBox, Config::FrequencySpan);
    m_metricComboBoxes[ui->freqStepUnitsComboBox]   = std::pair(ui->freqStepSpinBox, Config::FrequencyStep);

    // components
    m_metricSpinBoxes[ui->defaultCapacitorSpinBox]  = std::pair(ui->defaultCapacitorUnitsComboBox, Config::ComponentDefaultCapacitor);
    m_metricSpinBoxes[ui->defaultInductanceSpinBox] = std::pair(ui->defaultInductanceUnitsComboBox, Config::ComponentDefaultInductance);

    m_metricComboBoxes[ui->defaultCapacitorUnitsComboBox] = std::pair(ui->defaultCapacitorSpinBox, Config::ComponentDefaultCapacitor);
    m_metricComboBoxes[ui->defaultInductanceUnitsComboBox]= std::pair(ui->defaultInductanceSpinBox, Config::ComponentDefaultInductance);

    QString frequencyBaseUnit = tr("Hz");
    QString capacitorBaseUnit = tr("F");
    QString inductanceBaseUnit = tr("H");
    for(int i = 0; i < 4; ++i)
    {
        ui->freqCenterUnitsComboBox->addItem(MetricNotion::getIncPrefix(i) + frequencyBaseUnit, (i+1)*3);
        ui->freqSpanUnitsComboBox->addItem(MetricNotion::getIncPrefix(i) + frequencyBaseUnit, (i+1)*3);
        ui->freqStepUnitsComboBox->addItem(MetricNotion::getIncPrefix(i) + frequencyBaseUnit, (i+1)*3);

        ui->defaultCapacitorUnitsComboBox->addItem(MetricNotion::getDecPrefix(i) + capacitorBaseUnit, (i+1)*(-3));
        ui->defaultInductanceUnitsComboBox->addItem(MetricNotion::getDecPrefix(i) + inductanceBaseUnit, (i+1)*(-3));
    }

    loadCurrentPreferences();

    connect(ui->addCircleButton, &QPushButton::clicked, this, &PreferencesDialog::addCircleValue);
    connect(ui->deleteCircleButton, &QPushButton::clicked, this, &PreferencesDialog::removeCircleValue);
    connect(ui->addArcButton, &QPushButton::clicked, this, &PreferencesDialog::addArcValue);
    connect(ui->deleteArcButton, &QPushButton::clicked, this, &PreferencesDialog::removeArcValue);

    connect(ui->okButton, &QPushButton::clicked, this, &PreferencesDialog::accept);
    connect(ui->applyButton, &QPushButton::clicked, this, &PreferencesDialog::apply);
    connect(ui->cancelButton, &QPushButton::clicked, this, &PreferencesDialog::reject);

    setMinimumSize(sizeHint());
}

//---------------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::accept()
{
    apply();
    QDialog::accept();
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::apply()
{
    Config *config = Config::instance();

    for (auto& [key, value] : m_changedStringValues)
        config->setStringValue(key, value);

    for (auto& [key, value] : m_changedIntValues)
        config->setIntValue(key, value);

    for (auto& [key, value] : m_changedDoubleValues)
        config->setDoubleValue(key, value);

    for (auto& [key, model] : m_changedDoubleArraysModels)
        config->setDoubleVector(key, model->getAllData());

}

//---------------------------------------------------------------------------------------
void PreferencesDialog::switchPage(int idx)
{
    ui->stackedWidget->setCurrentIndex(idx);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::selectColor()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !m_colorButtons.count(btn))
        return;

    QString backgroundStyleSheet("background-color:%1;");
    QColor color(btn->property(PropertyColor).toString());
    QColorDialog dlg(color);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString colorName = dlg.selectedColor().name(QColor::HexRgb);
        m_changedStringValues[m_colorButtons[btn]] = colorName;

        btn->setProperty(PropertyColor, colorName);
        btn->setStyleSheet(backgroundStyleSheet.arg(colorName));
    }
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::addIntValueToChangedList(int val)
{
    QSpinBox *sb = qobject_cast<QSpinBox*>(sender());
    if (!sb || !m_intSpinBoxes.count(sb))
        return;

    m_changedIntValues[m_intSpinBoxes[sb]] = val;
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::addDoubleValueToChangedList(double val)
{
    QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox*>(sender());
    if (!dsb || !m_doubleSpinBoxes.count(dsb))
        return;

    m_changedDoubleValues[m_doubleSpinBoxes[dsb]] = val;
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::addMetricValueToChangedList()
{
    QComboBox *cb;
    QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox*>(sender());
    if (!dsb || !m_metricSpinBoxes.count(dsb))
    {
        cb = qobject_cast<QComboBox*>(sender());
        if (!cb || !m_metricComboBoxes.count(cb))
            return;
        else
            dsb = m_metricComboBoxes[cb].first;
    }
    else
    {
        cb = m_metricSpinBoxes[dsb].first;
    }

    int powerOf10 = cb->currentData().toInt();
    double convertedValue = dsb->value();

    m_changedDoubleValues[m_metricSpinBoxes[dsb].second] = convertedValue * pow(10, powerOf10);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::addDoubleArrayModelToChangedList()
{
    DoubleListModel *m = qobject_cast<DoubleListModel*>(sender());
    if (!m || !m_doubleArraysModels.count(m))
        return;

    m_changedDoubleArraysModels[m_doubleArraysModels[m]] = m;
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::addCircleValue()
{
    QModelIndex index = m_circlesValuesModel->addItem();
    QItemSelectionModel *sm = ui->circlesValuesListView->selectionModel();
    sm->clear();
    sm->select(index, QItemSelectionModel::Select);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::removeCircleValue()
{
    QModelIndexList indexList = ui->circlesValuesListView->selectionModel()->selectedRows();
    for (auto index : indexList)
        m_circlesValuesModel->removeItem(index);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::addArcValue()
{
    QModelIndex index = m_arcsValuesModel->addItem();
    QItemSelectionModel *sm = ui->arcsValuesListView->selectionModel();
    sm->clear();
    sm->select(index, QItemSelectionModel::Select);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::removeArcValue()
{
    QModelIndexList indexList = ui->arcsValuesListView->selectionModel()->selectedRows();
    for (auto index : indexList)
        m_arcsValuesModel->removeItem(index);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::createSwitchList()
{
    QListWidgetItem *smithChartItem = new QListWidgetItem(QIcon(":/logo/SmithChart"),
                                                          tr("Smith chart"));

    QListWidgetItem *initialDataItem = new QListWidgetItem(QIcon(":/icons/edit_page"),
                                                          tr("Initial data and components"));

    ui->panelSwitchListWidget->insertItem(0, smithChartItem);
    ui->panelSwitchListWidget->insertItem(1, initialDataItem);

    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->panelSwitchListWidget, &QListWidget::currentRowChanged,
            this, &PreferencesDialog::switchPage);
}

//---------------------------------------------------------------------------------------
void PreferencesDialog::loadCurrentPreferences()
{
    Config *config = Config::instance();

    // colors
    QString backgroundStyleSheet("background-color:%1;");
    QString color;
    for (auto& [btn, colorKey] : m_colorButtons)
    {
        color = config->getColor(colorKey).name(QColor::HexRgb);
        btn->setProperty(PropertyColor, color);
        btn->setStyleSheet(backgroundStyleSheet.arg(color));
        connect(btn, &QPushButton::clicked, this, &PreferencesDialog::selectColor);
    }

    // int values
    for (auto& [sb, valKey] : m_intSpinBoxes)
    {
        int val = config->getIntValue(valKey);
        sb->setValue(val);
        connect(sb, &QSpinBox::valueChanged, this, &PreferencesDialog::addIntValueToChangedList);
    }

    // double arrays
    for (auto& [model, valKey] : m_doubleArraysModels)
    {
        model->fillModel(config->getDoubleVector(valKey));
        connect(model, &DoubleListModel::dataChanged,
                this, &PreferencesDialog::addDoubleArrayModelToChangedList);
    }

    // double values
    for (auto& [sb, valKey] : m_doubleSpinBoxes)
    {
        double val = config->getDoubleValue(valKey);
        sb->setValue(val);
        connect(sb, &QDoubleSpinBox::valueChanged, this, &PreferencesDialog::addDoubleValueToChangedList);
    }

    // double values in metric notion
    for (auto& [sb, p] : m_metricSpinBoxes)
    {
        double val = config->getDoubleValue(p.second);
        MetricNotion notion(val);
        sb->setValue(notion.convertedValue());
        QComboBox *cb = p.first;
        const int cbIndex = cb->findData(notion.powerOf10());
        if (cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);

        connect(sb, &QDoubleSpinBox::valueChanged, this, &PreferencesDialog::addMetricValueToChangedList);
        connect(cb, &QComboBox::currentIndexChanged, this, &PreferencesDialog::addMetricValueToChangedList);

    }
}

//---------------------------------------------------------------------------------------
