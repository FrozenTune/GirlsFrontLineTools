#ifndef GIRLSFRONTLINETOOLS_H
#define GIRLSFRONTLINETOOLS_H

#include <QWidget>

namespace Ui {
class GirlsFrontLineTools;
}

class GirlsFrontLineTools : public QWidget
{
    Q_OBJECT

public:
    explicit GirlsFrontLineTools(QWidget *parent = nullptr);
    ~GirlsFrontLineTools();

private:
    Ui::GirlsFrontLineTools *ui;
};

#endif // GIRLSFRONTLINETOOLS_H
