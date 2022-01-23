#ifndef ASPECTRATIOWIDGET_H
#define ASPECTRATIOWIDGET_H

#include <QWidget>
#include <QBoxLayout>

class AspectRatioWidget : public QWidget
{
public:
    AspectRatioWidget(QWidget *widget, float width, float height, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QBoxLayout *layout;
    float arWidth;  // aspect ratio width
    float arHeight; // aspect ratio height
};

#endif // ASPECTRATIOWIDGET_H
