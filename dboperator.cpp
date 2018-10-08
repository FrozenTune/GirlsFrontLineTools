#include "dboperator.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDebug>

#define FILE_PATH "./_GF.db"
#define DB_USERNAME "AX"
#define DB_PASSWORD "qwert74123"


DBOperator::DBOperator()
{
    init();
}

void DBOperator::init()
{
    QFile dbFile(FILE_PATH);
    if(!dbFile.open(QIODevice::Append))
    {
        qDebug()<<__LINE__<<"OPEN_FAILED";
    }

    dbFile.close();

    gf_database = QSqlDatabase::addDatabase("QSQLITE");
    gf_database.setDatabaseName(FILE_PATH);
    gf_database.setUserName(DB_USERNAME);
    gf_database.setPassword(DB_PASSWORD);


    bool ok = gf_database.open();
    qDebug()<<__LINE__<<"OPEN"<<ok;

    QSqlQuery query;

    bool isTableExist = query.exec("select * from LV_EXP");
    if(!isTableExist)
    {
        query.exec("create table LV_EXP(LV Integer,EXP Integer)");
        qDebug()<<__LINE__<<query.lastError();
    }
    else
        qDebug()<<__LINE__<<"LV_EXP exist";

    isTableExist = query.exec("select * from GUN_CONSUME");
    if(!isTableExist)
    {
        query.exec("create table GUN_CONSUME(TYPE varchar(4),"
                   "Ammo_Consume Integer, Ammo_Increment Interger,"
                   "Ration_Consume Integer, Ration_Increment Integer)");
        qDebug()<<__LINE__<<query.lastError();

    }
    else
        qDebug()<<__LINE__<<"GUN_CONSUME exist";

    isTableExist = query.exec("select * from MAP_DATA");
    if(!isTableExist)
    {
        query.exec("create table MAP_DATA(MAP_NAME varchar(6),"
                   "BaseExp Integer, Exp_weak_LV Interger)");
        qDebug()<<__LINE__<<query.lastError();
    }
    else
        qDebug()<<__LINE__<<"MAP_DATA exist";

}
