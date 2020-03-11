#include "repository.h"

void repository::add(const Drug &e)
{
	data.push_back(e);
}

const Drug& repository::get(int index) const
{
	return data.at(index);
}

void repository::remove(int index)
{
	data.erase(data.begin() + index);
}

void repository::insert(int index, const Drug & e)
{
	data.insert(data.begin() + index, e);
}

void repository::set(int index, const Drug &e)
{
	data.at(index) = e;
}

const vector<Drug>& repository::getAll() const noexcept
{
	return data;
}

size_t repository::get_len() const noexcept
{
	return data.size();
}

