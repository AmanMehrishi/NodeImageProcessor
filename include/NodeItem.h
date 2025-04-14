#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsItem>
#include <QString>
#include <opencv2/opencv.hpp>
#include <QPixmap>

class NodeItem : public QGraphicsItem {
public:
    NodeItem(const QString &title, QGraphicsItem *parent = nullptr);
    
    void setPreview(const cv::Mat &image);
    void setTitle(const QString &title);
    QString getTitle() const { return m_title; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QString m_title;
    QPixmap m_preview;
    bool m_hasPreview;
};

#endif 
