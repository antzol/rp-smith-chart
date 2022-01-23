#include "circuitdockwidget.h"
#include "ui_circuitdockwidget.h"

#include "componentgraphicsitem.h"
#include "circuitscene.h"

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QMouseEvent>

#include <QDrag>
#include <QMimeData>

//---------------------------------------------------------------------------------------
CircuitDockWidget::CircuitDockWidget(CircuitModel *circuitModel, QWidget *parent) : QDockWidget(parent),
    ui(new Ui::CircuitDockWidget)
{
    ui->setupUi(this);
    ui->RparallelLabel->setVisible(false);
    ui->RserialLabel->setVisible(false);

    m_scene = new CircuitScene(circuitModel, this);
    connect(m_scene, &CircuitScene::circuitComponentCreated,
            this, &CircuitDockWidget::circuitComponentCreated);

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    zoomIn();

    componentLabelsWithCodes.insert(ui->CparallelLabel, ComponentGraphicsItemType::CapacitorParallel);
    componentLabelsWithCodes.insert(ui->CserialLabel, ComponentGraphicsItemType::CapacitorSerial);
    componentLabelsWithCodes.insert(ui->LparallelLabel, ComponentGraphicsItemType::InductanceParallel);
    componentLabelsWithCodes.insert(ui->LserialLabel, ComponentGraphicsItemType::InductanceSerial);

    connect(ui->zoomInButton, &QToolButton::clicked, this, &CircuitDockWidget::zoomIn);
    connect(ui->zoomOutButton, &QToolButton::clicked, this, &CircuitDockWidget::zoomOut);

}

//---------------------------------------------------------------------------------------
CircuitDockWidget::~CircuitDockWidget()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
CircuitScene* CircuitDockWidget::scene()
{
    return m_scene;
}

//---------------------------------------------------------------------------------------
void CircuitDockWidget::zoomIn()
{
    ui->graphicsView->scale(m_zoomStep, m_zoomStep);
}

//---------------------------------------------------------------------------------------
void CircuitDockWidget::zoomOut()
{
    ui->graphicsView->scale(1/m_zoomStep, 1/m_zoomStep);
}

//---------------------------------------------------------------------------------------
void CircuitDockWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QLabel *label = qobject_cast<QLabel*>(childAt(event->pos()));
        if (!label || !componentLabelsWithCodes.contains(label))
            return;

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(label->toolTip());
        QByteArray componentType = QByteArray::number(componentLabelsWithCodes.value(label));
        mimeData->setData(CircuitScene::MimeTypeComponent, componentType);
        drag->setMimeData(mimeData);
        drag->setPixmap(label->pixmap());
        drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                                drag->pixmap().height()));

        /*Qt::DropAction dropAction =*/ drag->exec();
    }
}

//---------------------------------------------------------------------------------------
