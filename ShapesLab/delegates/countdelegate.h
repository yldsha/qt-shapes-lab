#ifndef COUNTDELEGATE_H
#define COUNTDELEGATE_H

#include <QStyledItemDelegate>

class CountDelegate : public QStyledItemDelegate
{
public:
    explicit CountDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private:
    int  iconCount(int count) const;   
    void drawIcon(QPainter *painter, const QRectF &rect, const QString &type) const;
};

#endif // COUNTDELEGATE_H