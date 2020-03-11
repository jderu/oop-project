#pragma once
#include <iostream>
#include "service.h"
#include "utility.h"

class ui
{
private:
	service ui_service;
public:
	//constructor
	ui(service service);

	//starts the ui
	void run();

	void print_menu() noexcept;

	void ui_print_drugs();

	void ui_add_drug();

	void ui_remove_drug();

	void ui_change_drug();

	void ui_find_drug();

	void ui_filter();

	void ui_sort();

	void print_recipe_menu() noexcept;

	void ui_recipe();

	void ui_recipe_add();

	void ui_recipe_clear();

	void ui_recipe_generate();

	void ui_recipe_save_to_HTML();

	void ui_undo();
};

