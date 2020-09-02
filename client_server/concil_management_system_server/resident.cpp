#include "resident.h"

Resident::Resident(){

}

Resident::Resident(QString & ID,
                   QString & name,
                   int & gender,
                   QString & prNumber,
                   int & familyStatus,
                   QString & residentialAddress,
                   QString & domicile,
                   QString & phone,
                   QString & nation,
                   int & educationLevel,
                   QString & occupation,
                   int & militaryService,
                   int & maritalStatus,
                   QString & maritalDate,
                   QString & marriageNumber,
                   QString & spouseInfo,
                   QString & childrenAmount,
                   QString & childrenName,
                   int & contraception,
                   int & onlyChild,
                   int & entitledGroup,
                   int & entitledCategory,
                   int & partyMember,
                   int & subsistenceAllowance,
                   QString & allowanceAmount,
                   int & retirement,
                   QString & pension,
                   int & healthInsurance,
                   QString & immigrationDate,
                   QString & immigrationDistrict,
                   QString & emigrationDate,
                   QString & emigrationCause,
                   QString & notes)
{
    this->ID = ID;
    this->name = name;
    this->gender = gender;
    this->prNumber = prNumber;
    this->familyStatus = familyStatus;
    this->residentialAddress = residentialAddress;
    this->domicile = domicile;
    this->phone = phone;
    this->nation = nation;
    this->educationLevel = educationLevel;
    this->occupation = occupation;
    this->militaryService = militaryService;
    this->maritalStatus = maritalStatus;
    this->maritalDate = maritalDate;
    this->marriageNumber = marriageNumber;
    this->spouseInfo = spouseInfo;
    this->childrenAmount = childrenAmount;
    this->childrenName = childrenName;
    this->contraception = contraception;
    this->onlyChild = onlyChild;
    this->entitledGroup = entitledGroup;
    this->entitledCategory = entitledCategory;
    this->partyMember = partyMember;
    this->subsistenceAllowance = subsistenceAllowance;
    this->allowanceAmount = allowanceAmount;
    this->retirement = retirement;
    this->pension = pension;
    this->healthInsurance = healthInsurance;
    this->immigrationDate = immigrationDate;
    this->immigrationDistrict = immigrationDistrict;
    this->emigrationDate = emigrationDate;
    this->emigrationCause = emigrationCause;
    this->notes = notes;
}
