#ifndef RANGESLIDER_H
#define RANGESLIDER_H
#include <QSlider>
#include <QStyle>

class RangeSlider: public QSlider {
    Q_OBJECT
public:
    RangeSlider(Qt::Orientation orientation, QWidget* parent = 0);
    int low(); //return the lower limit
    void setLow(int low);
    int high();
    void setHigh(int high);

signals:
    void sliderMoved(int, int);

private:

protected:
    void paintEvent(QPaintEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    int lowLimit;
    int highLimit;
    int pick(QPoint const& pt);
    int pixelPosToRangeValue(int pos);
    QStyle::SubControl pressedControl = QStyle::SC_None;
    int tickInterval = 0;
    QSlider::TickPosition tickPosition = QSlider::NoTicks;
    QStyle::SubControl hoverControl = QStyle::SC_None;
    int clickOffset = 0;
    int activeSlider = 0;
};

#endif // RANGESLIDER_H
