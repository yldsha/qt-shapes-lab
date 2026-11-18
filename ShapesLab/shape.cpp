#include "shape.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

Shape::Shape(int id, double width, double height)
    : m_id(id), m_width(width), m_height(height)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

QRectF Shape::boundingRect() const
{
    return shapeRect().adjusted(-2, -2, 2, 2);
}

void Shape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::black, 2);
    if (option->state & QStyle::State_Selected) {   
        pen.setColor(Qt::blue);
        pen.setStyle(Qt::DashLine);
    }
    painter->setPen(pen);
    painter->setBrush(fillColor());

    drawShape(painter);   

    painter->drawText(shapeRect(), Qt::AlignCenter, QString::number(m_id));
}

void RectShape::drawShape(QPainter *painter)
{
    painter->drawRect(shapeRect());
}

void EllipseShape::drawShape(QPainter *painter)
{
    painter->drawEllipse(shapeRect());
}

void PolygonShape::drawShape(QPainter *painter)
{
    QPolygonF triangle;
    triangle << QPointF(m_width / 2, 0)
             << QPointF(m_width, m_height)
             << QPointF(0, m_height);
    painter->drawPolygon(triangle);
}