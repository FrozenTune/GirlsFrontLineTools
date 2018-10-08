#ifndef DBOPERATOR_H
#define DBOPERATOR_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>


class DBOperator
{
public:
    DBOperator();

private:
    QSqlDatabase gf_database;

    void init();
};

#endif // DBOPERATOR_H
