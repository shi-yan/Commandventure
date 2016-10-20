#include "MinimapNavigator.h"

MinimapNavigator::MinimapNavigator(QWidget *parent)
    :QWidget(parent),
      m_minimapImage(160, 1000, QImage::Format_RGBA8888),
      m_painter(),
      m_lineCount(0)
{

}

void MinimapNavigator::pushLine(QVector<Konsole::Character> &c)
{
    if (m_lineCount < 100)
    {
        if (text.size() != c.size())
        {
            text.resize(c.size());
        }

        QChar *charbuf = text.data();

        for(int i = 0;i<c.size();++i)
        {
            charbuf[i] = c[i].character;
        }


        m_painter.begin(&m_minimapImage);
        m_painter.setPen(Qt::blue);
        m_painter.setFont(QFont("Arial",5));
        m_painter.drawText(0,m_lineCount*5, text);


        m_lineCount ++;
        m_painter.end();
    }

    m_minimapImage.save("test.png");
}
