#pragma once
#include "ui.h"

ui::ui(service service)
{
	ui_service = service;
}

void ui::run()
{
	int command = -2;
	print_menu();
	while (1)
	{
		cout << "Enter command:\n";
		command = -2;
		read_int(command);
		if (command == 0)
			break;
		switch (command)
		{
			case -1:
				print_menu();
				break;
			case 1:
				ui_add_drug();
				break;
			case 2:
				ui_remove_drug();
				break;
			case 3:
				ui_change_drug();
				break;
			case 4:
				ui_print_drugs();
				break;
			case 5:
				ui_find_drug();
				break;
			case 6:
				ui_filter();
				break;
			case 7:
				ui_sort();
				break;
			case 8:
				ui_recipe();
				print_menu();
				break;
			case 9:
				ui_undo();
				break;
			default:
				cout << "Inexistent command.\n";
				break;
		}
	}
}

void ui::print_menu() noexcept
{
	printf(
		"-1 - Swow menu\n"
		"0  - Exit\n"
		"1  - Add drug\n"
		"2  - Remove drug\n"
		"3  - Change drug\n"
		"4  - Print drugs\n"
		"5  - Find drug\n"
		"6  - Filter drugs\n"
		"7  - Sort drugs\n"
		"8  - Recipe menu\n"
		"9  - Undo\n"
	);
}

void ui::ui_find_drug()
{
	string name, producer;
	cout << "Enter the name of the drug:\n";
	getline(cin, name);
	cout << "Enter the producer of the drug:\n";
	getline(cin, producer);
	Drug drug(name, producer, "", 0);
	try
	{
		const Drug found = ui_service.find_drug(drug);
		cout << found;
	}
	catch (ServiceException const&msg)
	{
		cout << msg;
	}
}

void ui::ui_filter()
{
	int option = 0, price = 0;
	string active_substance;
	vector<Drug> temporary;
	while (1)
	{
		printf(
			"1 - Filter by price\n"
			"2 - Filter by active substance\n"
		);
		cout << "Enter the option.\n";
		if (read_int(option))
			break;
		else
			cout << "Invalid command.\n";
	}
	switch (option)
	{
		case 1:
			while (1)
			{
				cout << "Enter the searched price:\n";
				if (read_int(price))
					break;
				else
					cout << "Price must be a number.\n";
			}
			temporary = ui_service.filter_by_price(price);
			break;
		case 2:
			cout << "Enter the searched active substance:\n";
			getline(cin, active_substance);
			temporary = ui_service.filter_by_active_substance(active_substance);
			break;
		default:
			cout << "Invalid command\n";
			break;
	}
	for (unsigned i = 0; i < temporary.size(); i++)
		cout << i << ": " << temporary.at(i);
	if (temporary.size() == 0)
		cout << "Nothing found.\n";
}

void ui::ui_sort()
{
	int option = -1;
	vector<Drug> temporary;
	printf(
		"1 - Sort by name\n"
		"2 - Sort by producer\n"
		"3 - Sort by active substance and price\n"
	);
	while (1)
	{
		cout << "Enter the option.\n";
		if (read_int(option))
			break;
		else
			cout << "Invalid command.\n";
	}
	switch (option)
	{
		case 1:
			temporary = ui_service.sort("by_name");
			break;
		case 2:
			temporary = ui_service.sort("by_producer");
			break;
		case 3:
			temporary = ui_service.sort("by_as_price");
			break;
		default:
			cout << "Invalid command\n";
			break;
	}
	for (int i = 0; i < temporary.size(); i++)
		cout << i << ": " << temporary.at(i);
	if (temporary.size() == 0)
		cout << "The database is empty.\n";
}

void ui::print_recipe_menu() noexcept
{
	printf(
		"-1 - Print recipe menu.\n"
		"0  - Exit.\n"
		"1  - Add a new drug to the recipe.\n"
		"2  - Clear the recipe.\n"
		"3  - Generate a recipe.\n"
		"4  - Save the recipe to a HTML file.\n"
	);
}

void ui::ui_recipe()
{
	int command = -2;
	print_recipe_menu();
	ui_service.recipe_clear();
	while (1)
	{
		cout << "Enter command:\n";
		command = -2;
		read_int(command);
		if (command == 0)
			break;
		switch (command)
		{
			case -1:
				print_recipe_menu();
				break;
			case 1:
				ui_recipe_add();
				break;
			case 2:
				ui_recipe_clear();
				break;
			case 3:
				ui_recipe_generate();
				break;
			case 4:
				ui_recipe_save_to_HTML();
				break;
			default:
				cout << "Inexistent recipe command.\n";
				break;
		}
	}
}

void ui::ui_recipe_add()
{
	string name;
	Drug found;
	cout << "Enter the name of the drug:\n";
	getline(cin, name);
	try
	{
		found = ui_service.find_name(name);
		ui_service.recipe_add(found);
		cout << "The recipe has: " << ui_service.recipe_length() << " drugs.\n";
	}
	catch (ServiceException const &msg)
	{
		cout << msg;
	}
}

void ui::ui_recipe_clear()
{
	ui_service.recipe_clear();
	cout << "The recipe has: " << ui_service.recipe_length() << " drugs.\n";
}

void ui::ui_recipe_generate()
{
	ui_service.recipe_clear();
	int number = 0;
	while (1)
	{
		cout << "Enter the number of drugs you want your recipe to contain:\n";
		if (read_int(number))
			break;
		else
			cout << "Must be a number.\n";
	}
	try
	{
		ui_service.recipe_generate(number);
		cout << "The recipe has: " << ui_service.recipe_length() << " drugs.\n";
	}
	catch (ServiceException const &msg)
	{
		cout << msg;
	}
}

void ui::ui_recipe_save_to_HTML()
{
	ui_service.recipe_save_to_HTML("recipe.HTML");
	cout << "Recipe saved to \"recipe.HTML\".\n";
}

void ui::ui_undo()
{
	try
	{
		ui_service.undo();
	}
	catch (ServiceException const &msg)
	{
		cout << msg;
	}
}

void ui::ui_print_drugs()
{
	vector<Drug> data = ui_service.print_drugs();
	for (int i = 0; i < data.size(); i++)
		cout << i << ": " << data.at(i);
	if (data.size() == 0)
		cout << "The database is empty.\n";
}

void ui::ui_add_drug()
{
	string name, producer, active_substance;
	int price = -1;
	cout << "Enter the name of the drug:\n";
	getline(cin, name);
	cout << "Enter the producer of the drug:\n";
	getline(cin, producer);
	cout << "Enter the active substance of the drug:\n";
	getline(cin, active_substance);
	while (1)
	{
		cout << "Enter the price of the drug:\n";
		if (read_int(price))
			break;
		else
			cout << "Price must be a number.\n";
	}
	Drug drug{name, producer, active_substance, price};
	try
	{
		ui_service.add_drug(drug);
	}
	catch (ServiceException const &msg)
	{
		cout << msg;
	}
}

void ui::ui_remove_drug()
{
	string name, producer;
	cout << "Enter the name of the drug:\n";
	getline(cin, name);
	cout << "Enter the producer of the drug:\n";
	getline(cin, producer);
	Drug drug(name, producer, "1", 1);
	try
	{
		ui_service.remove_drug(drug);
	}
	catch (ServiceException const &msg)
	{
		cout << msg;
	}
}

void ui::ui_change_drug()
{
	string nume1, producator1, nume2, producator2, active_substance;
	int price = -1;
	cout << "Enter the name of the drug pe care doriti sa il modificati:\n";
	getline(cin, nume1);
	cout << "Enter the producer of the drug pe care doriti sa il modificati:\n";
	getline(cin, producator1);
	cout << "Enter the new name of the drug :\n";
	getline(cin, nume2);
	cout << "Enter the new producer of the drug:\n";
	getline(cin, producator2);
	cout << "Enter the new active substance of the drug:\n";
	getline(cin, active_substance);
	while (1)
	{
		cout << "Enter the new price of the drug:\n";
		if (read_int(price))
			break;
		else
			cout << "Price must be a number.\n";
	}
	Drug drug1{nume1, producator1, "", 0};
	Drug drug2{nume2, producator2, active_substance, price};
	try
	{
		ui_service.change_drug(drug1, drug2);
	}
	catch (ServiceException const &msg)
	{
		cout << msg;
	}
}

