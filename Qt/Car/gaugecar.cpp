#include "gaugecar.h"
#include <QtMath>
#include <QPainter>
#include <QTimer>

GaugeCar::GaugeCar(QWidget *parent) : QWidget(parent)
{
    this->setRange(0, 100);
    this->setPieStyle(PieStyle::PieStyle_Three);
    this->setPointerStyle(PointerStyle::PointerStyle_Indicator);
    this->setPointerColor("#DF6969");
    this->setAnimation(true);
    this->setAnimationStep(1);
    this->setTextColor("#7F7F7F");
    this->setValue(80);
    this->setPrecision(0);

    this->setSartAngle(45);
    this->setEndAngle(45);

    this->setScaleColor("#D0D0D0");
    this->setScaleMajor(10);
    this->setScaleMinor(10);
    this->setScaleColor("#DCDCDC");

    this->setPieColorStart("#18BD9B");
    this->setPieColorMid("#DADA00");
    this->setPieColorEnd("#FF6B6B");

    this->setOuterCircleColor("#505050");
    this->setInnerCircleColor("#3A3B38");
    this->setCoverCircleColor("#7F7F7F");
    this->setShowOverlay(false);
    this->setOverlayColor("#7F7F7F");
    this->setCenterCircleColor("#C8C8C8");
    this->currentValue = 0;
    timer = new QTimer();

    connect(this,  SIGNAL(valueChanged()), this, SLOT(paintEvent(QPaintEvent *event)));
    timer->start();
}

void GaugeCar::paintEvent(QPaintEvent *event)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制外圆
    drawOuterCircle(&painter);
    //绘制内圆
    drawInnerCircle(&painter);
    //绘制饼圆
    drawPie(&painter);
    //绘制覆盖圆用以遮住饼圆多余部分
    drawCoverCircle(&painter);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);
    //根据指针样式进行绘制指针
    if(pointerStyle == PointerStyle::PointerStyle_Circle){
        drawPointerCircle(&painter);
    }
    else if (pointerStyle == PointerStyle::PointerStyle_Triangle) {
        drawPointerTriangle(&painter);
    }
    else if (pointerStyle == PointerStyle::PointerStyle_Indicator) {
        drawPointerIndicator(&painter);
    }
    else if (pointerStyle == PointerStyle::PointerStyle_IndicatorR) {
        drawPointerIndicatorR(&painter);
    }
    //绘制指针中心圆外边框
    drawRoundCircle(&painter);
    //绘制指针中心圆
    drawCenterCircle(&painter);
    //绘制当前值
    drawValue(&painter);
    //绘制遮罩层
    drawOverlay(&painter);
    update();
}

void GaugeCar::drawOuterCircle(QPainter *painter){
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawInnerCircle(QPainter *painter){
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(innerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawCoverCircle(QPainter *painter){
    int radius = 50;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(coverCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawPie(QPainter *painter){
    int radius = 60;
    painter->save();
    painter->setPen(Qt::NoPen);
    QRectF rect(-radius, -radius, radius * 2, radius * 2);
    if(pieStyle == PieStyle::PieStyle_Three){
        double angleAll = 360.0 - startAngle - endAngle;
        double angleStart = angleAll * 0.7;
        double angleMid = angleAll * 0.15;
        double angleEnd = angleAll * 0.15;

        int offset = 3;
        //绘制开始饼圆
        painter->setBrush(pieColorStart);
        painter->drawPie(rect, (270 - startAngle - angleStart) * 16, angleStart * 16);
        //绘制中间饼圆
        painter->setBrush(pieColorMid);
        painter->drawPie(rect, (270 - startAngle - angleStart - angleMid) * 16 + offset, angleMid * 16);
        //绘制结束饼圆
        painter->setBrush(pieColorEnd);
        painter->drawPie(rect, (270 - startAngle - angleStart - angleMid - angleEnd) * 16 + offset * 2, angleEnd * 16);

    }
    else if (pieStyle == PieStyle::PieStyle_Current) {
        double angleAll = 360.0 - startAngle - endAngle;
        double angleCurrent = angleAll * ((currentValue - minValue) / (maxValue - minValue));
        double angleOther = angleAll - angleCurrent;

        //绘制当前值饼圆
        painter->setBrush(pieColorStart);
        painter->drawPie(rect, (270 - startAngle - angleCurrent) * 16, angleCurrent * 16);
        //绘制剩余值饼圆
        painter->setBrush(pieColorEnd);
        painter->drawPie(rect, (270 - startAngle - angleCurrent - angleOther) * 16, angleOther * 16);
    }
    painter->restore();

}

void GaugeCar::drawScale(QPainter *painter){
    int radius = 72;
    painter->save();

    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;

    QPen pen;
    pen.setColor(scaleColor);
    pen.setCapStyle(Qt::RoundCap);

    for(int i = 0; i <= steps; ++i){
        if(i % scaleMinor == 0){
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 10, 0, radius);
        }
        else {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 5, 0, radius);
        }
        painter->rotate(angleStep);

    }

    painter->restore();
}

void GaugeCar::drawScaleNum(QPainter *painter){
    int radius = 82;
    painter->save();
    painter->setPen(scaleColor);

    double startRad = (360.0 - startAngle - 90) * (M_PI / 180);// 5pi/4
    double deltaRad = (360.0 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;// 3pi / 20

    for(int i = 0; i <= scaleMajor; ++i){
        double sina = qSin(startRad - i * deltaRad);
        double cosa = qCos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;

        QString strValue = QString("%1").arg(value, 0, 'f', precision);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 6;
        painter->drawText(x, y, strValue);
    }
    painter->restore();
}

void GaugeCar::drawPointerCircle(QPainter *painter){
    int radius = 6;
    int offset = 30;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * currentValue;
    painter->rotate(degRotate);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);

    painter->restore();
}

void GaugeCar::drawPointerIndicator(QPainter *painter){
    int radius = 75;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void GaugeCar::drawPointerIndicatorR(QPainter *painter){
    int radius = 75;
    painter->save();
    painter->setOpacity(1.0);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(pointerColor);
    painter->setPen(pen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    //增加绘制圆角直线 与之前的三角形重叠， 形成圆角指针
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(4);
    painter->setPen(pen);
    painter->drawLine(0, 0, 0, radius);

    painter->restore();
}

void GaugeCar::drawPointerTriangle(QPainter *painter){
    int radius = 10;
    int offset = 38;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeCar::drawRoundCircle(QPainter *painter){
    int radius = 18;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);

    painter->restore();
}

void GaugeCar::drawCenterCircle(QPainter *painter){
    int radius = 15;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(centerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawValue(QPainter *painter){
    int radius = 100;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(18);
    painter->setFont(font);

    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1").arg(static_cast<double>(currentValue), 0, 'f', precision);

    painter->drawText(textRect, Qt::AlignCenter, strValue);
    painter->restore();
}

void GaugeCar::drawOverlay(QPainter *painter){
    if(!showOverlay){
        return ;
    }
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);

    QPainterPath smallCircle, bigCircle;
    radius -= 1;
    smallCircle.addEllipse(-radius, -radius, radius * 2, radius * 2);
    radius *= 2;
    bigCircle.addEllipse(-radius, -radius + 140, radius * 2, radius * 2);

    QPainterPath highlight = smallCircle - bigCircle;

    QLinearGradient linearGradient(0, -radius / 2, 0, 0);
    overlayColor.setAlpha(100);
    linearGradient.setColorAt(0.0, overlayColor);
    overlayColor.setAlpha(30);
    linearGradient.setColorAt(1.0, overlayColor);
    painter->setBrush(linearGradient);
    painter->rotate(-20);
    painter->drawPath(highlight);

    painter->restore();
}

void GaugeCar::updataValue()
{
    if(this->currentValue < 100){
        ++this->currentValue;
    }
    emit valueChanged();
}

double GaugeCar::getMinValue() const
{
    return this->minValue;
}

double GaugeCar::getMaxValue() const
{
    return this->maxValue;
}

double GaugeCar::getValue() const
{
    return this->value;
}

int GaugeCar::getPrecision() const
{
    return this->precision;
}

int GaugeCar::getScaleMajor() const
{
    return this->scaleMajor;
}

int GaugeCar::getScaleMinor() const
{
    return this->scaleMinor;
}

int GaugeCar::getSartAngle() const
{
    return this->startAngle;
}

int GaugeCar::geEndAngle() const
{
    return this->endAngle;
}

bool GaugeCar::getAnimation() const
{
    return this->animation;
}

double GaugeCar::getAnimationStep() const
{
    return this->animationStep;
}

QColor GaugeCar::getOuterCircleColor()const
{
    return this->outerCircleColor;
}

QColor GaugeCar::getInnerCircleColor() const
{
    return this->innerCircleColor;
}

QColor GaugeCar::getCoverCircleColor() const
{
    return this->coverCircleColor;
}

QColor GaugeCar::getPieColorStart() const
{
    return this->pieColorStart;
}

QColor GaugeCar::getPieColorMid() const
{
    return this->pieColorMid;
}

QColor GaugeCar::getPieColorEnd() const
{
    return this->pieColorEnd;
}

QColor GaugeCar::getScaleColor() const
{
    return this->scaleColor;
}

QColor GaugeCar::getPointerColor() const
{
    return this->pointerColor;
}

QColor GaugeCar::getCenterCircleColor() const
{
    return this->centerCircleColor;
}

QColor GaugeCar::getTextColor() const
{
    return this->textColor;
}

bool GaugeCar::getShowOverlay()
{
    return this->showOverlay;
}

QColor GaugeCar::getOverlayColor()
{
    return this->overlayColor;
}

GaugeCar::PieStyle GaugeCar::getPieStyle()
{
    return this->pieStyle;
}

GaugeCar::PointerStyle GaugeCar::getPointerStyle()
{
    return this->pointerStyle;
}

void GaugeCar::setRange(double minValue, double maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void GaugeCar::setRange(int minValue, int maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void GaugeCar::setMinValue(double minValue)
{
    this->minValue = minValue;
}

void GaugeCar::setMaxValue(double maxValue)
{
    this->maxValue = maxValue;
}

void GaugeCar::setValue(double value)
{
    this->currentValue = value;
    emit valueChanged();
}

void GaugeCar::setValue(int value)
{
    this->currentValue = value;
    emit valueChanged();
}

void GaugeCar::setPrecision(int precision)
{
    this->precision = precision;
}

void GaugeCar::setScaleMajor(int scaleMajor)
{
    this->scaleMajor = scaleMajor;
}

void GaugeCar::setScaleMinor(int scaleMinor)
{
    this->scaleMinor = scaleMinor;
}

void GaugeCar::setSartAngle(int startAngle)
{
    this->startAngle = startAngle;
}

void GaugeCar::setEndAngle(int endAgle)
{
    this->endAngle = endAgle;
}

void GaugeCar::setAnimation(bool animation)
{
    this->animation = animation;
}

void GaugeCar::setAnimationStep(double animationStep)
{
    this->animationStep = animationStep;
}

void GaugeCar::setOuterCircleColor(const QColor &outerCircleColor)
{
    this->outerCircleColor = outerCircleColor;
}

void GaugeCar::setInnerCircleColor(const QColor &innerCircleColor)
{
    this->innerCircleColor = innerCircleColor;
}

void GaugeCar::setCoverCircleColor(const QColor &coverCircleColor)
{
    this->coverCircleColor = coverCircleColor;
}

void GaugeCar::setPieColorStart(const QColor &pieColorStart)
{
    this->pieColorStart = pieColorStart;
}

void GaugeCar::setPieColorMid(const QColor &pieColorMid)
{
    this->pieColorMid = pieColorMid;
}

void GaugeCar::setPieColorEnd(const QColor &pieColorEnd)
{
    this->pieColorEnd = pieColorEnd;
}

void GaugeCar::setScaleColor(const QColor &scaleColor)
{
    this->scaleColor = scaleColor;
}

void GaugeCar::setPointerColor(const QColor &pointerColor)
{
    this->pointerColor = pointerColor;
}

void GaugeCar::setCenterCircleColor(const QColor &centerCircleColor)
{
    this->centerCircleColor = centerCircleColor;
}

void GaugeCar::setTextColor(const QColor &textColor)
{
    this->textColor = textColor;
}

void GaugeCar::setShowOverlay(bool showOverlay)
{
    this->showOverlay = showOverlay;
}

void GaugeCar::setOverlayColor(const QColor &overlayColor)
{
    this->overlayColor = overlayColor;
}

void GaugeCar::setPieStyle(const GaugeCar::PieStyle &pieStyle)
{
    this->pieStyle = pieStyle;
}

void GaugeCar::setPointerStyle(const GaugeCar::PointerStyle &pointerStyle)
{
    this->pointerStyle = pointerStyle;
}






