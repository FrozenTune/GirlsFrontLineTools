#include "gf_query.h"
#include "ui_gf_query.h"

GF_QUERY::GF_QUERY(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GF_QUERY)
{
    ui->setupUi(this);

    dboperator = new DBOperator();


    gf_calc = new GF_Calc();
    connect(gf_calc,SIGNAL(comsume_ok(RESOURCE_CONSUME)),
            this,SLOT(ShowConsume(RESOURCE_CONSUME)));


    gf_calc->init();

    //For Test
    gf_calc->Calc_Times(0);
    gf_calc->Calc_Consume();

//    exit(0);
}

GF_QUERY::~GF_QUERY()
{
    delete ui;
}

void GF_QUERY::ShowConsume(RESOURCE_CONSUME consume)
{
    ui->ammo_label->setText(QString::number(consume.Ammo));
    ui->ration_label->setText(QString::number(consume.Ration));
}


