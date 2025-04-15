#ifndef NODEITEM_H
#define NODEITEM_H
#include <QGraphicsObject>
#include <QString>
#include <opencv2/opencv.hpp>
#include <QPixmap>
#include "Node.h"
class NodeEdge;
class NodeItem : public QGraphicsObject {
    Q_OBJECT
public:
    NodeItem(const QString &title, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setPreview(const cv::Mat &image);
    void setTitle(const QString &title);
    QString getTitle() const;
    void setNode(Node *node);
    Node* getNode() const;
    QPointF getInputPort() const;
    QPointF getOutputPort() const;
signals:
    void connectionCreated(const QString &sourceId, const QString &targetId);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QString m_title;
    QPixmap m_preview;
    bool m_hasPreview;
    Node *m_node;
    bool m_draggingEdge;
    NodeEdge *m_tempEdge;
};
#endif
