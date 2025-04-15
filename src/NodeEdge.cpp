#include "NodeEdge.h"
#include <QPen>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
NodeEdge::NodeEdge(QGraphicsItem *source, QGraphicsItem *target, QGraphicsItem *parent)
  : QGraphicsItem(parent), sourceItem(source), targetItem(target), temporary(false) {
  setZValue(-1);
  updatePosition();
}
void NodeEdge::setTemporaryTarget(const QPointF &pt) {
    temporary = true;
    tempTarget = pt;
    prepareGeometryChange();
}
QRectF NodeEdge::boundingRect() const {
    QPointF start = sourceItem ? sourceItem->scenePos() + sourceItem->boundingRect().center() : QPointF();
    QPointF end = temporary ? tempTarget : (targetItem ? targetItem->scenePos() + targetItem->boundingRect().center() : QPointF());
    return QRectF(start, end).normalized().adjusted(-10, -10, 10, 10);
}
void NodeEdge::updatePosition() { prepareGeometryChange(); }
void NodeEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    if(!sourceItem) return;
    QPointF start = sourceItem->scenePos() + sourceItem->boundingRect().center();
    QPointF end = temporary ? tempTarget : (targetItem ? targetItem->scenePos() + targetItem->boundingRect().center() : start);
    QPainterPath path;
    path.moveTo(start);
    path.lineTo(end);
    QPen pen(Qt::black, 2);
    if(option->state & QStyle::State_Selected) pen.setColor(Qt::yellow);
    painter->setPen(pen);
    painter->drawPath(path);
}
