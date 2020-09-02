#include "saveexcel.h"
#include "ui_saveexcel.h"
#include "./QXlsx/xlsxdocument.h"
#include "./QXlsx/xlsxchartsheet.h"
#include "./QXlsx/xlsxcellrange.h"
#include "./QXlsx/xlsxchart.h"
#include "./QXlsx/xlsxrichstring.h"
#include "./QXlsx/xlsxworkbook.h"

#include <QDebug>

using namespace QXlsx;

SaveExcel::SaveExcel(QList<Resident> & searchResult, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveExcel)
{
    qDebug() << "In constructor of SaveExcel, the size of searchResult is " << searchResult.size();
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
        for(int col = 0; col < 33; ++col){
            switch (col) {
            case 0:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].ID));break;
            case 1:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].name));break;
            case 2: switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女")));break;
                    case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("中性")));break;
                    case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男变女")));break;
                    case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女变男")));break;
                    default:break;
                    }break;
            case 3:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].prNumber));break;
            case 4: switch (searchResult[row].gender) {
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
            case 5:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].residentialAddress));break;
            case 6:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].domicile));break;
            case 7:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].phone));break;
            case 8:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].nation));break;
            case 9: switch (searchResult[row].gender) {
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
            case 10:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].occupation));break;
            case 11: switch (searchResult[row].gender) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("无须服兵役")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未服兵役")));break;
                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("服兵役中")));break;
                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("已服兵役")));break;
                     default:break;
                     }break;
            case 12: switch (searchResult[row].gender) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未婚")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("已婚")));break;
                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("离婚")));break;
                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("再婚")));break;
                     case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他")));break;
                     default:break;
                     }break;
            case 13:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].maritalDate));break;
            case 14:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].marriageNumber));break;
            case 15:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].spouseInfo));break;
            case 16:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].childrenAmount));break;
            case 17:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].childrenName));break;
            case 18: switch (searchResult[row].gender) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("无")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("男扎")));break;
                     case 3: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("女扎")));break;
                     case 4: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("安环")));break;
                     case 5: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("其他")));break;
                     default:break;
                     }break;
            case 19: switch (searchResult[row].gender) {
                     case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                     case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                     case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                     default:break;
                     }break;
            case 20:switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 21: if (searchResult[row].gender == 0) {
                         ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    }break;
            case 22:switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 23:switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 24:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].allowanceAmount));break;
            case 25:switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("否")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("是")));break;
                    default:break;
                    }break;
            case 26:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].pension));break;
            case 27:switch (searchResult[row].gender) {
                    case 0: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("未知")));break;
                    case 1: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("职工医保")));break;
                    case 2: ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("城乡居民医保")));break;
                    default:break;
                    }break;
            case 28:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].immigrationDate));break;
            case 29:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].immigrationDistrict));break;
            case 30:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].emigrationDate));break;
            case 31:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].emigrationCause));break;
            case 32:ui->tableWidget->setItem(row, col, new QTableWidgetItem(searchResult[row].notes));break;
            default:break;
            }
        }
    }
}

SaveExcel::~SaveExcel()
{
    delete ui;
}

void SaveExcel::OnSave(){

    QXlsx::Document saveExcel;
    QString str = "asdf";

    saveExcel.write(1, 1, "身份证号");
    saveExcel.write(1, 1, "姓名");
    saveExcel.write(1, 1, "性别");
    saveExcel.write(1, 1, "户口编号");
    saveExcel.write(1, 1, "家庭身份");
    saveExcel.write(1, 1, "居住地址");
    saveExcel.write(1, 1, "户籍地");
    saveExcel.write(1, 1, "电话号码");
    saveExcel.write(1, 1, "民族");
    saveExcel.write(1, 1, "文化程度");
    saveExcel.write(1, 1, "职业");
    saveExcel.write(1, 1, "兵役状况");
    saveExcel.write(1, 1, "婚姻状况");
    saveExcel.write(1, 1, "结婚日期");
    saveExcel.write(1, 1, "结婚证号");
    saveExcel.write(1, 1, "配偶信息");
    saveExcel.write(1, 1, "子女数");
    saveExcel.write(1, 1, "子女姓名");
    saveExcel.write(1, 1, "避孕措施");
    saveExcel.write(1, 1, "独生子女");
    saveExcel.write(1, 1, "优抚对象");
    saveExcel.write(1, 1, "优抚类别");
    saveExcel.write(1, 1, "党员");
    saveExcel.write(1, 1, "享受低保");
    saveExcel.write(1, 1, "低保金额");
    saveExcel.write(1, 1, "退休");
    saveExcel.write(1, 1, "退休金额");
    saveExcel.write(1, 1, "医保");
    saveExcel.write(1, 1, "迁入日期");
    saveExcel.write(1, 1, "迁入地区");
    saveExcel.write(1, 1, "迁出日期");
    saveExcel.write(1, 1, "迁出原因");
    saveExcel.write(1, 1, "备注");

    for(int row = 0; row < population; ++row){
        for(int col = 0; col < 33; ++col){
            saveExcel.write(row + 2, col + 1, ui->tableWidget->item(row, col)->text());
        }
    }

    if(ui->lineEdit->text() == ""){
        saveExcel.saveAs(ui->lineEdit->placeholderText() + ".xlsx");
    }else{
        saveExcel.saveAs(ui->lineEdit->text() + ".xlsx");
    }
    accept();
}

void SaveExcel::OnCancel(){
    reject();
}
