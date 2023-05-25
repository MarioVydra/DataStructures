#pragma once
#include <libds/heap_monitor.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Windows.h"
#include "Test.h"

int main()
{
	initHeapMonitor();
	SetConsoleOutputCP(1250);
	SetConsoleCP(1250);
	setlocale(LC_ALL, "sk-SK");

	Test* test = new Test();
	test->start();
	delete test;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}