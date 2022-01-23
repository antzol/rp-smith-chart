#ifndef CIRCUITDOCKWIDGET_H
#define CIRCUITDOCKWIDGET_H

#include <QDockWidget>
#include <QLabel>
#include <QMap>

class QGraphicsView;

class ComponentGraphicsItem;

class CircuitModel;
class CircuitScene;

namespace Ui {
class CircuitDockWidget;
}

class CircuitDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit CircuitDockWidget(CircuitModel *circuitModel, QWidget *parent = nullptr);
    ~CircuitDockWidget();

    CircuitScene* scene();

private slots:
    void zoomIn();
    void zoomOut();

signals:
    void circuitComponentCreated(int pos, ComponentGraphicsItem *componentItem);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::CircuitDockWidget *ui;
    QMap<QLabel*, int> componentLabelsWithCodes;
    CircuitScene *m_scene;

    const double m_zoomStep{1.2};
};

#endif // CIRCUITDOCKWIDGET_H
