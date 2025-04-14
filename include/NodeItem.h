#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsItem>
#include <QString>
#include <opencv2/opencv.hpp>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include "Node.h"

class NodeItem : public QGraphicsItem {
public:
    NodeItem(const QString &title, QGraphicsItem *parent = nullptr);
    
    void setPreview(const cv::Mat &image);
    void setTitle(const QString &title);

    void setNode(Node *node) { m_node = node; }
    Node* getNode() const { return m_node; }


    QString getTitle() const { return m_title; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QString m_title;
    QPixmap m_preview;
    bool m_hasPreview;
    Node* m_node;
};

#endif 
