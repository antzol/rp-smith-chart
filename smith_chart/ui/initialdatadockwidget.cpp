#include "initialdatadockwidget.h"
#include "ui_initialdatadockwidget.h"

#include <QDoubleValidator>

#include <QDebug>

#include "impedancedialog.h"
#include "frequencydialog.h"
#include "config.h"

//---------------------------------------------------------------------------------------
InitialDataDockWidget::InitialDataDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::InitialDataDockWidget)
{
    ui->setupUi(this);

    setFixedHeight(sizeHint().height());

    connect(ui->configImpedanceButton, &QPushButton::clicked,
            this, &InitialDataDockWidget::configureImpedance);
    connect(ui->configFrequencyButton, &QPushButton::clicked,
            this, &InitialDataDockWidget::configureFrequency);
}

//---------------------------------------------------------------------------------------
InitialDataDockWidget::~InitialDataDockWidget()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::fillValues()
{
    Config *config = Config::instance();

    m_sourceImpedance = config->getSourceImpedanceDefaultValue();
    updateSourceImpedance(m_sourceImpedance);
    emit sourceImpedanceChanged(m_sourceImpedance);

    m_loadImpedance = config->getLoadImpedanceDefaultValue();
    updateLoadImpedance(m_loadImpedance);
    emit loadImpedanceChanged(m_loadImpedance);

    m_manualScalingValue = config->getIntValue(Config::ImpedanceZ0);
    emit scalingValueChanged(m_manualScalingValue);

    m_centerFrequency.update(config->getDoubleValue(Config::FrequencyCenter));
    updateCenterFrequency(m_centerFrequency);
    emit centerFrequencyChanged(m_centerFrequency.value());

    m_frequencySpan.update(config->getDoubleValue(Config::FrequencySpan));
    updateFrequencySpan(m_frequencySpan);
    emit frequencySpanChanged(m_frequencySpan.value());

    m_frequencyStep.update(config->getDoubleValue(Config::FrequencyStep));
    updateFrequencyStep(m_frequencyStep);
    emit frequencySpanChanged(m_frequencyStep.value());
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::configureImpedance()
{
    ImpedanceDialog dlg;
    dlg.setSourceImpedance(m_sourceImpedance);
    dlg.setLoadImpedance(m_loadImpedance);
    dlg.setScalingValue(m_manualScalingValue);
    if (dlg.exec() == QDialog::Accepted)
    {
        if (dlg.sourceImpedance() != m_sourceImpedance)
        {
            m_sourceImpedance = dlg.sourceImpedance();
            updateSourceImpedance(m_sourceImpedance);
            emit sourceImpedanceChanged(m_sourceImpedance);
        }

        if (dlg.loadImpedance() != m_loadImpedance)
        {
            m_loadImpedance = dlg.loadImpedance();
            updateLoadImpedance(m_loadImpedance);
            emit loadImpedanceChanged(m_loadImpedance);
        }

        if (dlg.scalingValue() != m_manualScalingValue)
        {
            m_manualScalingValue = dlg.scalingValue();
            emit scalingValueChanged(m_manualScalingValue);
        }
    }
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::configureFrequency()
{
    FrequencyDialog dlg;
    dlg.setCenterFrequency(m_centerFrequency);
    dlg.setFrequencySpan(m_frequencySpan);
    dlg.setFrequencyStep(m_frequencyStep);
    if (dlg.exec() == QDialog::Accepted)
    {
        if (dlg.centerFrequency() != m_centerFrequency)
        {
            m_centerFrequency = dlg.centerFrequency();
            updateCenterFrequency(m_centerFrequency);
            emit centerFrequencyChanged(m_centerFrequency.value());
        }

        if (dlg.frequencySpan() != m_frequencySpan)
        {
            m_frequencySpan = dlg.frequencySpan();
            updateFrequencySpan(m_frequencySpan);
            emit frequencySpanChanged(m_frequencySpan.value());
        }

        if (dlg.frequencyStep() != m_frequencyStep)
        {
            m_frequencyStep = dlg.frequencyStep();
            updateFrequencyStep(m_frequencyStep);
            emit frequencySpanChanged(m_frequencyStep.value());
        }
    }
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::updateSourceImpedance(std::complex<double> imp)
{
    QString str = QString("%1 %2 j%3")
            .arg(imp.real())
            .arg(imp.imag() < 0 ? '-' : '+')
            .arg(std::abs(imp.imag()));
    ui->sourceImpedanceLabel->setText(str);
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::updateLoadImpedance(std::complex<double> imp)
{
    QString str = QString("%1 %2 j%3")
            .arg(imp.real())
            .arg(imp.imag() < 0 ? '-' : '+')
            .arg(std::abs(imp.imag()));
    ui->loadImpedanceLabel->setText(str);
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::updateCenterFrequency(const MetricNotion &notion)
{
    QString str = tr("%1 %2Hz")
            .arg(notion.convertedValue())
            .arg(notion.prefix());
    ui->frequencyCenterLabel->setText(str);
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::updateFrequencySpan(const MetricNotion &notion)
{
    QString str = tr("%1 %2Hz")
            .arg(notion.convertedValue())
            .arg(notion.prefix());
    ui->frequencySpanLabel->setText(str);
}

//---------------------------------------------------------------------------------------
void InitialDataDockWidget::updateFrequencyStep(const MetricNotion &notion)
{
    QString str = tr("%1 %2Hz")
            .arg(notion.convertedValue())
            .arg(notion.prefix());
    ui->frequencyStepLabel->setText(str);
}

//---------------------------------------------------------------------------------------
