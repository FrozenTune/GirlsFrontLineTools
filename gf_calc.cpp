#include "gf_calc.h"
#include <QDebug>

GF_Calc::GF_Calc(QObject *parent) : QObject(parent)
{

}

void GF_Calc::init()
{
    isBonus = false;
    isAutoExpand = true;
    start_lv = 15;
    target_lv = 95;
    cb_plan.Map_CB_count = 5;
    cb_plan.cb_type = normal;
    for(int i = 0; i < 5; i++)
    {
        SetGunStatus(i,GetGunData());
    }
    for(int i = 0; i<100; i++)
    {
        ExpList.append(i*100);
    }
    map_data = GetMapData("1-1");

}

/*******************
 * Gather Gun Data
 *
 *******************/
GUN_DATA GF_Calc::GetGunData()
{
    GUN_DATA gun;
    //Data from UI
    gun.Gun_type = HG;
    gun.Force_Expansion = false;
    gun.Expansion = X2;
    gun.Start_LV = start_lv;
    start_lv+=5;
    gun.Target_LV = target_lv;
    target_lv=100;
    gun.Cur_EXP = 26;
    gun.MVP = false;
    gun.Marriage = false;


    //Data from db
    gun.HP = 497;

    //Auto data
    gun.Leader = false;

    return gun;
}

/**********************
 * Gather map data
 * map_name from UI
 **********************/
MAP_DATA GF_Calc::GetMapData(QString map_name)
{
    MAP_DATA map;
    //Data from db
    map.CB_EXP = 500;
    map.EXP_weaken_LV = 83;

    return map;
}


/***********************
 * Gather plan data
 *
 **********************/
CB_PLAN GF_Calc::GetCBPlan()
{
    CB_PLAN plan;
    //Data from UI
    plan.cb_type = normal;
    plan.Map_CB_count = 5;
    plan.Map_name = "5-4E";
    return plan;
}


void GF_Calc::SetGunStatus(int pos, GUN_DATA data)
{
    if(pos>4) return;

    if(pos == 0)
    {
        data.Leader = true;
        data.MVP = true;
        data.cb_type = normal;
//        data.Marriage = true;
    }


    this->gun_data[pos] = data;
}

int GF_Calc::CalcExpCount(int start_lv, int target_lv)
{
    return 1000;
}

EXPAND_TYPE GF_Calc::GetExpandStatus(int CurrentLV)
{
    EXPAND_TYPE val = X1;
    if(CurrentLV>=90) val = X5;
    else if(CurrentLV>=70) val = X4;
    else if(CurrentLV>=30) val = X3;
    else if(CurrentLV>=10) val = X2;
    return val;
}

double GF_Calc::GetDecayRate(int diffLV)
{
    double val = 1;
    if(diffLV<0) return val;
    switch (diffLV/10) {
    case 0:
        val = 0.8;
        break;
    case 1:
        val = 0.6;
        break;
    case 2:
        val = 0.4;
        break;
    case 3:
        val = 0.2;
        break;
    case 4:
        val = 0;
        break;
    default:
        val = 0;
        break;
    }

    return val;
}

void GF_Calc::CalcIndex(int mainframe, int *index)
{
    index[0] = mainframe;
    int j = 1;
    for(int i=0; i<5; i++)
    {
        if(i!=mainframe)
            index[j++] = i;
    }
}

//自适应每阶段地图内单轮战斗次数
void GF_Calc::AdjustMapCBCount(int mapCBCount,int pos)
{
    CALC_RESULT *temp_result;
    int temp_cb_count = 0;
    int over_cb_count = 0;

    //获取单角色战斗信息
    temp_result = &gun_result[pos];
    //循环计算各个扩编阶段战斗次数
    for(int j = 0; j < 4; j++)
    {
        //当前扩编战斗次数
        temp_cb_count = temp_result->X_CB_count[j];
        //达到扩编等级后图内仍需战斗次数（地图单次战斗数 - 当前扩编战斗数 % 达到扩编等级后图内仍需战斗次数（地图单次战斗数）
        over_cb_count = mapCBCount - temp_cb_count % mapCBCount;

        //当前扩编战斗数为0时不计算
        if(over_cb_count > 0 && temp_cb_count > 0)
        {
            temp_result->X_CB_count[j] += over_cb_count;
            temp_result->X_CB_count[j+1] -= over_cb_count;
        }
    }
}


void GF_Calc::Calc_Times(int mainframe)
{
    double MultiplyStats = 1;
    double ExpandBonus = 1;
    double DecayRate = 1;
    int CurrentExp = 0;
    double SingleCBExp = 0;
    double LvRemainExp = 0;
    int X1_CB_Count = 0, X2_CB_Count = 0, X3_CB_Count = 0, X4_CB_Count = 0, X5_CB_Count = 0;
    int Map_Weaken_lv = map_data.EXP_weaken_LV;
    int MapBaseExp = map_data.CB_EXP;
    EXPAND_TYPE ExpandType;
    int pos;
    int index[5];
    CalcIndex(mainframe,index);

    for(int i = 0; i < 5; i++)
    {
        pos = index[i];
        qDebug()<<"pos"<<pos;

        X1_CB_Count = 0, X2_CB_Count = 0, X3_CB_Count = 0, X4_CB_Count = 0, X5_CB_Count = 0;
        MultiplyStats = 1;
        MultiplyStats *= gun_data[pos].Leader?1.2:1;
        MultiplyStats *= gun_data[pos].MVP?1.3:1;
        MultiplyStats *= isBonus?1.5:1;
        MultiplyStats *= gun_data[pos].Marriage?2:1;

        qDebug()<<__LINE__<<"Gun_data"<<gun_data[pos].Leader<<"MVP"<<gun_data[pos].MVP<<"MultiplyStats"<<MultiplyStats;
        qDebug()<<__LINE__<<"Start_LV"<<gun_data[pos].Start_LV<<"Target_lV"<<gun_data[pos].Target_LV;

        int CurrentLV;
        for(CurrentLV = gun_data[pos].Start_LV;
            CurrentLV < gun_data[pos].Target_LV; CurrentLV++)
        {
            LvRemainExp = ExpList.at(CurrentLV-1);
            LvRemainExp -= CurrentExp;

            if(LvRemainExp <= 0)
                CurrentExp = -LvRemainExp;
            else
            {
                ExpandType = GetExpandStatus(CurrentLV);
                if(isAutoExpand)
                {
                    switch (ExpandType) {
                    case X1:
                        ExpandBonus = 1;
                        break;
                    case X2:
                        ExpandBonus = 1.5;
                        break;
                    case X3:
                        ExpandBonus = 2;
                        break;
                    case X4:
                        ExpandBonus = 2.5;
                        break;
                    case X5:
                        ExpandBonus = 3;
                        break;
                    }
                }
                else
                {
                    ExpandBonus = (double)(1 + gun_data[pos].Expansion * 0.5);
                }

                DecayRate = GetDecayRate(CurrentLV-Map_Weaken_lv);
    //                qDebug()<<__LINE__<<"CurrentLV="<<CurrentLV<<"DecayRate="<<DecayRate;

                SingleCBExp = MapBaseExp * ExpandBonus * DecayRate * MultiplyStats;
                if(SingleCBExp==0) SingleCBExp = 10;
    //                qDebug()<<MapBaseExp<<ExpandBonus<<DecayRate<<MultiplyStats;

                while(LvRemainExp > 0)
                {
                    LvRemainExp -= SingleCBExp;
    //                    qDebug()<<"LvRemainExp"<<LvRemainExp<<"SingleCBExp"<<SingleCBExp;
                    switch (ExpandType) {
                    case X1:
                        X1_CB_Count++;
                        break;
                    case X2:
                        X2_CB_Count++;
                        break;
                    case X3:
                        X3_CB_Count++;
                        break;
                    case X4:
                        X4_CB_Count++;
                        break;
                    case X5:
                        X5_CB_Count++;
                        break;
                    }
                }
                CurrentExp = -LvRemainExp;
            }
        }

        gun_result[pos].isMaxLV = CurrentLV == 100;
        gun_result[pos].X_CB_count[0] = X1_CB_Count;
        gun_result[pos].X_CB_count[1] = X2_CB_Count;
        gun_result[pos].X_CB_count[2] = X3_CB_Count;
        gun_result[pos].X_CB_count[3] = X4_CB_Count;
        gun_result[pos].X_CB_count[4] = X5_CB_Count;

        gun_result[pos].Total_count = X1_CB_Count + X2_CB_Count + X3_CB_Count+
                                    X4_CB_Count + X5_CB_Count;



        AdjustMapCBCount(cb_plan.Map_CB_count,pos);
        if(pos == mainframe)
        {
            qDebug()<<gun_result[pos].X_CB_count[0]
                    <<gun_result[pos].X_CB_count[1]
                    <<gun_result[pos].X_CB_count[2]
                    <<gun_result[pos].X_CB_count[3]
                    <<gun_result[pos].X_CB_count[4];
            qDebug()<<__LINE__<<"Count="<<gun_result[pos].Total_count;
        }
        else
        {
            int mainframe_count = gun_result[mainframe].Total_count;

            if(gun_result[pos].Total_count>mainframe_count)
            {
                qDebug()<<"Other Count"<<mainframe_count;

                int gun_index;
                int fake_count = 0;
                int over_count = 0;
                for(gun_index = 0; gun_index < 5; gun_index++)
                {
                    fake_count += gun_result[pos].X_CB_count[gun_index];
                    if(fake_count>mainframe_count) break;
                }

                over_count = fake_count - mainframe_count;
                gun_result[pos].X_CB_count[gun_index] -= over_count;
                qDebug()<<"gun_index"<<gun_index;
                for(gun_index++;gun_index<5;gun_index++)
                {
                   gun_result[pos].X_CB_count[gun_index] = 0;
                }


                qDebug()<<"Over Count"<<over_count<<"fake_count"<<fake_count;

                qDebug()<<gun_result[pos].X_CB_count[0]
                        <<gun_result[pos].X_CB_count[1]
                        <<gun_result[pos].X_CB_count[2]
                        <<gun_result[pos].X_CB_count[3]
                        <<gun_result[pos].X_CB_count[4];


            }
            else
            {
                qDebug()<<"Other Count"<<mainframe_count;
            }
        }
    }
}

void GF_Calc::Calc_Consume()
{
    source_consume.clean();
    int manpower,ammo,ration,part;
    GUN_CONSUME gun_consume;
    int temp_CB_Count = 0;
    gun_consume.clean();

//    if(cb_plan.cb_type == normal)
//    {

//    }

    //五把枪同时算
    for(int i = 0; i < 5; i++)
    {
        gun_consume = GetGunConsume(gun_data[i].Gun_type);
        if(gun_data[i].cb_type == saving) gun_consume.clean();

        //5种扩编战斗次数计算
        for(int j = 0; j < 5; j++)
        {
            temp_CB_Count = gun_result[i].X_CB_count[j];
            ammo = gun_consume.Ammo_consume;
            ration = gun_consume.Ration_consume;
//                qDebug()<<"ammo"<<ammo<<"ration"<<ration;

            ammo += gun_consume.Ammo_increment * j;
            ration += gun_consume.Ration_increment * j;

            ammo /= 5;
            ration /= 10;
//                qDebug()<<"ammo"<<ammo<<"ration"<<ration;

            ammo *= temp_CB_Count;
            ration *= temp_CB_Count;
//                qDebug()<<"ammo"<<ammo<<"ration"<<ration<<"temp_CB_Count"<<temp_CB_Count;

            source_consume.Ammo += ammo;
            source_consume.Ration += ration;
        }
    }


    qDebug()<<"CONSUME: Ammo"<<source_consume.Ammo<<"Ration"<<source_consume.Ration;

    emit comsume_ok(source_consume);


}

GUN_CONSUME GF_Calc::GetGunConsume(GUN_TYPE type)
{
    GUN_CONSUME consume;
    consume.Ammo_consume = 10;
    consume.Ammo_increment = 5;
    consume.Ration_consume = 10;
    consume.Ration_increment = 5;
    return consume;
}
