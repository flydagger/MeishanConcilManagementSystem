#ifndef RESIDENT_H
#define RESIDENT_H

#include <QString>

class Resident
{
public:
    Resident();
    Resident(QString &,QString &,int &,QString &,int &,QString &,QString &,QString &,QString &,int &,QString &,int &,int &,QString &,
             QString &,QString &,QString &,QString &,int &,int &,int &,int &,int &,int &,QString &,int &,QString &,int &,QString &,
             QString &,QString &,QString &,QString &);

    QString ID; //身份证号
    QString name; //姓名
    //性别:男；女；中性；男变女；女变男
//    enum gender{male, female, neuter, m2f, f2m} gender0;
    int gender;
    QString prNumber; //
    //家庭身份:户主,配偶,子女,父母,兄弟姐妹,岳父母,婿媳,侄儿侄女,（外）祖父母,（外）孙子女,其他关系
//    enum familyStatus{hz,po,zn,fm,xdjm,yfm,xx,zezn,zfm,szn,qtgx} familyStatus0;
    int familyStatus;
    QString residentialAddress; //居住地址
    QString domicile; //户籍地
    QString phone; //电话号码
    QString nation; //民族
//    QString birthday; //出生日期
    //文化程度:文盲,小学,初中,高中,大专,大学及以上
//    enum educationLevel{iliteracy,primarySchool,juniorMiddleSchool,hishSchool,diploma,bachelorAndOver} educationLevel0;
    int educationLevel;
    QString occupation; //职业
    //兵役状况:无须服兵役,未服兵役,服兵役中,已服兵役
//    enum militaryService{noNeed,noYet,during,finished} militaryService0;
    int militaryService;
    //婚姻状况:未婚,已婚,离婚,再婚,其他
//    enum maritalStatus{single,married,divorced,remarried,otherMS} maritalStatus0;
    int maritalStatus;
    QString maritalDate; //结婚日期
    QString marriageNumber; //结婚证号
    QString spouseInfo; //配偶信息
    QString childrenAmount; //子女数
    QString childrenName; //子女姓名
    //避孕措施:无,男扎,女扎,安环,其他
//    enum contraception{none,mLigation,fLigation,ring,otherC} contraception0;
    int contraception;
//    enum onlyChild{noOC,yesOC} onlyChild0; //独生子女
    int onlyChild;
//    enum entitledGroup{noEG,yesEG} entitledGroup0; //优抚对象
    int entitledGroup;
//    enum entitledCategory{} entitledCategory0; //优抚类别
    int entitledCategory;
//    enum partyMember{noPM,yesPM} partyMember0; //党员
    int partyMember;
//    enum subsistenceAllowance{noSA,yesSA} subsistenceAllowance0; //享受低保
    int subsistenceAllowance;
    QString allowanceAmount; //低保金额
//    enum retirement{noR,yesR} retirement0; //退休
    int retirement;
    QString pension; //退休金额
    //医保:职工医保,城乡居民医保
//    enum healthInsurance{employeeHI,residentHI} healthInsurance0;
    int healthInsurance;
    QString immigrationDate; //迁入日期
    QString immigrationDistrict; //迁入地区
    QString emigrationDate; //迁出日期
    QString emigrationCause; //迁出原因
    QString notes; //备注
};

#endif // RESIDENT_H
