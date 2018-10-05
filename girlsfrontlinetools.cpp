#include "girlsfrontlinetools.h"
#include "ui_girlsfrontlinetools.h"

GirlsFrontLineTools::GirlsFrontLineTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GirlsFrontLineTools)
{
    ui->setupUi(this);
}

GirlsFrontLineTools::~GirlsFrontLineTools()
{
    delete ui;
}
