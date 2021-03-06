#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QDebug>

#include "knmusiceqslider.h"

KNMusicEQSlider::KNMusicEQSlider(QWidget *parent) :
    KNAbstractSlider(parent)
{
    //Initial all the pixmaps.
    m_sliderBase=QPixmap(":/Music/Resources/Music/EQs/sliderbase.png");
    m_sliderBaseTop=QPixmap(":/Music/Resources/Music/EQs/sliderbasetop.png");
    m_sliderBaseBottom=QPixmap(":/Music/Resources/Music/EQs/sliderbasebottom.png");
    m_sliderButton=QPixmap(":/Music/Resources/Music/EQs/sliderbutton.png");
    setMinimumWidth(m_sliderButton.width());
    m_topRange=m_sliderBase.height();
    m_topMargin=m_sliderBaseTop.height();
    m_bottomMargin=m_sliderBase.height()+m_topMargin;
    m_mouseRange=m_bottomMargin-m_topMargin;
    connect(this, &KNAbstractSlider::rangeChanged,
            [=]
            {
                updateButtonPosition();
                update();
            });
}

void KNMusicEQSlider::setValue(float value)
{
    KNAbstractSlider::setValue(value);
    updateButtonPosition();
    update();
}

void KNMusicEQSlider::paintEvent(QPaintEvent *event)
{
    KNAbstractSlider::paintEvent(event);
    QPainter painter(this);
    int sliderX=(width()-m_sliderBase.width())>>1;
    painter.drawPixmap(sliderX,
                       0,
                       m_sliderBaseTop.width(),
                       m_sliderBaseTop.height(),
                       m_sliderBaseTop);
    painter.drawPixmap(sliderX,
                       m_topMargin,
                       m_sliderBase.width(),
                       m_mouseRange,
                       m_sliderBase);
    painter.drawPixmap(sliderX,
                       m_bottomMargin,
                       m_sliderBaseBottom.width(),
                       m_sliderBaseBottom.height(),
                       m_sliderBaseBottom);
    painter.drawPixmap(((width()-m_sliderButton.width())>>1)-1,
                       m_buttonTop,
                       m_sliderButton.width(),
                       m_sliderButton.height(),
                       m_sliderButton);
}

void KNMusicEQSlider::resizeEvent(QResizeEvent *event)
{
    KNAbstractSlider::resizeEvent(event);
    m_bottomMargin=height()-m_sliderBaseTop.height()-m_sliderBaseBottom.height();
    m_mouseRange=m_bottomMargin-m_topMargin;
    updateButtonPosition();
}

void KNMusicEQSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isSliderDown)
    {
        setValueFromMouseParam((float)event->y());
        emit sliderMoved(m_value);
    }
}

void KNMusicEQSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isSliderDown)
    {
        setValueFromMouseParam((float)event->y());
        emit sliderMoved(m_value);
    }
    KNAbstractSlider::mouseReleaseEvent(event);
}

void KNMusicEQSlider::updateButtonPosition()
{
    m_buttonTop=(m_maximum-m_value)/m_range*m_mouseRange+
            m_topMargin-(m_sliderButton.height()>>1);
}

void KNMusicEQSlider::setValueFromMouseParam(float mouseParam)
{
    if(mouseParam<m_topMargin) mouseParam=m_topMargin;
    if(mouseParam>m_bottomMargin) mouseParam=m_bottomMargin;
    //This is a hack.
    m_buttonTop=mouseParam-m_topMargin;
    setValue(m_maximum-(mouseParam-m_topMargin)/m_mouseRange*m_range);
    update();
}
