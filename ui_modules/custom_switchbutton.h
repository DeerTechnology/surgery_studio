#ifndef CUSTOM_SWITCHBUTTON_H
#define CUSTOM_SWITCHBUTTON_H

#include <QWidget>
#include <QTimer>
#include <QColor>









class Custom_SwitchButton : public QWidget
{
    Q_OBJECT
//    Q_PROPERTY(int m_space READ space WRITE setSpace)
//    Q_PROPERTY(int m_radius READ radius WRITE setRadius)
//    Q_PROPERTY(bool m_checked READ checked WRITE setChecked)
//    Q_PROPERTY(bool m_showText READ showText WRITE setShowText)
//    Q_PROPERTY(bool m_showCircle READ showCircel WRITE setShowCircle)
//    Q_PROPERTY(bool m_animation READ animation WRITE setAnimation)

//    Q_PROPERTY(QColor m_bgColorOn READ bgColorOn WRITE setBgColorOn)
//    Q_PROPERTY(QColor m_bgColorOff READ bgColorOff WRITE setBgColorOff)
//    Q_PROPERTY(QColor m_sliderColorOn READ sliderColorOn WRITE setSliderColorOn)
//    Q_PROPERTY(QColor m_sliderColorOff READ sliderColorOff WRITE setSliderColorOff)
//    Q_PROPERTY(QColor m_textColor READ textColor WRITE setTextColor)

//    Q_PROPERTY(QString m_textOn READ textOn WRITE setTextOn)
//    Q_PROPERTY(QString m_textOff READ textOff WRITE setTextOff)

//    Q_PROPERTY(int m_step READ step WRITE setStep)
//    Q_PROPERTY(int m_startX READ startX WRITE setStartX)
//    Q_PROPERTY(int m_endX READ endX WRITE setEndX)

public:
    explicit Custom_SwitchButton(QWidget *parent = 0);
    ~Custom_SwitchButton(){}
    bool status = false;

signals:
    void statusChanged(bool checked);

public slots:

private slots:
    void updateValue();

private:
    void drawBackGround(QPainter *painter);
    void drawSlider(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    int m_space;                //滑块距离边界距离
    int m_radius;               //圆角角度

    bool m_checked;             //是否选中
    bool m_showText;            //是否显示文字
    bool m_showCircle;          //是否显示圆圈
    bool m_animation;           //是否使用动画

    QColor m_bgColorOn;         //打开时候的背景色
    QColor m_bgColorOff;        //关闭时候的背景色
    QColor m_sliderColorOn;     //打开时候滑块颜色
    QColor m_sliderColorOff;    //关闭时候滑块颜色
    QColor m_textColor;         //文字颜色

    QString m_textOn;           //打开时候的文字
    QString m_textOff;          //关闭时候的文字

    QTimer  *m_timer;            //动画定时器
    int     m_step;             //动画步长
    int     m_startX;           //滑块开始X轴坐标
    int     m_endX;             //滑块结束X轴坐标

public:
    int space()                 const;
    int radius()                const;
    bool checked()              const;
    bool showText()             const;
    bool showCircel()           const;
    bool animation()            const;

    QColor bgColorOn()          const;
    QColor bgColorOff()         const;
    QColor sliderColorOn()      const;
    QColor sliderColorOff()     const;
    QColor textColor()          const;

    QString textOn()            const;
    QString textOff()           const;

    int step()                  const;
    int startX()                const;
    int endX()                  const;


public Q_SLOTS:
    void setSpace(int space);
    void setRadius(int radius);
    void setChecked(bool checked);
    void setShowText(bool show);
    void setShowCircle(bool show);
    void setAnimation(bool ok);

    void setBgColorOn(const QColor &color);
    void setBgColorOff(const QColor &color);
    void setSliderColorOn(const QColor &color);
    void setSliderColorOff(const QColor &color);
    void setTextColor(const QColor &color);

    void setTextOn(const QString &text);
    void setTextOff(const QString &text);

//    void setStep(int step);
//    void setStartX(int startX);
//    void setEndX(int endX);


};




















/*
 * old
 */
class Custom_SwitchButton_old : public QWidget
{
    Q_OBJECT

public:
    explicit Custom_SwitchButton_old(QWidget *parent = nullptr);
    ~Custom_SwitchButton_old();
    /**
     * @brief SetSize 设置按钮的尺寸
     * @param nWidth 按钮的新宽度
     * @param nHeight 按钮的新高度
     */
    void SetSize(int nWidth, int nHeight);

    /**
     * @brief SetActiveColor 设置按钮激活时候的颜色
     * @param color 激活颜色
     */
    void SetActiveColor(QColor color);

    /**
     * @brief SetInactiveColor 设置按钮未激活时候的颜色
     * @param color 未激活颜色
     */
    void SetInactiveColor(QColor color);

    /**
     * @brief SetSliderColor 设置滑块颜色
     * @param color 滑块的颜色
     */
    void SetSliderColor(QColor color);

    /**
     * @brief SetStatus 设置按钮状态
     * @param bActive true: 激活，false: 未激活
     */
    void SetStatus(bool bActive);

    /**
     * @brief GetStatus 获取按钮当前状态
     * @return  true: 激活，false: 未激活
     */
    inline bool GetStatus();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void DrawSlider(QPainter* p);

private:
    bool m_bActive; // 是否激活
    float m_fStep; // 每次重绘时候的滑块的步进值
    float m_fCurrent; // 当前滑块的位置（left位置）
    int m_nArcRadius; // 圆弧的半径
    int m_nRectWidth; // 矩形的宽度
    QColor m_colorActive; // 激活时的颜色
    QColor m_colorInactive;
    QColor m_colorSlider;
    QTimer m_timer;
private slots:
    void SwitchAnimation(); // 切换动画
};
#endif // CUSTOM_SWITCHBUTTON_H
