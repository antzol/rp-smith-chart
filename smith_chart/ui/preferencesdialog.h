#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QMap>

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

class DoubleListModel;

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();

public slots:
    void accept() override;

private slots:
    void apply();

    void switchPage(int idx);

    void selectColor();
    void addIntValueToChangedList(int val);
    void addDoubleValueToChangedList(double val);
    void addMetricValueToChangedList();
    void addDoubleArrayModelToChangedList();

    void addCircleValue();
    void removeCircleValue();
    void addArcValue();
    void removeArcValue();

private:
    void createSwitchList();
    void loadCurrentPreferences();

    Ui::PreferencesDialog *ui;
    DoubleListModel *m_circlesValuesModel;
    DoubleListModel *m_arcsValuesModel;

    std::unordered_map<QPushButton*, int> m_colorButtons;
    std::unordered_map<QSpinBox*, int> m_intSpinBoxes;
    std::unordered_map<QDoubleSpinBox*, int> m_doubleSpinBoxes;
    std::unordered_map<DoubleListModel*, int> m_doubleArraysModels;
    std::unordered_map<QDoubleSpinBox*, std::pair<QComboBox*, int>> m_metricSpinBoxes;
    std::unordered_map<QComboBox*, std::pair<QDoubleSpinBox*, int>> m_metricComboBoxes;

    // key - enum value from Config, value - parameter value
    std::unordered_map<int, QString> m_changedStringValues;
    std::unordered_map<int, int> m_changedIntValues;
    std::unordered_map<int, double> m_changedDoubleValues;
    std::unordered_map<int, DoubleListModel*> m_changedDoubleArraysModels;
};

#endif // PREFERENCESDIALOG_H
