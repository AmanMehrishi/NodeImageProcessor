#include "NodeItem.h"
#include <QPainter>
#include <QImage>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <opencv2/imgproc.hpp>
#include <iostream>

NodeItem::NodeItem(const QString &title, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_title(title), m_hasPreview(false), m_node(nullptr)
{
    setFlags(QGraphicsItem::ItemIsMovable |
        QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF NodeItem::boundingRect() const {
    return QRectF(0, 0, 150, 100);
}

void NodeItem::setTitle(const QString &title) {
    m_title = title;
    update();
}

void NodeItem::setPreview(const cv::Mat &image) {
    try {
        if (image.empty()) {
            m_hasPreview = false;
            return;
        }
        
        cv::Mat rgb;
        if (image.channels() == 3)
            cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);
        else if (image.channels() == 1)
            cv::cvtColor(image, rgb, cv::COLOR_GRAY2RGB);
        else
            rgb = image;
    
        if (!rgb.isContinuous())
            rgb = rgb.clone();
    
        QImage qimg(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888);
        QImage qimgCopy = qimg.copy(); 
        m_preview = QPixmap::fromImage(qimgCopy.scaled(130, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        m_hasPreview = true;
        
    } catch (const std::exception &ex) {
        std::cerr << "NodeItem::setPreview caught exception: " << ex.what() << std::endl;
        m_hasPreview = false;
    } catch (...) {
        std::cerr << "NodeItem::setPreview caught an unknown exception." << std::endl;
        m_hasPreview = false;
    }
    update();
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);

    painter->setBrush(Qt::lightGray);
    painter->drawRoundedRect(boundingRect(), 10, 10);

    if (option->state & QStyle::State_Selected) {
        QPen pen(Qt::yellow, 3);
        painter->setPen(pen);
        painter->drawRoundedRect(boundingRect(), 10, 10);
    } else {
        painter->setPen(Qt::black);
    }
    painter->drawText(QRectF(0, 0, 150, 20), Qt::AlignCenter, m_title);

    if (m_hasPreview) {
        painter->drawPixmap(QRectF(10, 30, 130, 60), m_preview, m_preview.rect());
    }
}

