#ifndef FREQUENCYDIALOG_H
#define FREQUENCYDIALOG_H

#include <QDialog>

#include "metricnotion.h"

namespace Ui {
class FrequencyDialog;
}

class FrequencyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FrequencyDialog(QWidget *parent = nullptr);
    ~FrequencyDialog();

    MetricNotion centerFrequency() const;
    MetricNotion frequencySpan() const;
    MetricNotion frequencyStep() const;

    void setCenterFrequency(const MetricNotion &notion);
    void setFrequencySpan(const MetricNotion &notion);
    void setFrequencyStep(const MetricNotion &notion);

private:
    Ui::FrequencyDialog *ui;
};

#endif // FREQUENCYDIALOG_H
