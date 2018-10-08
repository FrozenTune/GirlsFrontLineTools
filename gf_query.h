#ifndef GF_QUERY_H
#define GF_QUERY_H

#include <QWidget>

#include <QDebug>
#include <QList>

#include "gf_calc.h"
#include "dboperator.h"

namespace Ui {
class GF_QUERY;
}

class GF_QUERY : public QWidget
{
    Q_OBJECT

public:
    explicit GF_QUERY(QWidget *parent = 0);
    ~GF_QUERY();

private:
    Ui::GF_QUERY *ui;

    DBOperator *dboperator;
    GF_Calc *gf_calc;

    RESOURCE_CONSUME consume;

public slots:
    void ShowConsume(RESOURCE_CONSUME consume);

};

#endif // GF_QUERY_H
