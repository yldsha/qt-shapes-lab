#include "countdelegate.h"

#include <QPainter>
#include <QtMath>
#include <QApplication>
#include <QStyle>

CountDelegate::CountDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

int CountDelegate::iconCount(int count) const
{
    if (count >= 1 && count <= 3)
        return 1;
    if (count >= 4 && count <= 10)
        return 2;
    if (count > 10)
        return 3;
    else
        return 0;
}

void CountDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.text.clear();
    const QWidget *widget = opt.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

    const int count    = index.data().toInt();
    const QString type = index.sibling(index.row(), 1).data().toString();
    const int icons    = iconCount(count);

    // 3. Нарисовать icons фигурок в ряд по центру ячейки
    const QRect cell = option.rect;
    const int size = cell.height() - 8;
    const int gap  = 4;
    const int totalWidth = icons * size + (icons - 1) * gap;
    int x = cell.left() + (cell.width() - totalWidth) / 2;
    const int y = cell.top() + (cell.height() - size) / 2;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::black, 1));
    for (int i = 0; i < icons; ++i) {
        drawIcon(painter, QRectF(x, y, size, size), type);
        x += size + gap;
    }
    painter->restore();
}

void CountDelegate::drawIcon(QPainter *painter, const QRectF &rect, const QString &type) const
{
    if (type == "Прямоугольник") {
        painter->setBrush(QColor(120, 200, 255));
        painter->drawRect(rect.adjusted(0, rect.height() / 6, 0, -rect.height() / 6));
    } else if (type == "Эллипс") {
        painter->setBrush(QColor(255, 220, 100));
        painter->drawEllipse(rect.adjusted(0, rect.height() / 6, 0, -rect.height() / 6));
    } else {
        // многоугольник — маленький пятиугольник
        painter->setBrush(QColor(230, 140, 230));
        QPolygonF polygon;
        const QPointF c = rect.center();
        const double r = rect.width() / 2;
        for (int i = 0; i < 5; ++i) {
            const double angle = -M_PI / 2 + 2 * M_PI * i / 5;
            polygon << QPointF(c.x() + r * qCos(angle), c.y() + r * qSin(angle));
        }
        painter->drawPolygon(polygon);
    }
}