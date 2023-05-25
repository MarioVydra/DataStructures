#include "InputReader.h"

InputReader::InputReader(const string& pName)
{
    this->inputFile_ = ifstream(pName);
    this->firstLine_ = true;
    this->nameOfFile_ = pName;
}

void InputReader::changeFile(const string& pName)
{

    this->inputFile_ = ifstream(pName);
    this->firstLine_ = true;
    this->nameOfFile_ = pName;
}

TerritorialUnit* InputReader::read()
{
   string line, code, officialTitle, shortTitle, mediumTitle, note, sortNumber;
   TypeOfTerritorialUnit type;
    if (!this->inputFile_.is_open())
    {
        cerr << "Failed to open file.\n";
    }
    else {
        if (this->firstLine_)
        {
            getline(this->inputFile_, line);
            this->firstLine_ = false;
        }
        getline(this->inputFile_, line);
        if (line == "") 
        { 
            return nullptr; 
        }
        stringstream stringStream(line);
        getline(getline(getline(getline(getline(getline(stringStream, sortNumber, ';'), code, ';'), officialTitle, ';'), mediumTitle, ';'), shortTitle, ';'), note, ';');
        if (this->nameOfFile_ == "kraje.csv")
        {
            type = TypeOfTerritorialUnit::kraj;
        }
        else if (this->nameOfFile_ == "okresy.csv")
        {
            type = TypeOfTerritorialUnit::okres;
        }
        else {
            type = TypeOfTerritorialUnit::obec;
        }
        tempUnit = new TerritorialUnit(stoi(sortNumber), code, officialTitle, mediumTitle, shortTitle, note, type);
        return tempUnit;
    }
}

string& InputReader::readPopulation()
{
    if (!this->inputFile_.is_open())
    {
        cerr << "Failed to open file.\n";
    }
    else {
        getline(this->inputFile_, this->populationLine);
        return this->populationLine;
    }
}

InputReader::~InputReader()
{
    this->inputFile_.close();
}
