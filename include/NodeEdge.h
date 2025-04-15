#ifndef NODEEDGE_H
#define NODEEDGE_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
class NodeEdge : public QGraphicsItem {
public:
    NodeEdge(QGraphicsItem *source, QGraphicsItem *target, QGraphicsItem *parent = nullptr);
    void setTemporaryTarget(const QPointF &pt);
    void updatePosition();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QGraphicsItem *sourceItem;
    QGraphicsItem *targetItem;
    bool temporary;
    QPointF tempTarget;
};
#endif
