#include "domain.h"

string Drug::get_name() const
{
	return this->name;
}

void Drug::set_name(string drug_name)
{
	this->name = drug_name;
}

string Drug::get_producer() const
{
	return this->producer;
}

void Drug::set_producer(string drug_producer)
{
	this->producer = drug_producer;
}

string Drug::get_active_substance() const
{
	return this->active_substance;
}

void Drug::set_active_substance(string drug_active_substance)
{
	this->active_substance = drug_active_substance;
}

int Drug::get_price() const noexcept
{
	return this->price;
}

void Drug::set_price(int drug_price) noexcept
{
	this->price = drug_price;
}

bool const Drug::compare(const Drug & a, const Drug & b) noexcept
{
	return (a.name == b.name && a.producer == b.producer);
}

bool const Drug::operator==(const Drug & b)const noexcept
{
	return (name == b.name && producer == b.producer);
}


ostream & operator<<(ostream & out, const Drug & drug)
{
	out << drug.get_name() << " " << drug.get_producer() << " " << drug.get_active_substance() << " " << drug.get_price() << "\n";
	return out;
}
