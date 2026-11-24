#include "shapedatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool ShapeDatabase::open(const QString &fileName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");   // драйвер SQLite
    db.setDatabaseName(fileName);                             // файл базы

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");   
    createTables();

    query.exec("DELETE FROM shapes");          // новый сеанс: холст пуст, база тоже
    return true;
}

void ShapeDatabase::createTables()
{
    QSqlQuery query;

    // 1. Фигуры
    query.exec("CREATE TABLE IF NOT EXISTS shapes ("
               "id INTEGER PRIMARY KEY, "
               "type TEXT NOT NULL, "
               "total_count INTEGER DEFAULT 0)");

    // 2. Связи
    query.exec("CREATE TABLE IF NOT EXISTS connections ("
               "from_id INTEGER NOT NULL REFERENCES shapes(id) ON DELETE CASCADE, "
               "to_id   INTEGER NOT NULL REFERENCES shapes(id) ON DELETE CASCADE)");

    // 3. Представление для показа
    query.exec("CREATE VIEW IF NOT EXISTS shapes_view AS "
               "SELECT s.id, s.type, "
               "  (SELECT GROUP_CONCAT(CASE WHEN c.from_id = s.id "
               "                            THEN c.to_id ELSE c.from_id END) "
               "   FROM connections c "
               "   WHERE c.from_id = s.id OR c.to_id = s.id) AS connections, "
               "  s.total_count "
               "FROM shapes s");
}

void ShapeDatabase::insertShape(int id, const QString &type)
{
    QSqlQuery query;
    query.prepare("INSERT INTO shapes (id, type) VALUES (:id, :type)");
    query.bindValue(":id", id);
    query.bindValue(":type", type);
    if (!query.exec())
        qDebug() << "Ошибка вставки фигуры:" << query.lastError().text();
}

void ShapeDatabase::insertConnection(int fromId, int toId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO connections (from_id, to_id) VALUES (:from, :to)");
    query.bindValue(":from", fromId);
    query.bindValue(":to", toId);
    if (!query.exec())
        qDebug() << "Ошибка вставки связи:" << query.lastError().text();
}

void ShapeDatabase::updateTotalCount(const QString &type)
{
    QSqlQuery query;
    query.prepare("UPDATE shapes SET total_count = "
                  "(SELECT COUNT(*) FROM shapes WHERE type = :type) "
                  "WHERE type = :type");
    query.bindValue(":type", type);
    query.exec();
}
void ShapeDatabase::deleteShape(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM shapes WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
        qDebug() << "Ошибка удаления фигуры:" << query.lastError().text();
}