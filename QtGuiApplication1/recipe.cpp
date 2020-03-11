#include "recipe.h"

vector<Drug> recipe::get_recipe()
{
	return data;
}

int recipe::size() noexcept
{
	return (int)data.size();
}

void recipe::add(Drug drug)
{
	data.push_back(drug);
}

void recipe::clear() noexcept
{
	data.clear();
}
