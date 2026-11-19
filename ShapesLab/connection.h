#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsLineItem>

class Shape;   
//Отрезок, который помнит свои две фигуры и умеет пересчитывать концы при пермещении фигур
class Connection : public QGraphicsLineItem //берем базовый отрезок и добавляем свое поведение
{
public:
    Connection(Shape *from, Shape *to);

    Shape* getFrom() const { return m_from; }
    Shape *getTo()   const { return m_to; }

    void updatePosition();  // пересчитай концы по текущим центрам фигур 

private:
    Shape *m_from;
    Shape *m_to;
};

#endif // CONNECTION_H