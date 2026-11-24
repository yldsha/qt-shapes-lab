#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItem>
#include <QColor>
#include <QString>
#include <QList>

class Connection;

class Shape : public QGraphicsItem
{
public:
    enum class Type { Rectangle, Ellipse, Polygon };

    Shape(int id, double width, double height);
    virtual ~Shape() = default;

    int id() const { return m_id; }
    QPointF sceneCenter() const { return mapToScene(shapeRect().center()); }
    // shapeRect() возвращает прямоугольник, который занимает фигура в своих собственных координатах

    virtual Type    shapeType() const = 0;
    virtual QString typeName()  const = 0;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void addConnection(Connection* connection);
    void removeConnection(Connection* connection);
    const QList<Connection*>& connections() const { return m_connections; }

protected:
    virtual void   drawShape(QPainter *painter) = 0;
    virtual QColor fillColor() const = 0;
    QRectF shapeRect() const { return QRectF(0, 0, m_width, m_height); }
    //QRectF прямоугольник(левая верхняя координата, ширина, высота)

    int    m_id;
    double m_width;
    double m_height;
    QList<Connection*> m_connections;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    //QGraphicsItem - базовый класс для всего, что лежит на сцене, метод GraphicsItemChange enum, список возможных видов изменений
    //Qvariant универсальный класс, в переменную которого можно положит объект практически любого типа
};

class RectShape : public Shape
{
public:
    using Shape::Shape;
    Type    shapeType() const override { return Type::Rectangle; }
    QString typeName()  const override { return "Прямоугольник"; }
protected:
    void   drawShape(QPainter *painter) override;
    QColor fillColor() const override { return QColor(120, 200, 255); }
};

class EllipseShape : public Shape
{
public:
    using Shape::Shape;
    Type    shapeType() const override { return Type::Ellipse; }
    QString typeName()  const override { return "Эллипс"; }
protected:
    void   drawShape(QPainter *painter) override;
    QColor fillColor() const override { return QColor(255, 220, 100); }
};

class PolygonShape : public Shape
{
public:
    PolygonShape(int id, double width, double height, int vertices);
    Type    shapeType() const override { return Type::Polygon; }
    QString typeName()  const override { return "Многоугольник"; }
protected:
    void   drawShape(QPainter *painter) override;
    QColor fillColor() const override { return QColor(230, 140, 230); }
private:
    int m_vertices;
};

#endif // SHAPE_H