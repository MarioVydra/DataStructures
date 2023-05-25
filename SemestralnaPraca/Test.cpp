#include "Test.h"

void Test::start()
{
	HierarchyOfTerritorialUnits* hierarchy = new HierarchyOfTerritorialUnits();
	hierarchy->loadHierarchy();
	TablesOfTerritorialUnits* tables = new TablesOfTerritorialUnits(hierarchy->getHierarchy());
	tables->loadTables();
	tables->loadPopulation();
	hierarchy->countPopulation();

	string input;
	while (true)
	{
		cout << endl << "--------------------------------------------------------------------------------------------------------" << endl;
		cout << "Vyberte si pros�m, �trukt�ru s ktorou chcete pracova�: \t [ hierarchia / tabulky / exit ]" << endl;
		getline(cin, input);

		if (input == "hierarchia") {
			hierarchy->start();
		}
		else if (input == "tabulky") {
			tables->start();
		}
		else if (input == "exit")
		{
			break;
		}
		else {
			cout << "Zadali ste nespr�vnu oper�ciu, pros�m opravte sa." << endl;
		}
	}
	delete tables;
	delete hierarchy;
}