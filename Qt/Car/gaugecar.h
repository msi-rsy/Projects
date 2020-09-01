#ifndef GAUGECAR_H
#define GAUGECAR_H

#include <QWidget>
#include <QColor>


class GaugeCar : public QWidget
{
    Q_OBJECT
    Q_ENUMS(PieStyle)
    Q_ENUMS(PointerStyle)
    // 最小值
    Q_PROPERTY(double minValue READ getMinValue WRITE setMinValue)// 最小值
    Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue)// 最大值
    Q_PROPERTY(double value READ getValue WRITE setValue)// 设置目标值
    Q_PROPERTY(int precision READ getPrecision WRITE setPrecision)// 设置精确度


    Q_PROPERTY(int scaleMajor READ getScaleMajor WRITE setScaleMajor)//设置主刻度数量
    Q_PROPERTY(int scaleMinor READ getScaleMinor WRITE setScaleMinor)//设置小刻度数量
    Q_PROPERTY(int sartAngle READ getSartAngle WRITE setSartAngle)//设置开始旋转角度
    Q_PROPERTY(int endAngle READ geEndAngle WRITE setEndAngle)//设置结束旋转角度


    Q_PROPERTY(bool animation READ getAnimation WRITE setAnimation)//设置是否启用动画显示
    Q_PROPERTY(double animationStep READ getAnimationStep WRITE setAnimationStep)//设置动画显示的步长

    Q_PROPERTY(QColor outerCircleColor READ getOuterCircleColor WRITE setOuterCircleColor)//设置外圆背景颜色
    Q_PROPERTY(QColor innerCircleColor READ getInnerCircleColor WRITE setInnerCircleColor)
    //设置饼圆的三种颜色
    Q_PROPERTY(QColor pieColorStart READ getPieColorStart WRITE setPieColorStart)
    Q_PROPERTY(QColor pieColorMid READ getPieColorMid WRITE setPieColorMid)
    Q_PROPERTY(QColor pieColorEnd READ getPieColorEnd WRITE setPieColorEnd)

    Q_PROPERTY(QColor coverCircleColor READ getCoverCircleColor WRITE setCoverCircleColor)//设置覆盖圆背景颜色
    Q_PROPERTY(QColor scaleColor READ getScaleColor WRITE setScaleColor)//设置刻度尺颜色
    Q_PROPERTY(QColor pointerColor READ getPointerColor WRITE setPointerColor)//设置指针颜色
    Q_PROPERTY(QColor centerCircleColor READ getCenterCircleColor WRITE setCenterCircleColor)//设置中心圆样式
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)//设置文本颜色

    Q_PROPERTY(bool showOverlay READ getShowOverlay WRITE setShowOverlay)//是否显示遮罩层
    Q_PROPERTY(QColor overlayColor READ getOverlayColor WRITE setOverlayColor)//遮罩层颜色

    Q_PROPERTY(PieStyle pieStyle READ getPieStyle WRITE setPieStyle)//饼图样式
    Q_PROPERTY(PointerStyle pointerStyle READ getPointerStyle WRITE setPointerStyle)// 指针样式


public:
    enum PointerStyle {
        PointerStyle_Circle = 0,
        PointerStyle_Indicator,
        PointerStyle_IndicatorR,
        PointerStyle_Triangle
    };
    enum PieStyle {
        PieStyle_Three = 0,
        PieStyle_Current,
    };
    explicit GaugeCar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);
    void drawOuterCircle(QPainter *painter);
    void drawInnerCircle(QPainter *painter);
    void drawCoverCircle(QPainter *painter);
    void drawPie(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawPointerCircle(QPainter *painter);
    void drawPointerIndicator(QPainter *painter);
    void drawPointerIndicatorR(QPainter *painter);
    void drawPointerTriangle(QPainter *painter);
    void drawRoundCircle(QPainter *painter);
    void drawCenterCircle(QPainter *painter);
    void drawValue(QPainter *painter);
    void drawOverlay(QPainter *painter);

private slots:
    void updataValue();

Q_SIGNALS:
    void valueChanged();

private:
    double minValue;
    double maxValue;
    double value;

    int precision;

    int scaleMajor;
    int scaleMinor;
    int startAngle;
    int endAngle;

    bool animation;
    double animationStep;


    QColor outerCircleColor;


    QColor innerCircleColor;
    QColor coverCircleColor;

    QColor pieColorStart;

    QColor pieColorMid;
    QColor pieColorEnd;

    QColor scaleColor;
    QColor pointerColor;
    QColor centerCircleColor;
    QColor textColor;

    bool showOverlay;
    QColor overlayColor;

    PieStyle pieStyle;
    PointerStyle pointerStyle;

    bool reverse;
    double currentValue;
    QTimer *timer;

public:
    double getMinValue()const;
    double getMaxValue()const;
    double getValue()const;

    int getPrecision()const;

    int getScaleMajor()const;
    int getScaleMinor()const;

    int getSartAngle()const;
    int geEndAngle()const;

    bool getAnimation()const;
    double getAnimationStep()const;

    QColor getOuterCircleColor()const;
    QColor getInnerCircleColor()const;
    QColor getCoverCircleColor()const;

    QColor getPieColorStart()const;
    QColor getPieColorMid()const;
    QColor getPieColorEnd()const;

    QColor getScaleColor()const;
    QColor getPointerColor()const;
    QColor getCenterCircleColor()const;
    QColor getTextColor()const;

    bool getShowOverlay();
    QColor getOverlayColor();

    PieStyle getPieStyle();
    PointerStyle getPointerStyle();

public Q_SLOTS:
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);
    void setValue(double value);
    void setValue(int value);
    void setPrecision(int precision);

    void setScaleMajor(int scaleMajor);
    void setScaleMinor(int scaleMinor);
    void setSartAngle(int startAngle);
    void setEndAngle(int endAgle);

    void setAnimation(bool animation);
    void setAnimationStep(double animationStep);

    void setOuterCircleColor(const QColor& outerCircleColor);
    void setInnerCircleColor(const QColor&  innerCircleColor);
    void setCoverCircleColor(const QColor&  coverCircleColor);

    void setPieColorStart(const QColor&  pieColorStart);
    void setPieColorMid(const QColor&  pieColorMid);
    void setPieColorEnd(const QColor&  pieColorEnd);

    void setScaleColor(const QColor& scaleColor);
    void setPointerColor(const QColor& pointerColor);
    void setCenterCircleColor(const QColor& centerCircleColor);
    void setTextColor(const QColor& textColor);

    void setShowOverlay(bool showOverlay);
    void setOverlayColor(const QColor& overlayColor);

    void setPieStyle(const PieStyle& pieStyle);
    void setPointerStyle(const PointerStyle& pointerStyle);
};

#endif // GAUGECAR_H
