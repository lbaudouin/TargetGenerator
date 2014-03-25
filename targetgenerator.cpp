#include "targetgenerator.h"
#include "ui_targetgenerator.h"

TargetGenerator::TargetGenerator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TargetGenerator)
{
    ui->setupUi(this);

    m_highlighter = new Highlighter(ui->valuesEdit->document());
    m_highlighter->setMaximum( ui->messageSpinBox->maximum() );

    //Warning: only if same number of bits
    m_highlighter->setForbiddenValues( QList<int>() << ui->headerSpinBox->value() );

    QButtonGroup *typeGroup = new QButtonGroup;
    typeGroup->addButton(ui->dotsButton);
    typeGroup->addButton(ui->targetsButton);
    typeGroup->addButton(ui->chessboardButton);
    typeGroup->setExclusive(true);

    QButtonGroup *positionGroup = new QButtonGroup;
    positionGroup->addButton(ui->stantardButton);
    positionGroup->addButton(ui->shiftedButton);
    positionGroup->setExclusive(true);

    //Target connections
    connect(ui->diameterSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateTarget()));
    connect(ui->typeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateTarget()));
    connect(ui->headerSpinBox,SIGNAL(valueChanged(int)),this,SLOT(headerChanged(int)));
    connect(ui->headerSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateTarget()));
    connect(ui->messageSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateTarget()));
    connect(ui->headerBitsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(nbHearderBitsChanged(int)));
    connect(ui->messageBitsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(nbMessageBitsChanged(int)));
    connect(ui->parityBitCheckBox,SIGNAL(clicked()),this,SLOT(updateTarget()));
    connect(ui->inverseParityCheckBox,SIGNAL(clicked()),this,SLOT(updateTarget()));
    connect(ui->grayCodeCheckBox,SIGNAL(clicked()),this,SLOT(updateTarget()));

    //Grid connections
    connect(typeGroup,SIGNAL(buttonClicked(int)),this,SLOT(gridTypeChanged(int)));
    connect(positionGroup,SIGNAL(buttonClicked(int)),this,SLOT(updateGrid()));
    connect(ui->dotDistanceSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateGrid()));
    connect(ui->targetDistanceSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateGrid()));
    connect(ui->horizontalSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateGrid()));
    connect(ui->verticalSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateGrid()));
    connect(ui->dotSizeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateGrid()));
    connect(ui->squareSizeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateGrid()));
    connect(ui->valuesEdit,SIGNAL(textChanged()),this,SLOT(updateGrid()));

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
    connect(ui->generateButton,SIGNAL(clicked()),this,SLOT(pressGenerate()));

    //Save connections
    connect(ui->pdfButton,SIGNAL(clicked()),this,SLOT(pressExportPDF()));
    connect(ui->imageButton,SIGNAL(clicked()),this,SLOT(pressExportImage()));
    connect(ui->svgButton,SIGNAL(clicked()),this,SLOT(pressExportSVG()));

    gridTypeChanged(1);

    updateTarget();
    updateGrid();
}

TargetGenerator::~TargetGenerator()
{
    delete ui;
}

void TargetGenerator::headerChanged(int header)
{
    m_highlighter->setForbiddenValues( QList<int>() << header);
}

void TargetGenerator::nbHearderBitsChanged(int nbBits)
{
    ui->headerSpinBox->setMaximum(pow(2,nbBits)-1);
    updateTarget();
}

void TargetGenerator::nbMessageBitsChanged(int nbBits)
{
    ui->messageSpinBox->setMaximum(pow(2,nbBits)-1);
    m_highlighter->setMaximum( ui->messageSpinBox->maximum() );
    updateTarget();
}


void TargetGenerator::drawTarget(QPainter &painter, QPointF center, TargetType type, double diameter, int header, int header_bits, int message, int message_bits, bool gray_code, bool parity_bit, bool inverse_parity)
{
    QString headerString,messageString;

    if(gray_code){
        headerString  = dec2gray(header,header_bits);
        messageString = dec2gray(message,message_bits);
    }else{
        headerString  = dec2bin(header,header_bits);
        messageString = dec2bin(message,message_bits);
    }

    QString code = headerString + messageString;
    if(parity_bit){
        if(inverse_parity){
            messageString += parity(code,true);
        }else{
            messageString += parity(code);
        }
    }
    code = headerString + messageString;

    switch(type){
    case OneBlob: drawOneBlob(painter, center, diameter, code); break;
    case ThreeBlobs: drawThreeBlobs(painter, center, diameter, code); break;
    case TwoRings: drawTwoRings(painter, center, diameter, headerString, messageString); break;
    }
}

void TargetGenerator::drawOneBlob(QPainter &painter, QPointF center, double diameter, QString code)
{
    double radius = diameter/2.0;

    //Black code
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    int nb_bits = code.size();
    double angle = 360.0 / nb_bits;

    for(int i=0;i<nb_bits;i++){
        if(code.at(i)=='1'){
            //The startAngle and spanAngle must be specified in 1/16th of a degree
            painter.drawPie(QRectF(center.x()-diameter/2.0,center.y()-diameter/2.0,diameter,diameter),i*angle*16,angle*16);
        }
    }

    //White circle
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawEllipse(center,radius*2.0/3.0,radius*2.0/3.0);

    //Black circle
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center,radius*1.0/2.0,radius*1.0/2.0);
}


void TargetGenerator::drawThreeBlobs(QPainter &painter, QPointF center, double diameter, QString code)
{
    double radius = diameter/2.0;

    //Black code
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    int nb_bits = code.size();
    double angle = 360.0 / nb_bits;

    for(int i=0;i<nb_bits;i++){
        if(code.at(i)=='1'){
            //The startAngle and spanAngle must be specified in 1/16th of a degree
            painter.drawPie(QRectF(center.x()-diameter/2.0,center.y()-diameter/2.0,diameter,diameter),i*angle*16,angle*16);
        }
    }

    //White circle
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawEllipse(center,radius*4.0/5.0,radius*4.0/5.0);

    //Black circle
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center,radius*3.0/5.0,radius*3.0/5.0);

    //White circle
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawEllipse(center,radius*2.0/5.0,radius*2.0/5.0);

    //Black circle
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center,radius*1.0/5.0,radius*1.0/5.0);
}


void TargetGenerator::drawTwoRings(QPainter &painter, QPointF center, double diameter, QString header, QString message)
{
    double radius = diameter/2.0;

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center,radius,radius);

    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawEllipse(center,radius*5.0/6.0,radius*5.0/6.0);

    //Message ring
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    double angle = 360.0 / message.size();
    for(int i=0;i<message.size();i++){
        if(message.at(i)=='1'){
            //The startAngle and spanAngle must be specified in 1/16th of a degree
            painter.drawPie(QRectF(center.x()-diameter*4.0/12.0,center.y()-diameter*4.0/12.0,diameter*4.0/6.0,diameter*4.0/6.0),i*angle*16,angle*16);
        }
    }

    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawEllipse(center,radius*2.98/6.0,radius*2.98/6.0);

   //Header Ring
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    angle = 360.0 / header.size();
    for(int i=0;i<header.size();i++){
        if(header.at(i)=='1'){
            //The startAngle and spanAngle must be specified in 1/16th of a degree
            painter.drawPie(QRectF(center.x()-diameter*3.0/12.0,center.y()-diameter*3.0/12.0,diameter*3.0/6.0,diameter*3.0/6.0),i*angle*16,angle*16);
        }
    }

    //White circle
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawEllipse(center,radius*2.0/6.0,radius*2.0/6.0);

    //Black circle
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center,radius*1.0/6.0,radius*1.0/6.0);
}

void TargetGenerator::drawTarget(QPainter &painter, int value, QPointF center, double diameter)
{
    TargetType targetType = OneBlob;
    switch(ui->typeComboBox->currentIndex()){
    case 0: targetType = OneBlob; break;
    case 1: targetType = ThreeBlobs; break;
    case 2: targetType = TwoRings; break;
    }

    if(diameter<0)
        diameter = pixelPerMillimeter * ui->diameterSpinBox->value();

    int nbHearderBits = ui->headerBitsSpinBox->value();
    int header = ui->headerSpinBox->value();
    int nbMessageBits = ui->messageBitsSpinBox->value();
    int message = ui->messageSpinBox->value();
    if(value<0)
        value = message;

    bool grayCode = ui->grayCodeCheckBox->isChecked();
    bool parityBit = ui->parityBitCheckBox->isChecked();
    bool inverseParity = ui->inverseParityCheckBox->isChecked();

    if(center==QPointF(-1,-1))
        center = QPointF(1.2*diameter/2.0,1.2*diameter/2.0);

    drawTarget(painter,center,targetType,diameter,header,nbHearderBits,value,nbMessageBits,grayCode,parityBit,inverseParity);
}

void TargetGenerator::drawDotGrid(QPainter &painter, int nbCols, int nbRows, int distance, int dotSize)
{
    double distancePixel = pixelPerMillimeter * distance;
    double dotSizePixel = pixelPerMillimeter * dotSize;

    GridPosition position = Stantard;
    if(ui->stantardButton->isChecked()){
        position = Stantard;
    }else if(ui->shiftedButton->isChecked()){
        position = Shifted;
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    for(int col=0;col<nbCols;col++){
        for(int row=0;row<nbRows;row++){
            QPointF pt(QPointF(distancePixel/2.0+col*distancePixel,distancePixel/2.0+row*distancePixel));
            if(col%2==1 && position==Shifted)
                pt.ry() += distancePixel/2.0;
            painter.drawEllipse(pt,dotSizePixel/2.0,dotSizePixel/2.0);
        }
    }
}

void TargetGenerator::drawTargetGrid(QPainter &painter, int nbCols, int nbRows, int distance, int diameter)
{
    double distancePixel = pixelPerMillimeter * distance;
    double diameterPixel = pixelPerMillimeter * diameter;

    GridPosition position = Stantard;
    if(ui->stantardButton->isChecked()){
        position = Stantard;
    }else if(ui->shiftedButton->isChecked()){
        position = Shifted;
    }

    QString valuesString = ui->valuesEdit->toPlainText();
    valuesString.replace(","," ");
    valuesString.replace(";"," ");
    valuesString.replace("-"," ");
    valuesString.replace("."," ");
    valuesString.replace("\n"," ");
    valuesString.replace("\t"," ");

    QStringList valuesStringList = valuesString.split(" ",QString::SkipEmptyParts);
    QList<int> values;

    for(int i=0;i<valuesStringList.size();i++){
        bool ok;
        int val = valuesStringList.at(i).toInt(&ok);
        if(ok){
            if(val>ui->messageSpinBox->maximum())
                values.push_back(0);
            else
                values.push_back(val);
        }
    }

    for(int col=0;col<nbCols;col++){
        for(int row=0;row<nbRows;row++){
            int index = row*nbCols+col;
            int value = (index<values.size()?values[index]:0);

            QPointF pt(QPointF(distancePixel/2.0+col*distancePixel,distancePixel/2.0+row*distancePixel));
            if(col%2==1 && position==Shifted)
                pt.ry() += distancePixel/2.0;

            drawTarget(painter, value, pt, diameterPixel);
        }
    }
}

void TargetGenerator::drawChessboard(QPainter &painter, int nbCols, int nbRows, int size)
{
    double sizePixel = pixelPerMillimeter * size;

    painter.setPen(Qt::black);
    for(int col=0;col<nbCols;col++){
        for(int row=0;row<nbRows;row++){
            if( (col+row)%2 ){
                painter.fillRect(col*sizePixel,row*sizePixel,sizePixel,sizePixel,Qt::black);
            }
        }
    }
}

void TargetGenerator::drawGrid(QPainter &painter)
{
    GridType type = Dots;
    if(ui->dotsButton->isChecked()){
        type = Dots;
    }else if(ui->targetsButton->isChecked()){
        type = Targets;
    }else if(ui->chessboardButton->isChecked()){
        type = Chessboard;
    }

    int h = ui->horizontalSpinBox->value();
    int w = ui->verticalSpinBox->value();
    int dotSize = ui->dotSizeSpinBox->value();
    int targetSize = ui->targetSizeSpinBox->value();
    int size = ui->squareSizeSpinBox->value();

    switch(type){
    case Dots:
        drawDotGrid(painter,h,w,ui->dotDistanceSpinBox->value(),dotSize);
        break;
    case Targets:
        drawTargetGrid(painter,h,w,ui->targetDistanceSpinBox->value(),targetSize);
        break;
    case Chessboard:
        drawChessboard(painter,h,w,size);
        break;
    }
}

void TargetGenerator::draw(QPainter &painter)
{
    if(ui->tabWidget->currentIndex()==0){
        drawTarget(painter);
    }else{
        drawGrid(painter);
    }
}

QString TargetGenerator::dec2bin(int dec, int nb_bits)
{
    QString bin = QString::number(dec,2);
    if(bin.size()>nb_bits){
        //QMessageBox::warning(this,tr("Warning"),tr("Not enough bits to display the decimal number"));
    }
    for(int i=bin.size();i<nb_bits;i++)
        bin.prepend('0');
    return bin;
}

QString TargetGenerator::dec2gray(int dec, int nb_bits)
{
    QString bin = dec2bin(dec,nb_bits);
    return  bin2gray(bin);
}

QString TargetGenerator::bin2gray(QString bin)
{
    QString gray = bin.at(0);
    int cnt = 0;
    while(cnt<bin.size()-1){
        gray += (bin[cnt] != bin[cnt+1]?'1':'0');
        cnt++;
    }
    return gray;
}

QString TargetGenerator::parity(QString bin, bool inverse)
{
    return QString::number( (bin.count('1') + (inverse?1:0))%2 );
}

void TargetGenerator::updateTarget()
{
    QPixmap pix( computeSize() );
    pix.fill(Qt::white);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    drawTarget(painter);

    QString name = generateName();

    painter.drawText(pix.rect(),name);

    ui->targetLabel->setPixmap( pix.scaled(600,600,Qt::KeepAspectRatio) );
}

void TargetGenerator::updateGrid()
{
    QPixmap pix(computeSize());
    pix.fill(Qt::white);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid(painter);

    ui->gridLabel->setPixmap( pix.scaled(600,600,Qt::KeepAspectRatio) );
}

void TargetGenerator::tabChanged(int tabIndex)
{
    if(tabIndex==0)
        updateTarget();
    if(tabIndex==1)
        updateGrid();
}

void TargetGenerator::gridTypeChanged(int)
{
    GridType gridType = Dots;
    if(ui->dotsButton->isChecked()){
        gridType = Dots;
    }else if(ui->targetsButton->isChecked()){
        gridType = Targets;
    }else if(ui->chessboardButton->isChecked()){
        gridType = Chessboard;
    }

    switch(gridType){
    case Targets:
        ui->positionGroupBox->show();
        ui->dotGroupBox->hide();
        ui->targetGroupBox->show();
        ui->squareGroupBox->hide();
        ui->valuesGroupBox->show();
        break;
    case Dots:
        ui->positionGroupBox->show();
        ui->dotGroupBox->show();
        ui->targetGroupBox->hide();
        ui->squareGroupBox->hide();
        ui->valuesGroupBox->hide();
        break;
    case Chessboard:
        ui->positionGroupBox->hide();
        ui->dotGroupBox->hide();
        ui->targetGroupBox->hide();
        ui->squareGroupBox->show();
        ui->valuesGroupBox->hide();
        break;
    }

    updateGrid();
}


void TargetGenerator::pressGenerate()
{
    int nb = ui->horizontalSpinBox->value() * ui->verticalSpinBox->value();
    ui->valuesEdit->clear();

    int header = ui->headerSpinBox->value();
    int max = pow(2,ui->messageBitsSpinBox->value())-1;

    if(nb>=max-1){
        qWarning() << "Too much targets";
        return;
    }

    QList<int> values;

    for(int i=0;i<nb;){
        int m = rand()%max;
        if(m==header)
            continue;
        if(values.contains(m))
            continue;
        values.push_back(m);
        i++;
    }

    qSort(values);

    QString text;
    foreach(int val, values)
        text += QString::number(val) + " ";

    ui->valuesEdit->setPlainText(text);
}

void TargetGenerator::pressExportImage()
{
    QList<QByteArray> supported = QImageWriter::supportedImageFormats();
    QStringList filters;
    foreach(QByteArray filter, supported){
        filters << "*." + QString(filter);
    }

    QString name = generateName();
    QString filepath = QFileDialog::getSaveFileName(this,tr("Save target"),"targets/"+name+".png",filters.join(";;"));

    if(filepath.isEmpty())
        return;

    QPixmap pix(computeSize());
    pix.fill(Qt::white);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    if(ui->tabWidget->currentIndex()==0)
        painter.drawText(pix.rect(),name);
    else{
        QRect rect = pix.rect();
        painter.drawText(QRect(rect.x(),rect.height()-25,rect.width(),25),name);
    }

    draw(painter);

    pix.save(filepath);
}

void TargetGenerator::pressExportPDF()
{
    QString name = generateName();

    QString filepath = QFileDialog::getSaveFileName(this,tr("Save target"),"targets/"+name+".pdf","*.pdf");
    if(filepath.isEmpty())
        return;

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filepath);

    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);

    if(ui->tabWidget->currentIndex()==0)
        painter.drawText(printer.pageRect(),name);
    else{
        QRect rect = printer.pageRect();
        painter.drawText(QRect(rect.x(),rect.height()-25,rect.width(),25),name);
    }

    draw(painter);
}

void TargetGenerator::pressExportSVG()
{
    QString name = generateName();

    QString filepath = QFileDialog::getSaveFileName(this,tr("Save target"),"targets/"+name+".svg","*.svg");
    if(filepath.isEmpty())
        return;

    QSize size = computeSize();

    QSvgGenerator generator;
    generator.setFileName(filepath);
    generator.setSize(size);
    generator.setViewBox(QRect(QPoint(0,0), size));
    generator.setTitle(name);
    generator.setDescription("Target\nType: %1");

    QPainter painter(&generator);
    painter.fillRect(QRect(QPoint(0,0), size),Qt::white);

    if(ui->tabWidget->currentIndex()==0)
        painter.drawText(generator.viewBox(),name);
    else{
        QRect rect = generator.viewBox();
        painter.drawText(QRect(rect.x(),rect.height()-25,rect.width(),25),name);
    }

    draw(painter);
}

QString TargetGenerator::generateName()
{
    if(ui->tabWidget->currentIndex()==0){

        TargetType targetType;
        switch(ui->typeComboBox->currentIndex()){
        case 0: targetType = OneBlob; break;
        case 1: targetType = ThreeBlobs; break;
        case 2: targetType = TwoRings; break;
        default: targetType = OneBlob; break;
        }

        int header_bits = ui->headerBitsSpinBox->value();
        int header = ui->headerSpinBox->value();
        int message_bits = ui->messageBitsSpinBox->value();
        int message = ui->messageSpinBox->value();
        bool gray_code = ui->grayCodeCheckBox->isChecked();
        bool parity_bit = ui->parityBitCheckBox->isChecked();
        bool inverse_parity = ui->inverseParityCheckBox->isChecked();

        QString modelName;
        switch(targetType){
        case OneBlob: modelName = "1B"; break;
        case ThreeBlobs: modelName = "3B"; break;
        case TwoRings: modelName = "2R"; break;
        }

        QString headerName = QString("%1h%2").arg(header_bits).arg(header);
        QString messageName = QString("%1m%2").arg(message_bits).arg(message);

        QString headerString,messageString;

        if(gray_code){
            headerString  = dec2gray(header,header_bits);
            messageString = dec2gray(message,message_bits);
        }else{
            headerString  = dec2bin(header,header_bits);
            messageString = dec2bin(message,message_bits);
        }

        QString code = headerString + messageString;

        QString parityName;
        if(parity_bit){
            if(inverse_parity){
                messageString += parity(code,true);
                parityName = "pi" + code.right(1) + "_";
            }else{
                messageString += parity(code);
                parityName = "p" + code.right(1) + "_";
            }
        }

        return QString("%1_%2_%3_%4%5").arg(modelName,headerName,messageName,parityName,(gray_code?"gray":"bin"));
    }else{

        GridType gridType = Dots;
        if(ui->dotsButton->isChecked()){
            gridType = Dots;
        }else if(ui->targetsButton->isChecked()){
            gridType = Targets;
        }else if(ui->chessboardButton->isChecked()){
            gridType = Chessboard;
        }

        GridPosition position = Stantard;
        if(ui->stantardButton->isChecked()){
            position = Stantard;
        }else if(ui->shiftedButton->isChecked()){
            position = Shifted;
        }

        int nbCols = ui->horizontalSpinBox->value();
        int nbRows = ui->verticalSpinBox->value();
        int dotDistance = ui->dotDistanceSpinBox->value();
        int targetDistance = ui->targetDistanceSpinBox->value();
        int size = ui->squareSizeSpinBox->value();

        switch(gridType){
        case Dots: return QString("grid_dots%1_%2x%3_%4").arg(position==Shifted?"_shifted":"").arg(nbCols).arg(nbRows).arg(dotDistance);
        case Targets: return QString("grid_targets%1_%2x%3_%4").arg(position==Shifted?"_shifted":"").arg(nbCols).arg(nbRows).arg(targetDistance);
        case Chessboard: return QString("grid_chessboard_%1x%2_%3").arg(nbCols).arg(nbRows).arg(size);
        default: return  QString("grid_%1x%2").arg(nbCols).arg(nbRows);
        }
    }
}

QSize TargetGenerator::computeSize()
{
    if(ui->tabWidget->currentIndex()==0){
        double size = 1.2 * pixelPerMillimeter * ui->diameterSpinBox->value();
        return QSize(size,size);
    }else{
        GridType gridType = Dots;
        if(ui->dotsButton->isChecked()){
            gridType = Dots;
        }else if(ui->targetsButton->isChecked()){
            gridType = Targets;
        }else if(ui->chessboardButton->isChecked()){
            gridType = Chessboard;
        }

        GridPosition position = Stantard;
        if(ui->stantardButton->isChecked()){
            position = Stantard;
        }else if(ui->shiftedButton->isChecked()){
            position = Shifted;
        }

        int nbCols = ui->horizontalSpinBox->value();
        int nbRows = ui->verticalSpinBox->value();
        double dotDistance = pixelPerMillimeter * ui->dotDistanceSpinBox->value();
        double targetDistance = pixelPerMillimeter * ui->targetDistanceSpinBox->value();
        double squareSize = pixelPerMillimeter * ui->squareSizeSpinBox->value();

        switch(gridType){
        case Dots: return QSize(nbCols*dotDistance, nbRows*dotDistance+(position==Shifted?dotDistance/2.0:0)) + QSize(0,25); break;
        case Targets: return QSize(nbCols*targetDistance, nbRows*targetDistance+(position==Shifted?targetDistance/2.0:0))+ QSize(0,25); break;
        case Chessboard: return QSize(nbCols*squareSize,nbRows*squareSize)+ QSize(0,25); break;
        default: return QSize(0,0);
        }
    }
}
