#include "frequencydialog.h"
#include "ui_frequencydialog.h"

#include <math.h>

#include "metricnotion.h"

//---------------------------------------------------------------------------------------
FrequencyDialog::FrequencyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrequencyDialog)
{
    ui->setupUi(this);

    QString baseUnit = tr("Hz");

    for(int i = 0; i < 4; ++i)
    {
        ui->freqCenterUnitsComboBox->addItem(MetricNotion::getIncPrefix(i) + baseUnit, (i+1)*3);
        ui->freqSpanUnitsComboBox->addItem(MetricNotion::getIncPrefix(i) + baseUnit, (i+1)*3);
        ui->freqStepUnitsComboBox->addItem(MetricNotion::getIncPrefix(i) + baseUnit, (i+1)*3);
    }

    double w = (sizeHint().width() > minimumWidth()) ? sizeHint().width() : minimumWidth();
    setFixedSize(w, sizeHint().height());

    connect(ui->okButton, &QPushButton::clicked,this, &FrequencyDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked,this, &FrequencyDialog::reject);
}

//---------------------------------------------------------------------------------------
FrequencyDialog::~FrequencyDialog()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
MetricNotion FrequencyDialog::centerFrequency() const
{
    int powerOf10 = ui->freqCenterUnitsComboBox->currentData().toInt();
    double convertedValue = ui->freqCenterSpinBox->value();
    return MetricNotion(convertedValue * pow(10, powerOf10));
}

//---------------------------------------------------------------------------------------
MetricNotion FrequencyDialog::frequencySpan() const
{
    int powerOf10 = ui->freqSpanUnitsComboBox->currentData().toInt();
    double convertedValue = ui->freqSpanSpinBox->value();
    return MetricNotion(convertedValue * pow(10, powerOf10));
}

//---------------------------------------------------------------------------------------
MetricNotion FrequencyDialog::frequencyStep() const
{
    int powerOf10 = ui->freqStepUnitsComboBox->currentData().toInt();
    double convertedValue = ui->freqStepSpinBox->value();
    return MetricNotion(convertedValue * pow(10, powerOf10));
}

//---------------------------------------------------------------------------------------
void FrequencyDialog::setCenterFrequency(const MetricNotion &notion)
{
    ui->freqCenterSpinBox->setValue(notion.convertedValue());
    const int cbIndex = ui->freqCenterUnitsComboBox->findData(notion.powerOf10());
    if (cbIndex >= 0)
        ui->freqCenterUnitsComboBox->setCurrentIndex(cbIndex);
}

//---------------------------------------------------------------------------------------
void FrequencyDialog::setFrequencySpan(const MetricNotion &notion)
{
    ui->freqSpanSpinBox->setValue(notion.convertedValue());
    const int cbIndex = ui->freqSpanUnitsComboBox->findData(notion.powerOf10());
    if (cbIndex >= 0)
        ui->freqSpanUnitsComboBox->setCurrentIndex(cbIndex);
}

//---------------------------------------------------------------------------------------
void FrequencyDialog::setFrequencyStep(const MetricNotion &notion)
{
    ui->freqStepSpinBox->setValue(notion.convertedValue());
    const int cbIndex = ui->freqStepUnitsComboBox->findData(notion.powerOf10());
    if (cbIndex >= 0)
        ui->freqStepUnitsComboBox->setCurrentIndex(cbIndex);
}

//---------------------------------------------------------------------------------------
