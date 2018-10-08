#ifndef GF_CALC_H
#define GF_CALC_H

#include <QObject>

#include "dboperator.h"

enum GUN_TYPE{
    HG,
    SMG,
    AR,
    RF,
    MG,
    SG
};

enum EXPAND_TYPE{
    X1,
    X2,
    X3,
    X4,
    X5
};

enum CB_TYPE{
    normal,
    saving
};

typedef struct {
    GUN_TYPE Gun_type;
    int HP;
    bool Force_Expansion;
    EXPAND_TYPE Expansion;
    int Cur_EXP;
    bool MVP;
    bool Leader;
    int Start_LV;
    int Target_LV;
    bool Marriage;
    bool MainFrame;
    CB_TYPE cb_type;
}GUN_DATA;

typedef struct {
    int Ammo_consume;
    int Ration_consume;
    int Ammo_increment;
    int Ration_increment;
    void clean()
    {
        Ammo_consume = 0;
        Ration_consume = 0;
        Ammo_increment = 0;
        Ration_increment = 0;
    }
}GUN_CONSUME;



typedef struct {
    int CB_EXP;
    int EXP_weaken_LV;
}MAP_DATA;


typedef struct
{
    QString Map_name;
    int Map_CB_count;
    CB_TYPE cb_type;
}CB_PLAN;

typedef struct {
    int Total_count;
    int X_CB_count[5];
    bool isMaxLV;
}CALC_RESULT;

typedef struct {
    int Manpower;
    int Ammo;
    int Ration;
    int Part;
    void clean()
    {
        Manpower = 0;
        Ammo = 0;
        Ration = 0;
        Part = 0;
    }
}RESOURCE_CONSUME;


class GF_Calc : public QObject
{
    Q_OBJECT
public:
    explicit GF_Calc(QObject *parent = nullptr);

    void init();
    void Calc_Times(int mainframe);
    void Calc_Consume();

private:
    MAP_DATA map_data;
    GUN_DATA gun_data[5];
    CB_PLAN cb_plan;
    CALC_RESULT gun_result[5];
    RESOURCE_CONSUME source_consume;
    bool isBonus;
    bool isAutoExpand;
    QList<int> ExpList;

    GUN_DATA GetGunData();
    MAP_DATA GetMapData(QString map_name);
    CB_PLAN GetCBPlan();
    void SetGunStatus(int pos,GUN_DATA data);
    int CalcExpCount(int start_lv, int target_lv);
    EXPAND_TYPE GetExpandStatus(int CurrentLV);
    double GetDecayRate(int diffLV);
    void CalcIndex(int mainframe, int *index);
    GUN_CONSUME GetGunConsume(GUN_TYPE type);
    void AdjustMapCBCount(int mapCBCount, int pos);

//    DBOperator *dbOperator;


    int start_lv;
    int target_lv;

signals:
    void comsume_ok(RESOURCE_CONSUME);

public slots:
};

#endif // GF_CALC_H
