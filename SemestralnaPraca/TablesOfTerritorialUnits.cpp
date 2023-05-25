#include "TablesOfTerritorialUnits.h"

TablesOfTerritorialUnits::TablesOfTerritorialUnits(MultiWayExplicitHierarchy<TerritorialUnit*>& pHierarchy)
{
	this->hierarchyPointer_ = &pHierarchy;
}

void TablesOfTerritorialUnits::loadTables()
{
	ImplicitList<TerritorialUnit*>* list = nullptr;
	this->hierarchyPointer_->processLevelOrder(this->hierarchyPointer_->accessRoot(), [&](MultiWayExplicitHierarchyBlock<TerritorialUnit*>* block)
	{ 
		if (block->data_->getType() == TypeOfTerritorialUnit::kraj)
		{
			if (this->kraje_.contains(block->data_->getOfficialTitle()))
			{
				this->kraje_.find(block->data_->getOfficialTitle())->insertLast(block->data_);
			}
			else {
				list = new ImplicitList<TerritorialUnit*>();
				list->insertLast(block->data_);
				this->kraje_.insert(block->data_->getOfficialTitle(), list);
			}
		}
		else if (block->data_->getType() == TypeOfTerritorialUnit::okres)
		{
			if (this->okresy_.contains(block->data_->getOfficialTitle()))
			{
				this->okresy_.find(block->data_->getOfficialTitle())->insertLast(block->data_);
			}
			else {
				list = new ImplicitList<TerritorialUnit*>();
				list->insertLast(block->data_);
				this->okresy_.insert(block->data_->getOfficialTitle(), list);
			}
		} else {
			if (this->obce_.contains(block->data_->getOfficialTitle()))
			{
				this->obce_.find(block->data_->getOfficialTitle())->insertLast(block->data_);
			}
			else {
				list = new ImplicitList<TerritorialUnit*>();
				list->insertLast(block->data_);
				this->obce_.insert(block->data_->getOfficialTitle(), list);
			}
		}
	});
	list == nullptr;
}


void TablesOfTerritorialUnits::loadPopulation()
{
	HashTable<string, TerritorialUnit*> tempTable;
	TerritorialUnit** tempUnit_pointer;
	InputReader* readerOfPopulation = new InputReader("obyvatelstvo.csv");
	string line, noteObce, noteKraju;
	int population;

	this->hierarchyPointer_->processLevelOrder(this->hierarchyPointer_->accessRoot(), [&](MultiWayExplicitHierarchyBlock<TerritorialUnit*>* block)
		{
			if (block->data_->getType() == TypeOfTerritorialUnit::obec && block->data_->getNote() != "")
			{		
				tempTable.insert(block->data_->getNote(), block->data_);
			}
		});

	line = readerOfPopulation->readPopulation();
	do {
		noteKraju = line.substr(0, line.find("_"));
		noteObce = line.substr(0, line.find(";"));
		noteObce = noteObce.substr(noteObce.find("_") + 1, noteObce.length());
		population = stoi(line.substr(line.find(";") + 1, line.length()));

		if (tempTable.tryFind(noteObce, tempUnit_pointer))
		{
			(*tempUnit_pointer)->addPopulation(population);
		}
		line = readerOfPopulation->readPopulation();
	} while (line != "");

	tempTable.clear();
	tempUnit_pointer = nullptr;
	delete readerOfPopulation;
}


void TablesOfTerritorialUnits::start()
{
	string inputTerritorialUnit, inputString, dataType;
	HashTable<string, ImplicitList<TerritorialUnit*>*>* dataPointer;
	ImplicitList<TerritorialUnit*>** list_pointer = nullptr;
	while (true)
	{
		std::cout << endl <<  "--------------------------------------------------------------------------------------------------------" << endl;
		std::cout << "Zvo¾te prosím územné jednotky, s ktorými chcete pracova: \t [ kraje / okresy / obce / exit ]" << endl;
		std::getline(cin, inputTerritorialUnit);
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
			std::cout << "Nezadali ste správnu operáciu, prosím opravte sa." << endl << endl;
			continue;
		}
		std::cout << "Zadajte prosím k¾úè: " << endl;
		std::getline(cin, inputString);

		if (dataPointer->tryFind(inputString, list_pointer))
		{
			std::cout << endl << "Nájdené dáta:" << endl;
			for (size_t i = 0; i < (*list_pointer)->size(); i++)
			{
				std::cout << "\t";
				(*list_pointer)->access(i)->writeInfo();
			}
			list_pointer = nullptr;
		}
		else {
			std::cout << endl << "Nenašli sa žiadne dáta so zadaným k¾úèom." << endl;
		}
	}
}

TablesOfTerritorialUnits::~TablesOfTerritorialUnits()
{
	hierarchyPointer_ == nullptr;
	alg_.filter([&](TableItem<string, ImplicitList<TerritorialUnit*>*> unit)  -> bool
		{
			return true;

		}, [&](TableItem<string, ImplicitList<TerritorialUnit*>*> unit) -> void
		{
			for (size_t i = 0; i < unit.data_->size(); i++)
			{
				unit.data_->access(i) == nullptr;
			}
			delete unit.data_;

		}, this->kraje_.begin(), this->kraje_.end());

	alg_.filter([&](TableItem<string, ImplicitList<TerritorialUnit*>*> unit)  -> bool
		{
			return true;

		}, [&](TableItem<string, ImplicitList<TerritorialUnit*>*> unit) -> void
		{
			for (size_t i = 0; i < unit.data_->size(); i++)
			{
				unit.data_->access(i) == nullptr;
			}
			delete unit.data_;

		}, this->okresy_.begin(), this->okresy_.end());

	alg_.filter([&](TableItem<string, ImplicitList<TerritorialUnit*>*> unit)  -> bool
		{
			return true;

		}, [&](TableItem<string, ImplicitList<TerritorialUnit*>*> unit) -> void
		{
			for (size_t i = 0; i < unit.data_->size(); i++)
			{
				unit.data_->access(i) == nullptr;
			}
			delete unit.data_;

		}, this->obce_.begin(), this->obce_.end());
}

