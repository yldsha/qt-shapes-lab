#include "shape.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>

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
    const double cx = m_width / 2;    // центр фигуры
    const double cy = m_height / 2;
    const double rx = m_width / 2;    // отступ от центра по горизонтали
    const double ry = m_height / 2;   // отступ от центра по вертикали

    QPolygonF polygon;
    for (int i = 0; i < m_vertices; ++i) {
        // угол вершины: шаг 2π/n, первая вершина сверху (-π/2)
        const double angle = -M_PI / 2 + 2 * M_PI * i / m_vertices;

        const double x = cx + rx * qCos(angle);
        const double y = cy + ry * qSin(angle);

        polygon << QPointF(x, y);
    }

    painter->drawPolygon(polygon);
}

PolygonShape::PolygonShape(int id, double width, double height, int vertices)
: Shape(id, width, height), m_vertices(vertices)
{
}