#include "TerritorialUnit.h"

TerritorialUnit::TerritorialUnit()
{
}

TerritorialUnit::TerritorialUnit(const int pSortNumber, const string& pCode, const string& pOfficialTitle, const string& pMediumTitle, const string& pShortTitle, const string& pNote, TypeOfTerritorialUnit pType)
{
	this->sortNumber_ = pSortNumber;
	this->code_ = pCode;
	this->officialTitle_ = pOfficialTitle;
	this->mediumTitle_ = pMediumTitle;
	this->shortTitle_ = pShortTitle;
	this->note_ = pNote;
	this->type_ = pType;
	this->population_ = 0;
}

bool TerritorialUnit::operator==(const TerritorialUnit& source)
{
	return this == &source;
}

string& TerritorialUnit::getOfficialTitle()
{
	return this->officialTitle_;
}

string& TerritorialUnit::getMediumTitle()
{
	return this->mediumTitle_;
}

string& TerritorialUnit::getCode()
{
	return this->code_;
}

string& TerritorialUnit::getNote()
{
	return this->note_;
}

int TerritorialUnit::getSortNumber()
{
	return this->sortNumber_;
}

int TerritorialUnit::countVowelsInOfficialTitle()
{
	string vowels = "aäeiyouAÄEIYOUáéíıóúÁÉÍİÓÚ";
	int numberOfVowels = 0;

	for (size_t i = 0; i < this->officialTitle_.length(); i++)
	{
		if (vowels.find(officialTitle_[i]) != std::string::npos) 
		{
			++numberOfVowels;
		}
	}
	return numberOfVowels;
}

TypeOfTerritorialUnit TerritorialUnit::getType()
{
	return this->type_;
}

void TerritorialUnit::addPopulation(int pNumber)
{
	this->population_ += pNumber;
}

int TerritorialUnit::getPopulation()
{
	return this->population_;
}

void TerritorialUnit::writeInfo()
{
	cout << this->sortNumber_ << " " << this->code_ << " " << this->officialTitle_ << " " << this->mediumTitle_ << " " << this->shortTitle_ << " " << this->note_ << " - { Population: " << this->population_ << " }" << endl;
}
