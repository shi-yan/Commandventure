#include "MinimapNavigator.h"

MinimapNavigator::MinimapNavigator(QWidget *parent)
    :QWidget(parent)
{

}

QPixmap* MinimapNavigator::getCurrentScreenCachedImage()
{
    return &m_currentScreenCachedImage;
}

void MinimapNavigator::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QRect currentRect = m_currentScreenCachedImage.rect();
    QRect destRect = QRect(0,0,192, currentRect.height() * 192.0f / currentRect.width());



    painter.drawPixmap(destRect, m_currentScreenCachedImage.scaled(destRect.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation), destRect);

  //  QPainter painter(this);
  //  painter.fillRect(rect(), Qt::red);
}
