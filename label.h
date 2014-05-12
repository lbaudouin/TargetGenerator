#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include <QResizeEvent>

class Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QPixmap m_pix;

signals:

public slots:
    void setFullSizePixmap(const QPixmap &pixmap);

};

#endif // LABEL_H
