#pragma once
#include <libds/heap_monitor.h>
#include <libds/amt/explicit_hierarchy.h>
#include "TerritorialUnit.h"

using namespace ds::amt;

class HierarchyIterator
{
private:
	MultiWayExplicitHierarchyBlock<TerritorialUnit*>* actualPosition_;
	MultiWayExplicitHierarchy<TerritorialUnit*>* hierarchyPointer_;
public:
	HierarchyIterator(MultiWayExplicitHierarchy<TerritorialUnit*>& pHierarchy);
	void goToTheParent();
	void goToTheSon(const int index);
	void getInfo();
	MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator getIterator();
};

