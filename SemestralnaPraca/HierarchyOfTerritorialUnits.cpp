#include "HierarchyOfTerritorialUnits.h"

void HierarchyOfTerritorialUnits::loadHierarchy()
{
	InputReader* readerKrajov = new InputReader("kraje.csv");
	InputReader* readerOkresov = new InputReader("okresy.csv");
	InputReader* readerObci = new InputReader("obce.csv");
	TerritorialUnit* tempKraj = nullptr;
	TerritorialUnit* tempOkres = readerOkresov->read();
	TerritorialUnit* tempObec = readerObci->read();
	MultiWayExplicitHierarchyBlock<TerritorialUnit*>* kraj_pointer;
	MultiWayExplicitHierarchyBlock<TerritorialUnit*>* okres_pointer;
	int i = 0, j = 0, k = 0;
	string tempString;

	this->slovakia_.emplaceRoot().data_ = new TerritorialUnit(0, "0", "Slovenská Republika", "Slovensko", "Slovensko", "", TypeOfTerritorialUnit::krajina);
	
	while ((tempKraj = readerKrajov->read()) != nullptr)
	{
		this->slovakia_.emplaceSon(*this->slovakia_.accessRoot(), i).data_ = tempKraj;
		kraj_pointer = this->slovakia_.accessSon(*this->slovakia_.accessRoot(), i);
		j = 0;
		while (tempOkres != nullptr)
		{
			if (tempOkres->getNote().find(tempKraj->getCode()) == 0 || (i == 8 && tempOkres->getCode().find("SKZZZ") == 0))
			{
				this->slovakia_.emplaceSon(*kraj_pointer, j).data_ = tempOkres;
				okres_pointer = this->slovakia_.accessSon(*kraj_pointer, j);
			} else { break;	}

			k = 0;
			while (tempObec != nullptr)
			{
				tempString = tempObec->getCode().substr(0, 6);
				if (tempString == tempOkres->getCode() || (i == 8 && tempString.find("SKZZZ") == 0))
				{
					this->slovakia_.emplaceSon(*okres_pointer, k).data_ = tempObec;
				} else { break; }
				k++;
				tempObec = readerObci->read();
			}
			j++;
			tempOkres = readerOkresov->read();
		}
		i++;
	}

	delete readerKrajov;
	delete readerOkresov;
	delete readerObci;
	tempKraj = nullptr;
	tempObec = nullptr;
	tempOkres = nullptr;
	okres_pointer = nullptr;
	kraj_pointer = nullptr;
}

void HierarchyOfTerritorialUnits::countPopulation()
{
	this->slovakia_.processPostOrder(this->slovakia_.accessRoot(), [&](MultiWayExplicitHierarchyBlock<TerritorialUnit*>* block) {
		
		if (block->sons_ != nullptr)
		{
			for (size_t i = 0; i < block->sons_->size(); i++)
			{
				block->data_->addPopulation(block->sons_->access(i)->data_->data_->getPopulation());
			}
		}
	});
}

void HierarchyOfTerritorialUnits::start()
{
	string inputOperation, inputString, indexOfSon, tempTitle, inputComparator, tempString, alphabetOrder, title1, title2;
	char letter1, letter2;
	alphabetOrder = "AÁÄBCÈDÏEÉFGHIÍJKLÅ¼MNÒOÓÔPQRÀSŠTUÚVWXYÝZŽ";
	int sizeOfUnitCode, alphabetPosition1, alphabetPosition2, population;
	auto actualPosition = this->slovakia_.accessRoot();
	HierarchyIterator iterator(slovakia_);
	ImplicitSequence<TerritorialUnit*> sequence;

	while (true)
	{
		std::cout << endl <<"--------------------------------------------------------------------------------------------------------" << endl;
		std::cout << endl << "Zadaj operáciu [ son / parent / [s]tartsWithStr / [c]ontainsStr / [h]asType / [m]orePopulation / [l]essPopulation / exit ]" << endl << endl;
		std::cout << "Aktuálne sa nachádzaš na: " << endl;
		iterator.getInfo();

		getline(cin, inputOperation);
		if (inputOperation == "exit")
		{
			break;
		}
		else if (inputOperation == "parent")
		{
			iterator.goToTheParent();
		}
		else if (inputOperation == "son")
		{
			try {
				std::cout << "Zadajte index syna, na ktorý chcete ís:" << endl;
				getline(cin, indexOfSon);
				iterator.goToTheSon(stoi(indexOfSon));
			}
			catch (std::exception& e) {
				std::cout << endl << "Nezadali ste èíslo." << endl;
			}	
		}
		else if (inputOperation == "s")
		{
			std::cout << "Zadajte string:" << endl;
			getline(cin, inputString);
			if (!inputString.empty())
			{
				transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower);
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
						sequence.insertLast().data_ = unit;

					}, iterator.getIterator(), this->slovakia_.end());
			}
		}
		else if (inputOperation == "c")
		{
			std::cout << "Zadajte string:" << endl;
			getline(cin, inputString);
			if (!inputString.empty())
			{
				transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower);
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
						sequence.insertLast().data_ = unit;

					}, iterator.getIterator(), this->slovakia_.end());
			}
		}
		else if (inputOperation == "h")
		{
			std::cout << "Zadajte typ územnej jednotky: \t [ kraj / okres / obec ]" << endl;
			getline(cin, inputString);
			if (!inputString.empty())
			{
				alg_.filter([&](TerritorialUnit* unit)  -> bool
					{
						if (inputString == "kraj")
						{
							return unit->getType() == TypeOfTerritorialUnit::kraj;
						}
						else if (inputString == "okres")
						{
							return unit->getType() == TypeOfTerritorialUnit::okres;
						}
						else if (inputString == "obec")
						{
							return unit->getType() == TypeOfTerritorialUnit::obec;
						}
						else {
							return false;
						}

					}, [&](TerritorialUnit* unit) -> void
					{
						sequence.insertLast().data_ = unit;

					}, iterator.getIterator(), this->slovakia_.end());
			}
		}
		else if (inputOperation == "m")
		{
			try {
				cout << "Zadajte minimálny poèet obyvate¾ov." << endl;
				getline(cin, inputString);
				population = stoi(inputString);
			}
			catch (std::exception& e) {
				std::cout << endl << "Nezadali ste èíslo." << endl;
				continue;
			}

			alg_.filter([&](TerritorialUnit* unit)  -> bool
				{
					return population <= unit->getPopulation();

				}, [&](TerritorialUnit* unit) -> void
				{
					sequence.insertLast().data_ = unit;

				}, iterator.getIterator(), this->slovakia_.end());
		}
		else if (inputOperation == "l")
		{
			try {
				cout << "Zadajte minimálny poèet obyvate¾ov." << endl;
				getline(cin, inputString);
				population = stoi(inputString);
			}
			catch (std::exception& e) {
				std::cout << endl << "Nezadali ste èíslo." << endl;
			}

			alg_.filter([&](TerritorialUnit* unit)  -> bool
				{
					return population >= unit->getPopulation();

				}, [&](TerritorialUnit* unit) -> void
				{
					sequence.insertLast().data_ = unit;

				}, iterator.getIterator(), this->slovakia_.end());
		}
		else
		{
			std::cout << "Zadali ste nesprávnu operáciu. Prosím opravte sa." << endl;
		}

		if (!sequence.isEmpty())
		{
			std::cout << "Filtrované dáta: " << endl;
			for (size_t i = 0; i < sequence.size(); i++)
			{
				std::cout << "\t";
				sequence.access(i)->data_->writeInfo();
			}
		}
		else {
			std::cout << endl << "Nenašli sa žiadne dáta." << endl;
		}

		if (!sequence.isEmpty())
		{
			while (true)
			{
				std::cout << endl << "Na zoradenie môžete použi nasledovné komparátory: \t [ [1] compareAlphabetical / [2] compareVowelsCount / [3] comparePopulation / [4] nesortuj ]" << endl;
				getline(cin, inputComparator);
				if (inputComparator == "1")
				{
					sorter_.sort(sequence, [&](TerritorialUnit* unit1, TerritorialUnit* unit2)  -> bool
						{
							title1 = unit1->getOfficialTitle();
							title2 = unit2->getOfficialTitle();
							
							for (size_t i = 0; i < min(title1.length(), title2.length()); i++)
							{
								letter1 = toupper(title1[i]);
								letter2 = toupper(title2[i]);
								if (letter1 != letter2)
								{
									alphabetPosition1 = alphabetOrder.find(letter1);
									alphabetPosition2 = alphabetOrder.find(letter2);
									if (alphabetPosition1 != std::string::npos && alphabetPosition2 != std::string::npos)
									{
										return alphabetPosition1 < alphabetPosition2;
									}
									else {
										return letter1 < letter2; // ak sa nenachádza v slovenskej abecede, tak zoradí na základe ASCII hodnoty
									}
								}
							}
							return title1.length() < title2.length();
						});
					std::cout << "Filtrované dáta usporiadané abecedne pod¾a officialTitle: " << endl;
					for (size_t i = 0; i < sequence.size(); i++)
					{
						std::cout << "\t";
						sequence.access(i)->data_->writeInfo();
					}
					break;
				}
				else if (inputComparator == "2")
				{
					sorter_.sort(sequence ,[&](TerritorialUnit* unit1, TerritorialUnit* unit2)  -> bool
						{
							return unit1->countVowelsInOfficialTitle() < unit2->countVowelsInOfficialTitle();
						});
					std::cout << "Filtrované dáta usporiadané poïla poètu samohlások v officialTitle: " << endl;
					for (size_t i = 0; i < sequence.size(); i++)
					{
						std::cout << "\t Poèet samohlások: <" << sequence.access(i)->data_->countVowelsInOfficialTitle() << "> - ";
						sequence.access(i)->data_->writeInfo();
					}
					break;
				}
				else if (inputComparator == "3")
				{
					sorter_.sort(sequence, [&](TerritorialUnit* unit1, TerritorialUnit* unit2)  -> bool
						{
							return unit1->getPopulation() > unit2->getPopulation();
						});
					std::cout << "Filtrované dáta usporiadané poïla poètu obyvate¾ov zostupne: " << endl;
					for (size_t i = 0; i < sequence.size(); i++)
					{
						std::cout << "\t";
						sequence.access(i)->data_->writeInfo();
					}
					break;
				}
				else if (inputComparator == "4")
				{
					break;
				}
				else {
					std::cout << "Zadali ste zlý komparátor, prosím opravte sa." << endl;
				}
			}
		}
		sequence.clear();
	}
}

MultiWayExplicitHierarchy<TerritorialUnit*>& HierarchyOfTerritorialUnits::getHierarchy()
{
	return this->slovakia_;
}

HierarchyOfTerritorialUnits::~HierarchyOfTerritorialUnits()
{
	this->slovakia_.processPostOrder(this->slovakia_.accessRoot(), [&](MultiWayExplicitHierarchyBlock<TerritorialUnit*>* block) { delete block->data_; });
}
