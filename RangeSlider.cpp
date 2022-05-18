#include "RangeSlider.h"
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QMouseEvent>

RangeSlider::RangeSlider(Qt::Orientation orientation, QWidget* parent): QSlider(orientation) {
    this->lowLimit = this->minimum();
    this->highLimit = this->maximum();
    this->pressedControl = QStyle::SC_None;
}

int RangeSlider::low() {
    return this->lowLimit;
}

void RangeSlider::setLow(int low) {
    this->lowLimit = low;
}

int RangeSlider::high() {
    return this->highLimit;
}

void RangeSlider::setHigh(int high) {
    this->highLimit = high;
}

void RangeSlider::updateTime() {
    //tell player to seek to lower limit
    emit newStart(low());
}

void RangeSlider::paintEvent(QPaintEvent *ev) {
    QPainter painter(this);
    QStyleOptionSlider opt;

    // Draw groove
    this->initStyleOption(&opt);
    opt.sliderValue = 0;
    opt.sliderPosition = 0;
    opt.subControls = QStyle::SC_SliderGroove;
    if (this->tickPosition != NoTicks) opt.subControls |= QStyle::SC_SliderTickmarks;
    this->style()->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);
    QRect groove = this->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    // Draw span
    this->initStyleOption(&opt);
    opt.subControls = QStyle::SC_SliderGroove;
    opt.sliderValue = 0;
    opt.sliderPosition = this->lowLimit;
    QRect low_rect = this->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    opt.sliderPosition = this->highLimit;
    QRect high_rect = this->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    int low_pos = this->pick(low_rect.center());
    int high_pos = this->pick(high_rect.center());
    int min_pos = std::min(low_pos, high_pos);
    int max_pos = std::max(low_pos, high_pos);

    QPoint c = QRect(low_rect.center(), high_rect.center()).center();
    QRect span_rect;
    if (opt.orientation == Qt::Horizontal) {
        span_rect = QRect(QPoint(min_pos, c.y()-2), QPoint(max_pos, c.y()+1));
        groove.adjust(0,0,-1,0);
    } else {
        span_rect = QRect(QPoint(c.x()-2, min_pos), QPoint(c.x()+1, max_pos));
        groove.adjust(0,0,0,-1);
    }

    if (1) {
        QColor highlight = this->palette().color(QPalette::Highlight);
        painter.setBrush(QBrush(highlight));
        painter.setPen(QPen(highlight,0));
        painter.drawRect(span_rect.intersected(groove));
        //.color(QPalette.)
    }

    QStyle* style = QApplication::style();
    this->initStyleOption(&opt);
    opt.subControls = QStyle::SC_SliderHandle; // | QStyle::SC_SliderGroove;
    if (this->tickPosition != QSlider::NoTicks) opt.subControls |= QStyle::SC_SliderTickmarks;
    if (this->pressedControl) opt.activeSubControls = this->pressedControl;
    else opt.activeSubControls = this->hoverControl;
    opt.sliderPosition = this->lowLimit;
    opt.sliderValue = this->lowLimit;
    style->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);

    this->initStyleOption(&opt);
    opt.subControls = QStyle::SC_SliderHandle;
    if (this->tickPosition != QSlider::NoTicks) opt.subControls |= QStyle::SC_SliderTickmarks;
    if (this->pressedControl) opt.activeSubControls = this->pressedControl;
    else opt.activeSubControls = this->hoverControl;
    opt.sliderPosition = this->highLimit;
    opt.sliderValue = this->highLimit;
    style->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);
}


void RangeSlider::mousePressEvent(QMouseEvent *ev) {
    /*  # In a normal slider control, when the user clicks on a point in the
        # slider's total range, but not on the slider part of the control the
        # control would jump the slider value to where the user clicked.
        # For this control, clicks which are not direct hits will slide both
        # slider parts
        */
    if (ev->button() == Qt::LeftButton) {
        ev->accept();
        QStyleOptionSlider opt;
        this->initStyleOption(&opt);
        this->activeSlider = -1;

        QStyle* style = QApplication::style();
        QStyle::SubControl hit;

        opt.sliderPosition = this->lowLimit;
        hit = style->hitTestComplexControl(QStyle::CC_Slider, &opt, ev->pos(), this);
        if (hit == QStyle::SC_SliderHandle) {
            this->activeSlider = 0;
            this->pressedControl = hit;
            this->triggerAction(this->SliderMove);
            this->setRepeatAction(this->SliderNoAction);
            this->setSliderDown(true);
        } else {
            opt.sliderPosition = this->highLimit;
            hit = style->hitTestComplexControl(QStyle::CC_Slider, &opt, ev->pos(), this);
            if (hit == QStyle::SC_SliderHandle) {
                this->activeSlider = 1;
                this->pressedControl = hit;
                this->triggerAction(this->SliderMove);
                this->setRepeatAction(this->SliderNoAction);
                this->setSliderDown(true);
            }
        }

        if (this->activeSlider < 0) {
            this->pressedControl = QStyle::SC_SliderHandle;
            this->clickOffset = this->pixelPosToRangeValue(this->pick(ev->pos()));
            this->triggerAction(this->SliderMove);
            this->setRepeatAction(this->SliderNoAction);
        }
    } else {
        ev->ignore();
    }

    QSlider::mousePressEvent(ev);
}

void RangeSlider::mouseMoveEvent(QMouseEvent *ev) {
    if (this->pressedControl != QStyle::SC_SliderHandle) {
        ev->ignore();
        return;
    }

    ev->accept();
    int new_pos = this->pixelPosToRangeValue(this->pick(ev->pos()));
    QStyleOptionSlider opt;
    this->initStyleOption(&opt);

    int offset, diff;
    if (this->activeSlider < 0) {
        offset = new_pos - this->clickOffset;
        this->highLimit += offset;
        this->lowLimit += offset;
        if (this->lowLimit < this->minimum()) {
            diff = this->minimum() - this->lowLimit;
            this->lowLimit += diff;
            this->highLimit += diff;
        }
        if (this->highLimit > this->maximum()) {
            diff = this->maximum() - this->highLimit;
            this->lowLimit += diff;
            this->highLimit += diff;
        }
    } else if (this->activeSlider == 0) {
        if (new_pos >= this->highLimit)
            new_pos = this->highLimit - 1;
        this->lowLimit = new_pos;
    } else {
        if (new_pos <= this->lowLimit)
            new_pos = this->lowLimit + 1;
        this->highLimit = new_pos;
    }

    this->clickOffset = new_pos;
    this->update();
    emit this->sliderMoved(this->lowLimit, this->highLimit);
}

int RangeSlider::pick(const QPoint &pt) {
    return this->orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

int RangeSlider::pixelPosToRangeValue(int pos) {
    QStyleOptionSlider opt;
    this->initStyleOption(&opt);
    QStyle* style = QApplication::style();

    QRect gr = style->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    int slider_length, slider_min, slider_max;
    if (this->orientation() == Qt::Horizontal) {
        slider_length = sr.width();
        slider_min = gr.x();
        slider_max = gr.right() - slider_length + 1;
    } else {
        slider_length = sr.height();
        slider_min = gr.y();
        slider_max = gr.bottom() - slider_length + 1;
    }

    return style->sliderValueFromPosition(this->minimum(), this->maximum(), pos-slider_min, slider_max-slider_min, opt.upsideDown);
}


