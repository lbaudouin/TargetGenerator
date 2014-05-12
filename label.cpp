#include "label.h"

Label::Label(QWidget *parent) :
    QLabel(parent)
{
    this->setMinimumSize(200,200);
}

void Label::resizeEvent(QResizeEvent *event)
{
    QSize size = this->size();
    this->setPixmap( m_pix.scaled(size,Qt::KeepAspectRatio) );


    QLabel::resizeEvent(event);
}

void Label::setFullSizePixmap(const QPixmap &pixmap)
{
    m_pix = pixmap;
    QSize size = this->size();
    this->setPixmap( m_pix.scaled(size,Qt::KeepAspectRatio) );
}
