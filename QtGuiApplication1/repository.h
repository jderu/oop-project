#pragma once
#include <vector>
#include "domain.h"

using namespace std;
class repository
{
private:
	vector<Drug> data;

public:
	//constructor
	repository() = default;

	//adds a new TElement in the repository
	void add(const Drug &e);

	//returns the TElement at the position index
	const Drug& get(int index)const;

	//removes the TElement at the position index
	void remove(int index);

	void insert(int index, const Drug &e);

	//changes the TElement at the position index with e
	void set(int index, const Drug &e);

	//returns all the TElements
	const vector<Drug>& getAll() const noexcept;

	//returns the number of TElements in the repository
	size_t get_len()const noexcept;
};

