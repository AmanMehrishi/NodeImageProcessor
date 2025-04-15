#include "NodeItem.h"
#include "NodeEdge.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <opencv2/imgproc.hpp>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <iostream>
NodeItem::NodeItem(const QString &title, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_title(title),
      m_hasPreview(false),
      m_node(nullptr),
      m_draggingEdge(false),
      m_tempEdge(nullptr)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}
QRectF NodeItem::boundingRect() const { return QRectF(0, 0, 150, 100); }
void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    painter->setBrush(Qt::lightGray);
    painter->drawRoundedRect(boundingRect(), 10, 10);
    if(option->state & QStyle::State_Selected) {
        QPen pen(Qt::yellow, 3);
        painter->setPen(pen);
        painter->drawRoundedRect(boundingRect(), 10, 10);
    } else {
        painter->setPen(Qt::black);
    }
    painter->drawText(QRectF(0, 0, 150, 20), Qt::AlignCenter, m_title);
    if(m_hasPreview)
        painter->drawPixmap(QRectF(10, 30, 130, 60), m_preview, m_preview.rect());
}
void NodeItem::setPreview(const cv::Mat &image) {
    try {
        if(image.empty()){
            m_hasPreview = false;
            return;
        }
        cv::Mat rgb;
        if(image.channels() == 3)
            cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);
        else if(image.channels() == 1)
            cv::cvtColor(image, rgb, cv::COLOR_GRAY2RGB);
        else
            rgb = image;
        if(!rgb.isContinuous())
            rgb = rgb.clone();
        QImage qimg(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888);
        QImage qimgCopy = qimg.copy();
        m_preview = QPixmap::fromImage(qimgCopy.scaled(130, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_hasPreview = true;
    } catch(const std::exception &ex) { std::cerr << "setPreview exception: " << ex.what() << std::endl; m_hasPreview = false; }
      catch(...) { std::cerr << "setPreview unknown exception." << std::endl; m_hasPreview = false; }
    update();
}
void NodeItem::setTitle(const QString &title) { m_title = title; update(); }
QString NodeItem::getTitle() const { return m_title; }
void NodeItem::setNode(Node *node) { m_node = node; }
Node* NodeItem::getNode() const { return m_node; }
QPointF NodeItem::getInputPort() const { return mapToScene(QPointF(0, boundingRect().height()/2)); }
QPointF NodeItem::getOutputPort() const { return mapToScene(QPointF(boundingRect().width(), boundingRect().height()/2)); }
void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QRectF outputPortRect(boundingRect().right()-40, boundingRect().top()+20, 40, 60);
    if(event->button() == Qt::LeftButton && outputPortRect.contains(event->pos())) {
        m_draggingEdge = true;
        m_tempEdge = new NodeEdge(this, nullptr);
        m_tempEdge->setTemporaryTarget(mapToScene(event->pos()));
        scene()->addItem(m_tempEdge);
        event->accept();
        return;
    }
    QGraphicsObject::mousePressEvent(event);
}
void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(m_draggingEdge && m_tempEdge) {
        m_tempEdge->setTemporaryTarget(mapToScene(event->pos()));
        m_tempEdge->update();
        event->accept();
        return;
    }
    QGraphicsObject::mouseMoveEvent(event);
}
void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_draggingEdge && m_tempEdge) {
        QList<QGraphicsItem*> itemsUnder = scene()->items(mapToScene(event->pos()));
        NodeItem* targetNode = nullptr;
        for (QGraphicsItem* it : itemsUnder) {
            if (it == this) continue;
            if (auto ni = dynamic_cast<NodeItem*>(it)) { targetNode = ni; break; }
        }
        QString sourceId = getTitle();
        QString targetId;
        if (targetNode && targetNode != this) {
            QPointF inputPort = targetNode->getInputPort();
            if (QLineF(mapToScene(event->pos()), inputPort).length() < 50) {
                targetId = targetNode->getTitle();
            }
        }
        scene()->removeItem(m_tempEdge);
        delete m_tempEdge;
        m_tempEdge = nullptr;
        m_draggingEdge = false;
        event->accept();
        if (!targetId.isEmpty())
            emit connectionCreated(sourceId, targetId);
        return;
    }
    QGraphicsObject::mouseReleaseEvent(event);
}