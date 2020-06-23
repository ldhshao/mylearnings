#include "mainwindow.h"
//#include "callout.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtGui/QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <QDebug>

CCurveView::CCurveView(funcint funcGetDesc, QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_chart(0),
      m_tooltip(0),
      m_funcGetDesc(funcGetDesc)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // chart
    m_chart = new QChart();
    m_chart->setMinimumSize(640, 480);
    m_chart->legend()->hide();
    //m_chart->createDefaultAxes();
    m_chart->setAcceptHoverEvents(true);

    //
    m_series = new QSplineSeries();
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_chart->addSeries(m_series);
    m_chart->setAxisX(m_axisX, m_series);
    m_chart->setAxisY(m_axisY, m_series);
    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    connect(m_series, &QLineSeries::clicked, this, &CCurveView::keepCallout);
    connect(m_series, &QLineSeries::hovered, this, &CCurveView::tooltip);

    this->setMouseTracking(true);
}

void CCurveView::setTitle(const QString& strTitle)
{
    m_chart->setTitle(strTitle);
}
void CCurveView::setAxisxTitle(const QString& strTitle)
{
    m_axisX->setTitleText(strTitle);
}
void CCurveView::setAxisyTitle(const QString& strTitle)
{
    m_axisY->setTitleText(strTitle);
}
void CCurveView::setAxisxRange(const double dMin, const double dMax)
{
    m_dXMin = dMin; m_dXMax = dMax;
    m_axisX->setRange(dMin, dMax);
}
void CCurveView::setAxisyRange(const double dMin, const double dMax)
{
    m_dYMin = dMin; m_dYMax = dMax;
    m_axisY->setRange(dMin, dMax);
}
void CCurveView::addSeries(const QList<QPointF> *pListPoint)
{
    for (QList<QPointF>::const_iterator it = pListPoint->begin(); it != pListPoint->end(); it++)
    {
        m_series->append(it->x(), it->y());
    }
}
void CCurveView::updateSeries(const QList<QPointF> *pListPoint)
{
    m_series->replace(*pListPoint);
}
void CCurveView::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        m_chart->resize(event->size());
    }
    QGraphicsView::resizeEvent(event);
}

void CCurveView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void CCurveView::mousePressEvent(QMouseEvent *event)
{
    QList<QPointF> lstPoint;
    static int iIdx = 0;
    iIdx += 70;
    lstPoint.append(QPointF(iIdx + 0, 2));
    lstPoint.append(QPointF(iIdx + 10, 3));
    lstPoint.append(QPointF(iIdx + 20, 2.5));
    lstPoint.append(QPointF(iIdx + 30, 2.8));
    lstPoint.append(QPointF(iIdx + 40, 2.5));
    lstPoint.append(QPointF(iIdx + 50, 3));
    lstPoint.append(QPointF(iIdx + 60, 2));
    lstPoint.append(QPointF(iIdx + 70, 1));
    if (iIdx + 70 > m_dXMax)
        setAxisxRange(m_dXMin, iIdx + 70 + 10);
    addSeries(&lstPoint);
    QGraphicsView::mousePressEvent(event);
}

void CCurveView::keepCallout()
{
    //m_callouts.append(m_tooltip);
    //m_tooltip = new Callout(m_chart);
}

void CCurveView::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new CCallout(m_chart);

    if (state) {
        int iIdx = getClosestValue(point.x());
        if (0 <= iIdx)
        {
            QString strInfo = m_funcGetDesc(iIdx);
            m_tooltip->setText(strInfo);
            m_tooltip->setAnchor(point);
            m_tooltip->setZValue(11);
            m_tooltip->updateGeometry();
            m_tooltip->show();
        }
    } else {
        m_tooltip->hide();
    }
}

int CCurveView::getClosestValue(double dVal)
{
    int iVal = dVal;
    if (fabs(dVal - iVal) <= 0.2)
        return iVal;
    iVal++;
    if (fabs(dVal - iVal) <= 0.2)
        return iVal;

    return -1;
}

///CCalout/////////////////////////////////////
CCallout::CCallout(QChart *chart):
    QGraphicsItem(chart),
    m_chart(chart)
{
}

QRectF CCallout::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect;
    rect.setLeft(qMin(m_rect.left(), anchor.x()));
    rect.setRight(qMax(m_rect.right(), anchor.x()));
    rect.setTop(qMin(m_rect.top(), anchor.y()));
    rect.setBottom(qMax(m_rect.bottom(), anchor.y()));
    return rect;
}

void CCallout::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path;
    path.addRoundedRect(m_rect, 5, 5);

    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    if (!m_rect.contains(anchor)) {
        QPointF point1, point2;

        // establish the position of the anchor point in relation to m_rect
        bool above = anchor.y() <= m_rect.top();
        bool aboveCenter = anchor.y() > m_rect.top() && anchor.y() <= m_rect.center().y();
        bool belowCenter = anchor.y() > m_rect.center().y() && anchor.y() <= m_rect.bottom();
        bool below = anchor.y() > m_rect.bottom();

        bool onLeft = anchor.x() <= m_rect.left();
        bool leftOfCenter = anchor.x() > m_rect.left() && anchor.x() <= m_rect.center().x();
        bool rightOfCenter = anchor.x() > m_rect.center().x() && anchor.x() <= m_rect.right();
        bool onRight = anchor.x() > m_rect.right();

        // get the nearest m_rect corner.
        qreal x = (onRight + rightOfCenter) * m_rect.width();
        qreal y = (below + belowCenter) * m_rect.height();
        bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
        bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

        qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
        qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);;
        point1.setX(x1);
        point1.setY(y1);

        qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);;
        qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);;
        point2.setX(x2);
        point2.setY(y2);

        path.moveTo(point1);
        path.lineTo(anchor);
        path.lineTo(point2);
        path = path.simplified();
    }
    painter->setBrush(QColor(255, 255, 255));
    painter->drawPath(path);
    painter->drawText(m_textRect, m_text);
}

void CCallout::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->setAccepted(true);
}

void CCallout::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        setPos(mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)));
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

void CCallout::setText(const QString &text)
{
    m_text = text;
    QFontMetrics metrics(m_font);
    m_textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
    m_textRect.translate(5, 5);
    prepareGeometryChange();
    m_rect = m_textRect.adjusted(-5, -5, 5, 5);

    //move when outof right
    QPointF ptTmp = m_chart->mapToPosition(m_anchor);
    QRectF  rctTmp = m_chart->rect();
    int iOffset = ptTmp.x() + m_rect.width() - rctTmp.width();
    if (iOffset > 0)
    {
        ptTmp.setX(ptTmp.x() - iOffset - 20);
        setPos(ptTmp);
    }
}

void CCallout::setAnchor(QPointF point)
{
    m_anchor = point;
}

void CCallout::updateGeometry()
{
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_anchor) + QPoint(10, -50));
}
