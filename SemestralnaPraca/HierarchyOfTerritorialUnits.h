#pragma once
#include <libds/heap_monitor.h>
#include "libds/amt/explicit_hierarchy.h"
#include "libds/adt/list.h"
#include <libds/adt/sorts.h>
#include "HierarchyIterator.h"
#include "TerritorialUnit.h"
#include "Algorithm.h"
#include "SortingAlgorithm.h"
#include "InputReader.h"

using namespace ds::amt;
using namespace ds::adt;

class HierarchyOfTerritorialUnits
{
private:
	MultiWayExplicitHierarchy<TerritorialUnit*> slovakia_;
	Algorithm<TerritorialUnit*, MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator> alg_;
	QuickSort<TerritorialUnit*> sorter_;
public:
	void loadHierarchy();
	void countPopulation();
	void start();
	MultiWayExplicitHierarchy<TerritorialUnit*>& getHierarchy();
	~HierarchyOfTerritorialUnits();
};

