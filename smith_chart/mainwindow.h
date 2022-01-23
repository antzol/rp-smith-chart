#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QBoxLayout>

class QActionGroup;
class QMenu;
class QTableView;

class ChartController;
class ChartWidget;

class CircuitModel;

class InitialDataDockWidget;
class CircuitDockWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void switchLanguage(QAction *action);

    void saveCircuitAsImage();
    void saveSmithChartAsImage();

    void openPreferencesDialog();

private:
    void createLanguageMenu();
    void createInitialDataUi();
    void createComponentsTable();
    void createCircuitUi();

    Ui::MainWindow *ui;
    QActionGroup *m_langActionGroup;
    QMenu *m_langMenu;
    QString m_currentLanguage;

    InitialDataDockWidget *m_initialDataDockWidget;
    CircuitDockWidget *m_circuitDockWidget;

    ChartController *m_chartController;
    ChartWidget *m_chartWidget;

    CircuitModel *m_circuitModel;
    QTableView *m_componentsTableView;

};
#endif // MAINWINDOW_H
