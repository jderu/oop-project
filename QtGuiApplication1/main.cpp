#pragma once

#include "MainWindow.h"
#include "ui.h"
#include "tests.h"
#include <vector>
#include <random>

using namespace std;

void add_drugs(repository &main_repository)
{
	string name = "name";
	string producer = "producer";
	string active_substance = "active_substance";
	int n = 1000000;
	for (int i = 0; i < n; i++)
	{
		Drug drug{name + to_string(rand() % n) , producer + to_string(rand() % n) , active_substance + to_string(rand() % n) , rand() % (5*n)};
		main_repository.add(drug);
	}
}

int main(int argc, char *argv[])
{
	bool const GUI = true;
	bool const testing = true;
	bool const interfacing = true;

	if (GUI)
	{
		QApplication app(argc, argv);
		repository main_repository;

		add_drugs(main_repository);

		validation main_validation;
		service main_service(main_repository, main_validation);
		MainWindow w(main_service);
		w.show();
		return app.exec();
	}
	else
	{
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
		if (testing)
		{
			tests test;
			test.run_tests();
		}

		if (interfacing)
		{
			repository main_repository;
			validation main_validation;
			service main_service(main_repository, main_validation);
			ui interface(main_service);
			interface.run();
		}
		_CrtDumpMemoryLeaks();
		return 0;
	}
}
