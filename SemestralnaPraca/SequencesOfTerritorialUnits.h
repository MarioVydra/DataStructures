#pragma once
#include <libds/heap_monitor.h>
#include <fstream>
#include <iostream>
#include "TerritorialUnit.h"
#include "libds/amt/implicit_sequence.h"
#include "Algorithm.h"
#include "InputReader.h"

using namespace ds::amt;

class SequencesOfTerritorialUnits
{
private:
	ImplicitSequence<TerritorialUnit*> kraje_;
	ImplicitSequence<TerritorialUnit*> obce_;
	ImplicitSequence<TerritorialUnit*> okresy_;
	Algorithm<TerritorialUnit*, ImplicitSequence<TerritorialUnit*>::IteratorType> alg_;
	InputReader* reader_ = nullptr;
public:
	SequencesOfTerritorialUnits();
	void loadSequences();
	void start();
	void print(ImplicitSequence<TerritorialUnit*>& pVector);
	~SequencesOfTerritorialUnits();
	
};

