#pragma once
#include "service.h"
#include <random>
#include <time.h>

service::service(repository ui_repository, validation ui_validation)
{
	service_repository = ui_repository;
	service_validation = ui_validation;
}

vector<Drug> service::print_drugs()
{
	return service_repository.getAll();
}

void service::add_drug(const Drug &drug)
{
	if (service_validation.validate(drug) == false)
		throw ServiceException("Drug isnt valid.\n");
	const int position = find(drug);
	if (position != -1)
		throw ServiceException("Drug already in the database\n");
	service_undo.push_back(new UndoAdauga{service_repository, drug});
	service_repository.add(drug);
}

void service::remove_drug(const Drug &drug)
{
	if (service_validation.validate(drug) == false)
		throw ServiceException("Drug isnt valid.\n");
	const int position = find(drug);
	if (position == -1)
		throw ServiceException("Drug not found in the database\n");
	service_undo.push_back(new UndoSterge{service_repository, service_repository.get(position), position});
	service_repository.remove(position);
}

void service::change_drug(Drug const & drug1, const Drug & drug2)
{
	const int position = find(drug1);
	if (service_validation.validate(drug2) == false)
		throw ServiceException("Drug isnt valid.\n");
	const int position2 = find(drug2);
	if (position == -1)
		throw ServiceException("Drug not found in the database\n");
	if (position2 != -1)
		throw ServiceException("Drug already in the database\n");
	service_undo.push_back(new UndoModifica{service_repository, service_repository.get(position), position});
	service_repository.set(position, drug2);
}

const int service::find(Drug const &drug) const
{
	vector<Drug> data = service_repository.getAll();
	int const position{(int)(distance(data.begin(), find_if(data.begin(), data.end(), [drug](Drug a)noexcept { return drug == a; })))};
	if (position != data.size())
		return position;
	return -1;
}

Drug service::find_name(string name) const
{
	vector<Drug> data = service_repository.getAll();
	auto found = find_if(data.begin(), data.end(), [name](Drug a) { return name == a.get_name(); });
	if (data.end() == found)
		throw ServiceException("Drug not found in the database\n");
	return *found;
}

const Drug service::find_drug(Drug const &drug) const
{
	vector<Drug> data = service_repository.getAll();
	auto found = find_if(data.begin(), data.end(), [drug](Drug a)noexcept { return drug == a; });
	if (data.end() == found)
		throw ServiceException("Drug not found in the database\n");
	return *found;
}

vector<Drug> service::filter_by_price(int price)
{
	vector<Drug> data = service_repository.getAll();
	vector<Drug> temporary;
	copy_if(data.begin(), data.end(), back_inserter(temporary), [price](Drug a)noexcept { return price == a.get_price(); });
	return temporary;
}

vector<Drug> service::filter_by_active_substance(const string &active_substance)
{
	vector<Drug> data = service_repository.getAll();
	vector<Drug> temporary;
	copy_if(data.begin(), data.end(), back_inserter(temporary), [active_substance](Drug a) { return active_substance == a.get_active_substance(); });
	return temporary;
}

vector<Drug> service::sort(const string option)
{
	vector<Drug> temporary = service_repository.getAll();
	if (option == "by_name")
		std::sort(temporary.begin(), temporary.end(), by_name);
	if (option == "by_producer")
		std::sort(temporary.begin(), temporary.end(), by_producer);
	if (option == "by_as_price")
		std::sort(temporary.begin(), temporary.end(), by_as_price);
	return temporary;
}

size_t service::get_len() noexcept
{
	return service_repository.get_len();
}

vector<Drug> service::get_recipe()
{
	return recipe.get_recipe();
}

void service::recipe_clear() noexcept
{
	recipe.clear();
}

void service::recipe_generate(int number)
{
	vector<Drug> data = service_repository.getAll();
	srand((int)time(nullptr));
	int const size = (int)data.size();
	if (size == 0)
		throw ServiceException("The database has no drugs.\n");
	for (int i = 0; i < number; i++)
		recipe.add(data.at(rand() % size));
}

void service::recipe_save_to_HTML(string file_name)
{
	vector<Drug> data = recipe.get_recipe();
	ofstream fout(file_name);
	for_each(data.begin(), data.end(), [&fout](Drug drug) { fout << "<p>" << drug << "<p>"; });
	fout.close();
}

void service::recipe_add(Drug drug)
{
	recipe.add(drug);
}

int service::recipe_length() noexcept
{
	return recipe.size();
}

void service::undo()
{
	if (service_undo.size() == 0)
		throw ServiceException("No previous actions were taken.\n");
	ActiuneUndo* action = service_undo.back();
	action->doUndo();
	service_undo.pop_back();
	delete action;
}

service::~service()
{
	for_each(service_undo.begin(), service_undo.end(), [](ActiuneUndo* action) { delete action; });
}

bool by_name(Drug const &drug1, Drug const &drug2)
{
	if (drug1.get_name() < drug2.get_name())
		return true;
	return false;
}

bool by_producer(Drug const &drug1, Drug const &drug2)
{
	if (drug1.get_producer() < drug2.get_producer())
		return true;
	return false;
}


bool by_as_price(Drug const &drug1, Drug const &drug2)
{
	if (drug1.get_active_substance() < drug2.get_active_substance() ||
		drug1.get_active_substance() == drug2.get_active_substance() && drug1.get_price() < drug2.get_price())
		return true;
	return false;
}
