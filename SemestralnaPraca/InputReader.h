#pragma once
#include <libds/heap_monitor.h>
#include <fstream>
#include <iostream>
#include "TerritorialUnit.h"
#include "libds/amt/implicit_sequence.h"

using namespace ds::amt;

class InputReader
{
private:
	ifstream inputFile_;
	string nameOfFile_;
	string populationLine;
	TerritorialUnit* tempUnit;
	bool firstLine_;
public:
	InputReader(const string& pName);
	void changeFile(const string& pName);
	TerritorialUnit* read();
	string& readPopulation();
	~InputReader();
};

