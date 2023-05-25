#pragma once
#include <libds/heap_monitor.h>
#include "TypeOfTerritorialUnit.h"
#include <string>
#include <iostream>

using namespace std;

class TerritorialUnit
{
private:
	int sortNumber_;
	string code_;
	string officialTitle_;
	string mediumTitle_;
	string shortTitle_;
	string note_;
	TypeOfTerritorialUnit type_;
	int population_;
public:
	TerritorialUnit();
	TerritorialUnit(const int pSortNumber, const string& pCode, const string& pOfficialTitle, const string& pMediumTitle, const string& pShortTitle, const string& pNote, TypeOfTerritorialUnit pType);
	bool operator==(const TerritorialUnit& source);
	string& getOfficialTitle();
	string& getMediumTitle();
	string& getCode();
	string& getNote();
	int countVowelsInOfficialTitle();
	TypeOfTerritorialUnit getType();
	void addPopulation(int pNumber);
	int getPopulation();
	int getSortNumber();
	void writeInfo();
};

