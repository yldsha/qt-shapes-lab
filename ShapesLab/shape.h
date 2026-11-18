#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItem>
#include <QColor>
#include <QString>

class Shape : public QGraphicsItem
{
public:
    enum class Type { Rectangle, Ellipse, Polygon };

    Shape(int id, double width, double height);
    virtual ~Shape() = default;

    int id() const { return m_id; }

    virtual Type    shapeType() const = 0;
    virtual QString typeName()  const = 0;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected:
    virtual void   drawShape(QPainter *painter) = 0;
    virtual QColor fillColor() const = 0;
    QRectF shapeRect() const { return QRectF(0, 0, m_width, m_height); }

    int    m_id;
    double m_width;
    double m_height;
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