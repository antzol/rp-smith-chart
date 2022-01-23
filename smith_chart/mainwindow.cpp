#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableView>
#include <QTranslator>

#include "aspectratiowidget.h"

#include "config.h"
#include "preferencesdialog.h"

#include "chartwidget.h"
#include "chartcontroller.h"

#include "circuitdockwidget.h"
#include "initialdatadockwidget.h"

#include "circuitmodel.h"
#include "doublespinboxdelegate.h"
#include "measureunitsdelegate.h"

#include "circuitscene.h"
#include "componentgraphicsitem.h"


//---------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createLanguageMenu();

    m_circuitModel = new CircuitModel(this);

    m_chartWidget = new ChartWidget();
    setCentralWidget(m_chartWidget);

    createInitialDataUi();
    createCircuitUi();
    createComponentsTable();

    m_chartController = new ChartController(m_chartWidget, this);

    connect(m_initialDataDockWidget, &InitialDataDockWidget::sourceImpedanceChanged,
            m_chartController, &ChartController::updateSourceImpedance);
    connect(m_initialDataDockWidget, &InitialDataDockWidget::loadImpedanceChanged,
            m_chartController, &ChartController::updateLoadImpedance);
    connect(m_initialDataDockWidget, &InitialDataDockWidget::scalingValueChanged,
            m_chartController, &ChartController::updateScalingValue);
    connect(m_initialDataDockWidget, &InitialDataDockWidget::centerFrequencyChanged,
            m_chartController, &ChartController::updateCenterFrequency);

    connect(m_initialDataDockWidget, &InitialDataDockWidget::sourceImpedanceChanged,
            m_circuitDockWidget->scene(), &CircuitScene::processSourceImpedanceChange);
    connect(m_initialDataDockWidget, &InitialDataDockWidget::loadImpedanceChanged,
            m_circuitDockWidget->scene(), &CircuitScene::processLoadImpedanceChange);

    connect(m_circuitModel, &CircuitModel::componentAdded,
            m_chartController, &ChartController::addComponent);
    connect(m_circuitModel, &CircuitModel::componentChanged,
            m_chartController, &ChartController::updateComponent);
    connect(m_circuitModel, &CircuitModel::componentDeleted,
            m_chartController, &ChartController::deleteComponent);

    connect(ui->actionSaveCircuitAsImage, &QAction::triggered,
            this, &MainWindow::saveCircuitAsImage);
    connect(ui->actionSaveSmithChartAsImage, &QAction::triggered,
            this, &MainWindow::saveSmithChartAsImage);
    connect(ui->actionPreferences, &QAction::triggered,
            this, &MainWindow::openPreferencesDialog);

    m_initialDataDockWidget->fillValues();

//    showMaximized();
}

//---------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
void MainWindow::switchLanguage(QAction *action)
{
    Config::instance()->setStringValue(Config::Language, action->data().toString());

    QMessageBox::information(this, tr("Attention"),
                             tr("The application must be restarted for the changes have been applied!"),
                             QMessageBox::Close);
}

//---------------------------------------------------------------------------------------
void MainWindow::saveCircuitAsImage()
{
    Config *config = Config::instance();
    QString dirPath = config->getRecentSavePath();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               dirPath + "/impedance matching circuit.png",
                               tr("Images (*.png)"));
    if (fileName.isEmpty())
        return;

    m_circuitDockWidget->scene()->clearSelection();
    m_circuitDockWidget->scene()->setSceneRect(m_circuitDockWidget->scene()->itemsBoundingRect());
    QImage image(m_circuitDockWidget->scene()->sceneRect().size().toSize(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_circuitDockWidget->scene()->render(&painter);
    image.save(fileName);

    config->setRecentSavePath(fileName);
}

//---------------------------------------------------------------------------------------
void MainWindow::saveSmithChartAsImage()
{
    Config *config = Config::instance();
    QString dirPath = config->getRecentSavePath();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               dirPath + "/smith chart.png",
                               tr("Images (*.png)"));
    if (fileName.isEmpty())
        return;

    m_chartWidget->chart();
    m_chartWidget->chart()->scene()->setSceneRect(m_chartWidget->chart()->scene()->itemsBoundingRect());
    QImage image(m_chartWidget->chart()->scene()->sceneRect().size().toSize(), QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_chartWidget->chart()->scene()->render(&painter);
    image.save(fileName);

    config->setRecentSavePath(fileName);
}

//---------------------------------------------------------------------------------------
void MainWindow::openPreferencesDialog()
{
    PreferencesDialog dlg;
    dlg.exec();
}

//---------------------------------------------------------------------------------------
void MainWindow::createLanguageMenu()
{
    m_currentLanguage = Config::instance()->getStringValue(Config::Language);
    m_langMenu = new QMenu(tr("Language"), this);
    m_langActionGroup = new QActionGroup(this);

    QDir qmDir(QApplication::applicationDirPath() + "/translations");
    QStringList fileNames = qmDir.entryList(QStringList("smith_chart_*.qm"));

    for (int i = 0; i < fileNames.count(); ++i)
    {
        QString locale = fileNames[i].mid(fileNames[i].size() - 5, 2);

        QTranslator translator;
        translator.load(fileNames[i], qmDir.absolutePath());
        QString language = translator.translate("MainWindow", "English");

        QAction *action = new QAction(tr("%1").arg(language), this);
        action->setCheckable(true);
        action->setData(locale);

        m_langMenu->addAction(action);
        m_langActionGroup->addAction(action);

        if (locale == m_currentLanguage)
            action->setChecked(true);
    }

    connect(m_langActionGroup, &QActionGroup::triggered, this, &MainWindow::switchLanguage);
    ui->menuSettings->addMenu(m_langMenu);
}

//---------------------------------------------------------------------------------------
void MainWindow::createInitialDataUi()
{
    m_initialDataDockWidget = new InitialDataDockWidget(this);
    m_initialDataDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::RightDockWidgetArea, m_initialDataDockWidget);
}

//---------------------------------------------------------------------------------------
void MainWindow::createComponentsTable()
{
    QDockWidget *dock = new QDockWidget(tr("Components table"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    m_componentsTableView = new QTableView();
    m_componentsTableView->setModel(m_circuitModel);

    m_componentsTableView->setColumnHidden(CircuitModel::ColumnBaseUnit, true);
    m_componentsTableView->setColumnHidden(CircuitModel::ColumnPowerOf10, true);

    DoubleSpinBoxDelegate *spinBoxDelegate = new DoubleSpinBoxDelegate(m_componentsTableView);
    m_componentsTableView->setItemDelegateForColumn(CircuitModel::ColumnConvertedValue, spinBoxDelegate);

    MeasureUnitsDelegate *unitsDelegate = new MeasureUnitsDelegate(m_componentsTableView);
    m_componentsTableView->setItemDelegateForColumn(CircuitModel::ColumnUnit, unitsDelegate);

    dock->setWidget(m_componentsTableView);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

//---------------------------------------------------------------------------------------
void MainWindow::createCircuitUi()
{
    m_circuitDockWidget = new CircuitDockWidget(m_circuitModel, this);
    m_circuitDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::RightDockWidgetArea, m_circuitDockWidget);
}

//---------------------------------------------------------------------------------------

