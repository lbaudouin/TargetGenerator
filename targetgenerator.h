#ifndef TARGETGENERATOR_H
#define TARGETGENERATOR_H

#include <QMainWindow>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageWriter>

#include <QSvgGenerator>

#include <QPrinter>
#include <QPrintDialog>

#include <QDebug>

#include "highlighter.h"

#include "math.h"

const double pixelPerMillimeter = 3.780;

namespace Ui {
class TargetGenerator;
}

class TargetGenerator : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TargetGenerator(QWidget *parent = 0);
    ~TargetGenerator();

    enum TargetType { OneBlob = 0, ThreeBlobs, TwoRings };
    enum GridType { Dots = 0, Targets, Chessboard};
    enum GridPosition { Stantard = 0, Shifted };

protected:
    void drawChessboard(QPainter &painter, int nbCols, int nbRows, int size);
    void drawDotGrid(QPainter &painter, int nbCols, int nbRows, int distance, int size);
    void drawTargetGrid(QPainter &painter, int nbCols, int nbRows, int distance, int diameter);

    QString dec2bin(int dec, int nb_bits);
    QString dec2gray(int dec, int nb_bits);
    QString bin2gray(QString bin);
    QString parity(QString bin, bool inverse = false);

    void drawTarget(QPainter &painter, QPointF center, TargetType type, double diameter, int header, int header_bits, int message, int message_bits, bool gray_code = false, bool parity_bit = false, bool inverse_parity = false);
    void drawOneBlob(QPainter &painter, QPointF center, double diameter, QString code);
    void drawThreeBlobs(QPainter &painter, QPointF center, double diameter, QString code);
    void drawTwoRings(QPainter &painter, QPointF center, double diameter, QString header, QString message);

    void drawTarget(QPainter &painter, int value = -1, QPointF center = QPointF(-1,-1), double diameter = -1);
    void drawGrid(QPainter &painter);
    void draw(QPainter &painter);
    QString generateName();
    QSize computeSize();

private:
    Ui::TargetGenerator *ui;
    Highlighter *m_highlighter;

public slots:
    void updateTarget();
    void updateGrid();

    void gridTypeChanged(int type);

    void tabChanged(int);

    void pressExportImage();
    void pressExportPDF();
    void pressExportSVG();

    void headerChanged(int);
    void nbHearderBitsChanged(int);
    void nbMessageBitsChanged(int);

    void pressGenerate();
};

#endif // TARGETGENERATOR_H
