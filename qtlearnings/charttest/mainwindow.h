#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//examples: charts/callout
#include <functional>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QValueAxis;
class QSplineSeries;
QT_CHARTS_END_NAMESPACE

class CCallout;
typedef std::function<QString (int)> funcint;

QT_CHARTS_USE_NAMESPACE

class CCurveView: public QGraphicsView
{
    Q_OBJECT

public:
    CCurveView(funcint funcGetDesc, QWidget *parent = 0);

    void setTitle(const QString& strTitle);
    void setAxisxTitle(const QString& strTitle);
    void setAxisyTitle(const QString& strTitle);
    void setAxisxRange(const double dMin, const double dMax);
    void setAxisyRange(const double dMin, const double dMax);
    void addSeries(const QList<QPointF> *pListPoint);
    void updateSeries(const QList<QPointF> *pListPoint);
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);

private:
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QSplineSeries *m_series;
    funcint        m_funcGetDesc;
    double  m_dXMin;
    double  m_dXMax;
    double  m_dYMin;
    double  m_dYMax;
    CCallout *m_tooltip;
    //QList<Callout *> m_callouts;

    int getClosestValue(double dVal);
};

class CCallout : public QGraphicsItem
{
public:
    CCallout(QChart *parent);

    void setText(const QString &text);
    void setAnchor(QPointF point);
    void updateGeometry();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_text;
    QRectF m_textRect;
    QRectF m_rect;
    QPointF m_anchor;
    QFont m_font;
    QChart *m_chart;
};

#endif // MAINWINDOW_H
