#pragma once
#include "repository.h"
#include "validation.h"
#include <vector>
#include <algorithm>
#include "recipe.h"
#include <fstream>
#include "ActiuneUndo.h"
#include "UndoAdauga.h"
#include "UndoModifica.h"
#include "UndoSterge.h"

using namespace std;

class service
{
private:
	repository service_repository;
	validation service_validation;
	recipe recipe;
	vector<ActiuneUndo*> service_undo;

public:
	//constructor
	service()noexcept = default;

	service(repository ui_repo, validation ui_valid);

	vector<Drug> print_drugs();

	void add_drug(const Drug &drug);

	void remove_drug(const Drug &drug);

	void change_drug(Drug const &drug1, Drug const &drug2);

	const int find(Drug const &drug) const;

	Drug find_name(string name) const;

	const Drug find_drug(Drug const &drug) const;

	vector<Drug> filter_by_price(const int price);

	vector<Drug> filter_by_active_substance(const string &active_substance);

	vector<Drug> sort(const string option);

	size_t get_len() noexcept;

	vector<Drug> get_recipe();

	void recipe_clear() noexcept;

	void recipe_generate(int number);

	void recipe_save_to_HTML(string file_name);

	void recipe_add(Drug drug);

	int recipe_length() noexcept;

	void undo();

	~service();
};

class ServiceException : public exception
{
public:
	const vector<string> error_message;
	ServiceException(const vector<string>& errors) : error_message{errors} {}
	ServiceException(const string& error) : error_message{vector<string>{error}} {}
	friend ostream& operator<<(ostream& out, const ServiceException& errors)
	{
		for_each(errors.error_message.begin(), errors.error_message.end(), [&out](string s) { out << s; });
		return out;
	};
};

bool by_name(Drug const &drug1, Drug const &drug2);

bool by_producer(Drug const &drug1, Drug const &drug2);

bool by_as_price(Drug const &drug1, Drug const &drug2);
