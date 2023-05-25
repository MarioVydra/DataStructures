#pragma once
#include <libds/heap_monitor.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>
#include <libds/adt/list.h>
#include "TerritorialUnit.h"
#include "Algorithm.h"
#include "InputReader.h"

using namespace ds::amt;
using namespace ds::adt;

class TablesOfTerritorialUnits
{
private:
	MultiWayExplicitHierarchy<TerritorialUnit*>* hierarchyPointer_;
	HashTable<string, ImplicitList<TerritorialUnit*>*> obce_;
	HashTable<string, ImplicitList<TerritorialUnit*>*> okresy_;
	HashTable<string, ImplicitList<TerritorialUnit*>*> kraje_;
	Algorithm<TableItem<string, ImplicitList<TerritorialUnit*>*>, HashTable<string, ImplicitList<TerritorialUnit*>*>::IteratorType> alg_;

public:
	TablesOfTerritorialUnits(MultiWayExplicitHierarchy<TerritorialUnit*>& pHierarchy);
	void loadTables();
	void loadPopulation();
	void start();
	~TablesOfTerritorialUnits();
};

