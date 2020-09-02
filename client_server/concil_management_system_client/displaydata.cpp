#include "displaydata.h"
#include "ui_displaydata.h"
#include "./QXlsx/xlsxdocument.h"
#include "./QXlsx/xlsxchartsheet.h"
#include "./QXlsx/xlsxcellrange.h"
#include "./QXlsx/xlsxchart.h"
#include "./QXlsx/xlsxrichstring.h"
#include "./QXlsx/xlsxworkbook.h"

#include <QDebug>

using namespace QXlsx;

DisplayData::DisplayData(QList<Resident> & searchResult, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayData)
{
    qDebug() << "In constructor of DisplayData, the size of searchResult is " << searchResult.size();
    ui->setupUi(this);
    connect(ui->pBtn_save, SIGNAL(clicked()), this, SLOT(OnSave()));
    connect(ui->pBtn_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

    this->searchResult = &searchResult;
    population = searchResult.size();
    ui->label->setText("总计：" + QString::number(population) + "人");
    ui->tableWidget->setRowCount(population);
/*
00    身份证号 string
01    姓名    string
02    性别    int
03    户口编号 string
04    家庭身份 int
05    居住地址 string
06    户籍地   string
07    电话号码 string
08    民族    string
09    文化程度 int
10    职业    string
11    兵役状况 int
12    婚姻状况 int
13    结婚日期 string
14    结婚证号 string
15    配偶信息 string
16    子女数   string
17    子女姓名 string
18    避孕措施 int
19    独生子女 int
20    优抚对象 int
21    优抚类别 int
22    党员    int
23    享受低保 int
24    低保金额 string
25    退休    int
26    退休金额 string
27    医保    int
28    迁入日期 string
29    迁入地区 string
30    迁出日期 string
31    迁出原因 string
32    备注    string
*/

    for(int row = 0; row < population; ++row){
        for(int col = 0; col < 35; ++col){
            switch (col) {
            case 0:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].prNumber.split("-")[1]));break;
            case 1:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].ID));break;
            case 2:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].name));break;
            case 3: switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女")));break;
                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("中性")));break;
                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男变女")));break;
                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女变男")));break;
                    default:break;
                    }break;
            case 4:ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString::number(ID2age(searchResult[row].ID))));break;
            case 5:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].prNumber));break;
            case 6: switch (searchResult[row].familyStatus) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("户主")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("配偶")));break;
                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("子女")));break;
                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("父母")));break;
                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("兄弟姐妹")));break;
                    case 6: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("岳父母")));break;
                    case 7: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("婿媳")));break;
                    case 8: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("侄儿侄女")));break;
                    case 9: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("（外）祖父母")));break;
                    case 10: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("（外）孙子女")));break;
                    case 11: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他关系")));break;
                    default:break;
                    }break;
            case 7:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].residentialAddress));break;
            case 8:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].domicile));break;
            case 9:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].phone));break;
            case 10:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].nation));break;
            case 11: switch (searchResult[row].educationLevel) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("文盲")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("小学")));break;
                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("初中")));break;
                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("高中")));break;
                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("大学")));break;
                    case 6: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("大专")));break;
                    case 7: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("大学及以上")));break;
                    default:break;
                    }break;
            case 12:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].occupation));break;
            case 13: switch (searchResult[row].militaryService) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("无须服兵役")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未服兵役")));break;
                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("服兵役中")));break;
                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("已服兵役")));break;
                     default:break;
                     }break;
            case 14: switch (searchResult[row].maritalStatus) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未婚")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("已婚")));break;
                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("离婚")));break;
                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("再婚")));break;
                     case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他")));break;
                     default:break;
                     }break;
            case 15:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].maritalDate));break;
            case 16:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].marriageNumber));break;
            case 17:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].spouseInfo));break;
            case 18:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].childrenAmount));break;
            case 19:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].childrenName));break;
            case 20: switch (searchResult[row].contraception) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("无")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男扎")));break;
                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女扎")));break;
                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("安环")));break;
                     case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他")));break;
                     default:break;
                     }break;
            case 21: switch (searchResult[row].onlyChild) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                     default:break;
                     }break;
            case 22:switch (searchResult[row].entitledGroup) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 23: if (searchResult[row].entitledCategory == 0) {
                         ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    }break;
            case 24:switch (searchResult[row].partyMember) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 25:switch (searchResult[row].subsistenceAllowance) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 26:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].allowanceAmount));break;
            case 27:switch (searchResult[row].retirement) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 28:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].pension));break;
            case 29:switch (searchResult[row].healthInsurance) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("职工医保")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("城乡居民医保")));break;
                    default:break;
                    }break;
            case 30:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].immigrationDate));break;
            case 31:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].immigrationDistrict));break;
            case 32:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].emigrationDate));break;
            case 33:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].emigrationCause));break;
            case 34:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].notes));break;


//            case 1:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].ID));break;
//            case 2:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].name));break;
//            case 3: switch (searchResult[row].gender) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女")));break;
//                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("中性")));break;
//                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男变女")));break;
//                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女变男")));break;
//                    default:break;
//                    }break;
//            case 4:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].prNumber));break;
//            case 5: switch (searchResult[row].familyStatus) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("户主")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("配偶")));break;
//                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("子女")));break;
//                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("父母")));break;
//                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("兄弟姐妹")));break;
//                    case 6: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("岳父母")));break;
//                    case 7: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("婿媳")));break;
//                    case 8: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("侄儿侄女")));break;
//                    case 9: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("（外）祖父母")));break;
//                    case 10: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("（外）孙子女")));break;
//                    case 11: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他关系")));break;
//                    default:break;
//                    }break;
//            case 7:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].residentialAddress));break;
//            case 8:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].domicile));break;
//            case 9:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].phone));break;
//            case 10:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].nation));break;
//            case 11: switch (searchResult[row].educationLevel) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("文盲")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("小学")));break;
//                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("初中")));break;
//                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("高中")));break;
//                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("大学")));break;
//                    case 6: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("大专")));break;
//                    case 7: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("大学及以上")));break;
//                    default:break;
//                    }break;
//            case 12:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].occupation));break;
//            case 13: switch (searchResult[row].militaryService) {
//                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("无须服兵役")));break;
//                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未服兵役")));break;
//                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("服兵役中")));break;
//                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("已服兵役")));break;
//                     default:break;
//                     }break;
//            case 14: switch (searchResult[row].maritalStatus) {
//                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未婚")));break;
//                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("已婚")));break;
//                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("离婚")));break;
//                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("再婚")));break;
//                     case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他")));break;
//                     default:break;
//                     }break;
//            case 15:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].maritalDate));break;
//            case 16:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].marriageNumber));break;
//            case 17:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].spouseInfo));break;
//            case 18:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].childrenAmount));break;
//            case 19:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].childrenName));break;
//            case 20: switch (searchResult[row].contraception) {
//                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("无")));break;
//                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男扎")));break;
//                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女扎")));break;
//                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("安环")));break;
//                     case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他")));break;
//                     default:break;
//                     }break;
//            case 21: switch (searchResult[row].onlyChild) {
//                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
//                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
//                     default:break;
//                     }break;
//            case 22:switch (searchResult[row].entitledGroup) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
//                    default:break;
//                    }break;
//            case 23: if (searchResult[row].entitledCategory == 0) {
//                         ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    }break;
//            case 24:switch (searchResult[row].partyMember) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
//                    default:break;
//                    }break;
//            case 25:switch (searchResult[row].subsistenceAllowance) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
//                    default:break;
//                    }break;
//            case 26:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].allowanceAmount));break;
//            case 27:switch (searchResult[row].retirement) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
//                    default:break;
//                    }break;
//            case 28:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].pension));break;
//            case 29:switch (searchResult[row].healthInsurance) {
//                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
//                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("职工医保")));break;
//                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("城乡居民医保")));break;
//                    default:break;
//                    }break;
//            case 30:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].immigrationDate));break;
//            case 31:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].immigrationDistrict));break;
//            case 32:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].emigrationDate));break;
//            case 33:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].emigrationCause));break;
//            case 34:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].notes));break;
            default:break;
            }
        }
    }
}

DisplayData::~DisplayData()
{
    delete ui;
}

void DisplayData::OnSave(){

    QXlsx::Document saveExcel;
    QString str = "asdf";

    saveExcel.write(1, 1, "身份证号");
    saveExcel.write(1, 2, "姓名");
    saveExcel.write(1, 3, "性别");
    saveExcel.write(1, 4, "户口编号");
    saveExcel.write(1, 5, "家庭身份");
    saveExcel.write(1, 6, "居住地址");
    saveExcel.write(1, 7, "户籍地");
    saveExcel.write(1, 8, "电话号码");
    saveExcel.write(1, 9, "民族");
    saveExcel.write(1, 10, "文化程度");
    saveExcel.write(1, 11, "职业");
    saveExcel.write(1, 12, "兵役状况");
    saveExcel.write(1, 13, "婚姻状况");
    saveExcel.write(1, 14, "结婚日期");
    saveExcel.write(1, 15, "结婚证号");
    saveExcel.write(1, 16, "配偶信息");
    saveExcel.write(1, 17, "子女数");
    saveExcel.write(1, 18, "子女姓名");
    saveExcel.write(1, 19, "避孕措施");
    saveExcel.write(1, 20, "独生子女");
    saveExcel.write(1, 21, "优抚对象");
    saveExcel.write(1, 22, "优抚类别");
    saveExcel.write(1, 23, "党员");
    saveExcel.write(1, 24, "享受低保");
    saveExcel.write(1, 25, "低保金额");
    saveExcel.write(1, 26, "退休");
    saveExcel.write(1, 27, "退休金额");
    saveExcel.write(1, 28, "医保");
    saveExcel.write(1, 29, "迁入日期");
    saveExcel.write(1, 30, "迁入地区");
    saveExcel.write(1, 31, "迁出日期");
    saveExcel.write(1, 32, "迁出原因");
    saveExcel.write(1, 33, "备注");

    qDebug() << (*searchResult)[0].ID;
    qDebug() << (*searchResult)[0].name;
    qDebug() << (*searchResult)[0].gender;
    qDebug() << (*searchResult)[0].prNumber;
    qDebug() << (*searchResult)[0].familyStatus;
    qDebug() << (*searchResult)[0].residentialAddress;
    qDebug() << (*searchResult)[0].domicile;
    qDebug() << (*searchResult)[0].phone;
    qDebug() << (*searchResult)[0].nation;
    qDebug() << (*searchResult)[0].educationLevel;
    qDebug() << (*searchResult)[0].occupation;
    qDebug() << (*searchResult)[0].militaryService;
    qDebug() << (*searchResult)[0].maritalStatus;
    qDebug() << (*searchResult)[0].maritalDate;
    qDebug() << (*searchResult)[0].marriageNumber;
    qDebug() << (*searchResult)[0].spouseInfo;
    qDebug() << (*searchResult)[0].childrenAmount;
    qDebug() << (*searchResult)[0].childrenName;
    qDebug() << (*searchResult)[0].contraception;
    qDebug() << (*searchResult)[0].onlyChild;
    qDebug() << (*searchResult)[0].entitledGroup;
    qDebug() << (*searchResult)[0].entitledCategory;
    qDebug() << (*searchResult)[0].partyMember;
    qDebug() << (*searchResult)[0].subsistenceAllowance;
    qDebug() << (*searchResult)[0].allowanceAmount;
    qDebug() << (*searchResult)[0].retirement;
    qDebug() << (*searchResult)[0].pension;
    qDebug() << (*searchResult)[0].healthInsurance;
    qDebug() << (*searchResult)[0].immigrationDate;
    qDebug() << (*searchResult)[0].immigrationDistrict;
    qDebug() << (*searchResult)[0].emigrationDate;
    qDebug() << (*searchResult)[0].emigrationCause;
    qDebug() << (*searchResult)[0].notes;

    for(int row = 0; row < population; ++row){
        saveExcel.write(row + 2, 1, (*searchResult)[row].ID);
        saveExcel.write(row + 2, 2, (*searchResult)[row].name);
        switch ((*searchResult)[row].gender) {
        case 0: saveExcel.write(row + 2, 3, "未知");break;
        case 1: saveExcel.write(row + 2, 3, "男");break;
        case 2: saveExcel.write(row + 2, 3, "女");break;
        case 3: saveExcel.write(row + 2, 3, "中性");break;
        case 4: saveExcel.write(row + 2, 3, "男变女");break;
        case 5: saveExcel.write(row + 2, 3, "女变男");break;
        default:break;
        }
        saveExcel.write(row + 2, 4, (*searchResult)[row].prNumber);
        switch ((*searchResult)[row].familyStatus) {
        case 0: saveExcel.write(row + 2, 5, "未知");break;
        case 1: saveExcel.write(row + 2, 5, "户主");break;
        case 2: saveExcel.write(row + 2, 5, "配偶");break;
        case 3: saveExcel.write(row + 2, 5, "子女");break;
        case 4: saveExcel.write(row + 2, 5, "父母");break;
        case 5: saveExcel.write(row + 2, 5, "兄弟姐妹");break;
        case 6: saveExcel.write(row + 2, 5, "岳父母");break;
        case 7: saveExcel.write(row + 2, 5, "婿媳");break;
        case 8: saveExcel.write(row + 2, 5, "侄儿侄女");break;
        case 9: saveExcel.write(row + 2, 5, "（外）祖父母");break;
        case 10: saveExcel.write(row + 2, 5, "（外）孙子女");break;
        case 11: saveExcel.write(row + 2, 5, "其他关系");break;
        default:break;
        }
        saveExcel.write(row + 2, 6, (*searchResult)[row].residentialAddress);
        saveExcel.write(row + 2, 7, (*searchResult)[row].domicile);
        saveExcel.write(row + 2, 8, (*searchResult)[row].phone);
        saveExcel.write(row + 2, 9, (*searchResult)[row].nation);
        switch ((*searchResult)[row].educationLevel) {
        case 0: saveExcel.write(row + 2, 10, "未知");break;
        case 1: saveExcel.write(row + 2, 10, "文盲");break;
        case 2: saveExcel.write(row + 2, 10, "小学");break;
        case 3: saveExcel.write(row + 2, 10, "初中");break;
        case 4: saveExcel.write(row + 2, 10, "高中");break;
        case 5: saveExcel.write(row + 2, 10, "大学");break;
        case 6: saveExcel.write(row + 2, 10, "大专");break;
        case 7: saveExcel.write(row + 2, 10, "大学及以上");break;
        default:break;
        }
        saveExcel.write(row + 2, 11, (*searchResult)[row].occupation);
        switch ((*searchResult)[row].militaryService) {
        case 0: saveExcel.write(row + 2, 12, "未知");break;
        case 1: saveExcel.write(row + 2, 12, "无须服兵役");break;
        case 2: saveExcel.write(row + 2, 12, "未服兵役");break;
        case 3: saveExcel.write(row + 2, 12, "服兵役中");break;
        case 4: saveExcel.write(row + 2, 12, "已服兵役");break;
        default:break;
        }
        switch ((*searchResult)[row].maritalStatus) {
        case 0: saveExcel.write(row + 2, 13, "未知");break;
        case 1: saveExcel.write(row + 2, 13, "未婚");break;
        case 2: saveExcel.write(row + 2, 13, "已婚");break;
        case 3: saveExcel.write(row + 2, 13, "离婚");break;
        case 4: saveExcel.write(row + 2, 13, "再婚");break;
        case 5: saveExcel.write(row + 2, 13, "其他");break;
        default:break;
        }
        saveExcel.write(row + 2, 14, (*searchResult)[row].maritalDate);
        saveExcel.write(row + 2, 15, (*searchResult)[row].marriageNumber);
        saveExcel.write(row + 2, 16, (*searchResult)[row].spouseInfo);
        saveExcel.write(row + 2, 17, (*searchResult)[row].childrenAmount);
        saveExcel.write(row + 2, 18, (*searchResult)[row].childrenName);
        switch ((*searchResult)[row].contraception) {
        case 0: saveExcel.write(row + 2, 19, "未知");break;
        case 1: saveExcel.write(row + 2, 19, "无");break;
        case 2: saveExcel.write(row + 2, 19, "男扎");break;
        case 3: saveExcel.write(row + 2, 19, "女扎");break;
        case 4: saveExcel.write(row + 2, 19, "安环");break;
        case 5: saveExcel.write(row + 2, 19, "其他");break;
        default:break;
        }
        switch ((*searchResult)[row].onlyChild) {
        case 0: saveExcel.write(row + 2, 20, "未知");break;
        case 1: saveExcel.write(row + 2, 20, "否");break;
        case 2: saveExcel.write(row + 2, 20, "是");break;
        default:break;
        }
        switch ((*searchResult)[row].entitledGroup) {
        case 0: saveExcel.write(row + 2, 21, "未知");break;
        case 1: saveExcel.write(row + 2, 21, "否");break;
        case 2: saveExcel.write(row + 2, 21, "是");break;
        default:break;
        }
        saveExcel.write(row + 2, 22, "未知");  // entitledCategory
        switch ((*searchResult)[row].partyMember) {
        case 0: saveExcel.write(row + 2, 23, "未知");break;
        case 1: saveExcel.write(row + 2, 23, "否");break;
        case 2: saveExcel.write(row + 2, 23, "是");break;
        default:break;
        }
        switch ((*searchResult)[row].subsistenceAllowance) {
        case 0: saveExcel.write(row + 2, 24, "未知");break;
        case 1: saveExcel.write(row + 2, 24, "否");break;
        case 2: saveExcel.write(row + 2, 24, "是");break;
        default:break;
        }
        saveExcel.write(row + 2, 25, (*searchResult)[row].allowanceAmount);
        switch ((*searchResult)[row].retirement) {
        case 0: saveExcel.write(row + 2, 26, "未知");break;
        case 1: saveExcel.write(row + 2, 26, "否");break;
        case 2: saveExcel.write(row + 2, 26, "是");break;
        default:break;
        }
        saveExcel.write(row + 2, 27, (*searchResult)[row].pension);
        switch ((*searchResult)[row].retirement) {
        case 0: saveExcel.write(row + 2, 28, "未知");break;
        case 1: saveExcel.write(row + 2, 28, "职工医保");break;
        case 2: saveExcel.write(row + 2, 28, "城乡居民医保");break;
        default:break;
        }
        saveExcel.write(row + 2, 29, (*searchResult)[row].immigrationDate);
        saveExcel.write(row + 2, 30, (*searchResult)[row].immigrationDistrict);
        saveExcel.write(row + 2, 31, (*searchResult)[row].emigrationDate);
        saveExcel.write(row + 2, 32, (*searchResult)[row].emigrationCause);
        saveExcel.write(row + 2, 33, (*searchResult)[row].notes);
    }

    if(ui->lineEdit->text() == ""){
        saveExcel.saveAs(ui->lineEdit->placeholderText() + ".xlsx");
    }else{
        saveExcel.saveAs(ui->lineEdit->text() + ".xlsx");
    }
}

void DisplayData::OnCancel(){
    close();
}

/* Function: Compare two qstring.
 * Param: strStar - string may include *
 * Param: str - string without *
 * Retrun: true if two strings are equivalent
 */
bool DisplayData::stringCompare(QString & strStar, QString & str){
    if(strStar.length() != str.length())
        return false;
    for(int i = 0; i < strStar.length(); i++){
        if(strStar[i] == "*")
            continue;
        if(strStar[i] != str[i])
            return false;
    }
    return true;
}

int DisplayData::ID2age(const QString & ID){
    int year = ID.mid(6, 4).toInt();
    int month = ID.mid(10, 2).toInt();
    int day = ID.mid(12, 2).toInt();

    int tYear = QDate::currentDate().year();
    int tMonth = QDate::currentDate().month();
    int tDay = QDate::currentDate().day();

    if(tMonth > month){
        return tYear - year;
    }else if(tMonth == month){
        if(tDay >= day)
            return tYear - year;
        else
            return tYear - year - 1;
    }else{
        return tYear - year - 1;
    }
}
