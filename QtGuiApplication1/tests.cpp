#pragma once
#include "tests.h"

void tests::run_tests()
{
	vector<Drug> data;
	Drug drug;
	repository main_repository;
	validation main_validation;
	service main_service(main_repository, main_validation);

	data = main_service.print_drugs();
	assert(data.size() == 0);
	try
	{
		main_service.recipe_generate(10);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}

	Drug drug1{"name1", "producer1", "as1", 2};
	Drug drug2{"name2", "producer2", "as2", 2};
	assert(!drug1.compare(drug1, drug2));
	main_service.add_drug(drug1);
	try
	{
		main_service.add_drug(drug1);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}
	assert(main_service.get_len() == 1);

	try
	{
		drug = main_service.find_drug(drug2);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}

	try
	{
		drug = main_service.find_name("name1");
		assert(true);
	}
	catch (ServiceException)
	{
		assert(false);
	}
	try
	{
		drug = main_service.find_name("name3");
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}
	try
	{
		drug = main_service.find_drug(drug1);
		assert(drug.get_name() == drug1.get_name());
		assert(drug.get_producer() == drug1.get_producer());
		assert(drug.get_active_substance() == drug1.get_active_substance());
		assert(drug.get_price() == drug1.get_price());
	}
	catch (ServiceException)
	{
		assert(false);
	}

	try
	{
		main_service.change_drug(drug1, drug2);
		assert(true);
	}
	catch (ServiceException)
	{
		assert(false);
	}
	try
	{
		main_service.change_drug(drug1, drug2);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}

	try
	{
		main_service.change_drug(drug2, drug2);
		assert(false);
	}
	catch (ServiceException)
	{
		drug2 = Drug{"name2", "producer2", "as2", 2};
		assert(true);
	}

	try
	{
		main_service.remove_drug(drug2);
		assert(true);
	}
	catch (ServiceException)
	{
		assert(false);
	}
	try
	{
		main_service.remove_drug(drug2);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}
	assert(main_service.get_len() == 0);

	Drug drug3{"name2", "producer2", "as6", 1};
	Drug drug4{"name3", "producer3", "as2", 6};
	Drug drug5{"name1", "producer1", "as2", 2};
	vector<Drug> data2, data3, data4, data5, data6;

	main_service.add_drug(drug3);
	main_service.add_drug(drug4);
	main_service.add_drug(drug5);
	assert(main_service.get_len() == 3);

	data2 = main_service.filter_by_active_substance("as3");
	assert(data2.size() == 0);
	data3 = main_service.filter_by_price(11);
	assert(data3.size() == 0);

	data2 = main_service.filter_by_active_substance("as2");
	assert(data2.size() == 2);
	data3 = main_service.filter_by_price(2);
	assert(data3.size() == 1);

	data4 = main_service.sort("by_name");
	assert((data4.at(0)).get_name() == drug5.get_name());
	assert((data4.at(0)).get_producer() == drug5.get_producer());
	assert((data4.at(0)).get_active_substance() == drug5.get_active_substance());
	assert((data4.at(0)).get_price() == drug5.get_price());

	data5 = main_service.sort("by_producer");
	assert((data5.at(0)).get_name() == drug5.get_name());
	assert((data5.at(0)).get_producer() == drug5.get_producer());
	assert((data5.at(0)).get_active_substance() == drug5.get_active_substance());
	assert((data5.at(0)).get_price() == drug5.get_price());

	data6 = main_service.sort("by_as_price");
	assert((data6.at(2)).get_name() == drug3.get_name());
	assert((data6.at(2)).get_producer() == drug3.get_producer());
	assert((data6.at(2)).get_active_substance() == drug3.get_active_substance());
	assert((data6.at(2)).get_price() == drug3.get_price());

	assert(by_name(drug5, drug3) == true);
	assert(by_name(drug5, drug5) == false);
	assert(by_producer(drug5, drug3) == true);
	assert(by_producer(drug5, drug5) == false);

	assert(by_as_price(drug5, drug5) == false);
	assert(by_as_price(drug5, drug3) == true);
	assert(by_as_price(drug5, drug4) == true);

	Drug drug0{"", "", "", -1};
	try
	{
		main_service.remove_drug(drug0);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}
	try
	{
		main_service.add_drug(drug0);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}
	try
	{
		main_service.change_drug(drug0, drug0);
		assert(false);
	}
	catch (ServiceException)
	{
		assert(true);
	}

	drug3.set_name("Teste");
	drug3.set_producer("trecute");
	drug3.set_active_substance("cu");
	drug3.set_price(~0);

	stringstream test;
	test << drug3;
	assert(test.str() == "Teste trecute cu -1\n");
	try
	{
		main_service.change_drug(drug0, drug0);
		assert(false);
	}
	catch (ServiceException const &msg)
	{
		test.str("");
		test << msg;
		assert(test.str() == "Drug isnt valid.\n");
	}

	assert(main_service.recipe_length() == 0);
	assert(main_service.get_recipe() == vector<Drug>());
	main_service.recipe_generate(10);
	assert(main_service.recipe_length() == 10);
	main_service.recipe_add(drug3);
	assert(main_service.recipe_length() == 11);
	main_service.recipe_clear();
	main_service.recipe_save_to_HTML("test.HTML");
	test_undo();
}

void tests::test_undo()
{
	repository main_repository;
	validation main_validation;
	service main_service(main_repository, main_validation);
	vector<Drug> data;
	Drug drug1{"n1","p1","as1",1};
	Drug drug2{"n2","p2","as2",2};
	main_service.add_drug(drug1);
	assert(main_service.get_len() == 1);
	main_service.undo();
	assert(main_service.get_len() == 0);
	try
	{
		main_service.undo();
		assert(false);
	}
	catch (...)
	{
		assert(true);
	}
	main_service.add_drug(drug1);
	main_service.add_drug(drug2);
	assert(main_service.get_len() == 2);
	main_service.remove_drug(drug2);
	assert(main_service.get_len() == 1);
	main_service.change_drug(drug1, drug2);
	assert(main_service.get_len() == 1);
	main_service.undo();
	assert(main_service.get_len() == 1);
	main_service.undo();
	assert(main_service.get_len() == 2);
	main_service.undo();
	assert(main_service.get_len() == 1);
	main_service.undo();
	assert(main_service.get_len() == 0);
}

