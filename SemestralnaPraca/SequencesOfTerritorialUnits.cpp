#include "SequencesOfTerritorialUnits.h"

SequencesOfTerritorialUnits::SequencesOfTerritorialUnits()
{
	this->reader_ = new InputReader("kraje.csv");
}

void SequencesOfTerritorialUnits::loadSequences()
{
	TerritorialUnit* tempUnit = nullptr;

	while ((tempUnit = this->reader_->read()) != nullptr)
	{
		this->kraje_.insertLast().data_ = tempUnit;
	}

	reader_->changeFile("okresy.csv");
	while ((tempUnit = this->reader_->read()) != nullptr)
	{
		this->okresy_.insertLast().data_ = tempUnit;
	}

	reader_->changeFile("obce.csv");
	while ((tempUnit = this->reader_->read()) != nullptr)
	{
		this->obce_.insertLast().data_ = tempUnit;
	}

}

void SequencesOfTerritorialUnits::start()
{
	string inputTerritorialUnit, inputString, inputPredicate, tempTitle;
	ImplicitSequence<TerritorialUnit*>* dataPointer;
	ImplicitSequence<TerritorialUnit*> newData;

	while (true)
	{
		cout << endl << "--------------------------------------------------------------------------------------------------------" << endl;
		cout << "Zvo¾te prosím územné jednotky, s ktorými chcete pracova: \t [ kraje / okresy / obce / exit ]" << endl;
		getline(cin, inputTerritorialUnit);
		if (inputTerritorialUnit == "exit")
		{
			break;
		}
		else if (inputTerritorialUnit == "kraje")
		{
			dataPointer = &this->kraje_;
		}
		else if (inputTerritorialUnit == "okresy")
		{
			dataPointer = &this->okresy_;
		}
		else if (inputTerritorialUnit == "obce")
		{
			dataPointer = &this->obce_;
		}
		else {
			cout << "Nezadali ste správnu operáciu, prosím opravte sa." << endl << endl;
			continue;
		}
		cout << "Zvo¾te prosím predikát, s ktorým chcete pracova: \t [ [s]tartsWithStr / [c]ontainsStr / exit ]" << endl;
		getline(cin, inputPredicate);
		cout << "Zadajte string." << endl;
		getline(cin, inputString);
		transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower);
		if (inputPredicate == "s")
		{
			alg_.filter([&](TerritorialUnit* unit)  -> bool
				{
					if (unit->getType() == TypeOfTerritorialUnit::okres)
					{
						tempTitle = unit->getMediumTitle();
					}
					else {
						tempTitle = unit->getOfficialTitle();
					}
					transform(tempTitle.begin(), tempTitle.end(), tempTitle.begin(), ::tolower);
					return tempTitle.find(inputString) == 0;

				}, [&](TerritorialUnit* unit) -> void
				{
					newData.insertLast().data_ = unit;

				}, dataPointer->begin(), dataPointer->end());
		}
		else if (inputPredicate == "c")
		{
			alg_.filter([&](TerritorialUnit* unit)  -> bool
				{
					if (unit->getType() == TypeOfTerritorialUnit::okres)
					{
						tempTitle = unit->getMediumTitle();
					}
					else {
						tempTitle = unit->getOfficialTitle();
					}
					transform(tempTitle.begin(), tempTitle.end(), tempTitle.begin(), ::tolower);
					return tempTitle.find(inputString) != std::string::npos;

				}, [&](TerritorialUnit* unit) -> void
				{
					newData.insertLast().data_ = unit;

				}, dataPointer->begin(), dataPointer->end());
		}
		else
		{
			cout << "Zadali ste zlý predikát." << endl << endl;
			continue;
		}
		cout << endl << "Filtrované dáta:" << endl;
		this->print(newData);

		newData.clear();
	}
}

void SequencesOfTerritorialUnits::print(ImplicitSequence<TerritorialUnit*>& pVector)
{
	if (pVector.size() == 0)
	{
		cout << "Nenašli sa žiadne dáta." << endl;
	}
	else {
		for (size_t i = 0; i < pVector.size(); i++)
		{
			pVector.access(i)->data_->writeInfo();
		}
	}
}

SequencesOfTerritorialUnits::~SequencesOfTerritorialUnits()
{
	for (size_t i = 0; i < kraje_.size(); i++)
	{
		delete this->kraje_.access(i)->data_;
	}
	for (size_t i = 0; i < okresy_.size(); i++)
	{
		delete this->okresy_.access(i)->data_;
	}
	for (size_t i = 0; i < obce_.size(); i++)
	{
		delete this->obce_.access(i)->data_;
	}
	delete this->reader_;
}
