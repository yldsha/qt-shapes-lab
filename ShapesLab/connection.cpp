#include "connection.h"
#include "shape.h"

#include <QPen>

Connection::Connection(Shape *from, Shape *to)
    : m_from(from), m_to(to)
{
    setPen(QPen(Qt::darkGray, 2));   // серая линия толщиной 2
    setZValue(-1);                   // линия рисуется под фигурами
    updatePosition();                // сразу поставить концы на место
}

void Connection::updatePosition()
{
    // отрезок от центра первой фигуры до центра второй (в координатах сцены)
    setLine(QLineF(m_from->sceneCenter(), m_to->sceneCenter()));
}