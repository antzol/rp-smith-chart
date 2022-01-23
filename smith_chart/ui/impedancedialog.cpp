#include "impedancedialog.h"
#include "ui_impedancedialog.h"



//---------------------------------------------------------------------------------------
ImpedanceDialog::ImpedanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImpedanceDialog)
{
    ui->setupUi(this);
    setFixedSize(sizeHint());

    connect(ui->okButton, &QPushButton::clicked,this, &ImpedanceDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked,this, &ImpedanceDialog::reject);
}

//---------------------------------------------------------------------------------------
ImpedanceDialog::~ImpedanceDialog()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
std::complex<double> ImpedanceDialog::sourceImpedance() const
{
    double re = ui->sourceReSpinBox->value();
    double im = ui->sourceImSpinBox->value();
    return std::complex<double>(re, im);
}

//---------------------------------------------------------------------------------------
std::complex<double> ImpedanceDialog::loadImpedance() const
{
    double re = ui->loadReSpinBox->value();
    double im = ui->loadImSpinBox->value();
    return std::complex<double>(re, im);
}

//---------------------------------------------------------------------------------------
double ImpedanceDialog::scalingValue() const
{
    return ui->manualZ0SpinBox->value();
}

//---------------------------------------------------------------------------------------
void ImpedanceDialog::setSourceImpedance(const std::complex<double> &imp)
{
    ui->sourceReSpinBox->setValue(imp.real());
    ui->sourceImSpinBox->setValue(imp.imag());
}

//---------------------------------------------------------------------------------------
void ImpedanceDialog::setLoadImpedance(const std::complex<double> &imp)
{
    ui->loadReSpinBox->setValue(imp.real());
    ui->loadImSpinBox->setValue(imp.imag());
}

//---------------------------------------------------------------------------------------
void ImpedanceDialog::setScalingValue(double val)
{
    ui->manualZ0SpinBox->setValue(val);
}

//---------------------------------------------------------------------------------------
