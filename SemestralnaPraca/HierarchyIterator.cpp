#include "HierarchyIterator.h"

HierarchyIterator::HierarchyIterator(MultiWayExplicitHierarchy<TerritorialUnit*>& pHierarchy)
{
	this->hierarchyPointer_ = &pHierarchy;
	this->actualPosition_ = this->hierarchyPointer_->accessRoot();
}

void HierarchyIterator::goToTheParent()
{
	if (this->actualPosition_->parent_ != nullptr)
	{
		this->actualPosition_ = this->hierarchyPointer_->accessParent(*actualPosition_);
	}
}

void HierarchyIterator::goToTheSon(const int index)
{
	if (this->actualPosition_->sons_ != nullptr)
	{
		if (index < this->hierarchyPointer_->degree(*actualPosition_))
		{
			this->actualPosition_ = this->hierarchyPointer_->accessSon(*actualPosition_, index);
		}
	}
}

void HierarchyIterator::getInfo()
{
	cout << "\t";
	this->actualPosition_->data_->writeInfo();
	cout << endl;
	if (this->actualPosition_->parent_ != nullptr) {
		cout << "Môžeš sa presunú na otca: >>> ";
		this->actualPosition_->parent_->data_->writeInfo();
		cout << endl;
	}
	else {
		cout << "Si v koreni hierarchie, na otca sa presunú nemôžeš." << endl << endl;
	}
	if (this->actualPosition_->sons_ != nullptr)
	{
		cout << "Môžeš sa presunú na nasledovných synov:" << endl;
		for (size_t i = 0; i < this->hierarchyPointer_->degree(*actualPosition_); i++)
		{
			cout << "\t Index syna: [" << i << "] >>> ";
			this->actualPosition_->sons_->access(i)->data_->data_->writeInfo();
		}
	}
	else {
		cout << "Si na liste hierarchie, na synov sa presunú nemôžeš." << endl << endl;
	}
}

MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator HierarchyIterator::getIterator()
{
	MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator it(this->hierarchyPointer_, this->actualPosition_);
	return it;
}
