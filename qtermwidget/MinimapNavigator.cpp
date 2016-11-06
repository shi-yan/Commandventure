#include "MinimapNavigator.h"
#include <QDebug>

MinimapNavigator::MinimapNavigator(QWidget *parent)
    :QWidget(parent)
{
    historyHeight = 0;
}

QPixmap* MinimapNavigator::getCurrentScreenCachedImage()
{
    return &m_currentScreenCachedImage;
}

QPixmap * MinimapNavigator::getHistoryScreenCachedImage()
{
    return &m_historyScreenCachedImage;
}

void MinimapNavigator::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QRect currentRect = m_currentScreenCachedImage.rect();
    float ratio = 192.0f / m_historyScreenCachedImage.width();

    QRect destRect = QRect(0, historyHeight*ratio,192, currentRect.height() * ratio);

    float drawHeight = historyHeight;

    if (drawHeight > m_historyScreenCachedImage.height())
    {
        drawHeight = m_historyScreenCachedImage.height();
    }

    if (historyHeight > 0)
    {
        QRect sdestRect = QRect(0,0,192, m_historyScreenCachedImage.height() * ratio);
        int y = -(m_historyScreenCachedImage.height() - historyHeight);
        if (y > 0)
            y = 0;
        qDebug() << sdestRect;

        QPixmap shrink = m_historyScreenCachedImage.scaled(sdestRect.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        //shrink.save("debug.png");



        painter.drawPixmap(QRect(0,0,192, drawHeight * ratio), shrink, QRect(0, sdestRect.height() - drawHeight*ratio, sdestRect.width(), drawHeight*ratio));
        //painter.fillRect(QRect(0,0,192, historyHeight * ratio), QBrush(Qt::red));
    }

    painter.drawPixmap(QRect(0, drawHeight * ratio, 192, destRect.height()), m_currentScreenCachedImage.scaled(destRect.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation), QRect(0,0, destRect.width(), destRect.height()));


  //  QPainter painter(this);
  //  painter.fillRect(rect(), Qt::red);
}

void MinimapNavigator::scrollImage(int dx, int dy, QRect &region)
{
    m_historyScreenCachedImage.scroll(dx, dy, m_historyScreenCachedImage.rect());

    QPainter painter(&m_historyScreenCachedImage);
    QRect target(0, m_historyScreenCachedImage.height() + dy, m_historyScreenCachedImage.width(), -dy);
    painter.drawPixmap(target, m_currentScreenCachedImage, QRect(0,0,m_currentScreenCachedImage.width(), -dy));
    //qDebug() << "target" << target << region;
    //qDebug() << "history height" << m_historyScreenCachedImage.height();
    historyHeight -= dy;
    m_currentScreenCachedImage.scroll(dx, dy, region);
    //m_historyScreenCachedImage.save("debug.png");
}
