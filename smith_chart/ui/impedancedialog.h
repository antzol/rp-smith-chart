#ifndef IMPEDANCEDIALOG_H
#define IMPEDANCEDIALOG_H

#include <QDialog>
#include <QPair>

#include <complex>

namespace Ui {
class ImpedanceDialog;
}

class ImpedanceDialog : public QDialog
{
    Q_OBJECT

public:
    enum {
        ImpedanceScaling_Auto,
        ImpedanceScaling_Manual
    };

    explicit ImpedanceDialog(QWidget *parent = nullptr);
    ~ImpedanceDialog();

    std::complex<double> sourceImpedance() const;
    std::complex<double> loadImpedance() const;
    double scalingValue() const;

    void setSourceImpedance(const std::complex<double> &imp);
    void setLoadImpedance(const std::complex<double> &imp);
    void setScalingValue(double val);


private:
    Ui::ImpedanceDialog *ui;
};

#endif // IMPEDANCEDIALOG_H
