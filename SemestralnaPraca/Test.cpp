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
		cout << "Vyberte si prosím, štruktúru s ktorou chcete pracova: \t [ hierarchia / tabulky / exit ]" << endl;
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
			cout << "Zadali ste nesprávnu operáciu, prosím opravte sa." << endl;
		}
	}
	delete tables;
	delete hierarchy;
}