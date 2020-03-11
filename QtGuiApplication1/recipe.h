#pragma once
#include "domain.h"
#include <vector>

class recipe
{
private:
	vector<Drug> data;
public:
	vector<Drug> get_recipe();

	int size() noexcept;

	void add(Drug drug);

	void clear() noexcept;
};

