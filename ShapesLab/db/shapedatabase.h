#ifndef SHAPEDATABASE_H
#define SHAPEDATABASE_H

#include <QString>

class ShapeDatabase
{
public:
    bool open(const QString &fileName);            // открыть файл базы и подготовить таблицы

    void insertShape(int id, const QString &type);  // добавить фигуру
    void insertConnection(int fromId, int toId);    // добавить связь
    void updateTotalCount(const QString &type);     // пересчитать количество фигур этого типа

private:
    void createTables();                            // создать таблицы и представление
};

#endif // SHAPEDATABASE_H