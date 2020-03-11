#pragma once
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class Drug
{
private:
	string name = "";
	string producer = "";
	string active_substance = "";
	int price = 0;
public:
	//constructor
	Drug(string n, string p, string a_s, int pr) :name{n}, producer{p}, active_substance{a_s}, price{pr}{};

	/*Drug(const Drug& ot) : name{ot.name}, producer{ot.producer}, active_substance{ot.active_substance}, price{ot.price}
	{
		std::cout << "!!!!!!!!!!!!!!!!!!!!\n";
	}*/

	Drug() = default;

	//returns the name of the drug
	string get_name() const;

	//changes the name of the drug
	void set_name(string name);

	//returns the producer of the drug
	string get_producer() const;

	//changes the producer of the drug
	void set_producer(string producer);

	//returns the active substance of the drug
	string get_active_substance() const;

	//changes the active_substance of the drug
	void set_active_substance(string active_substance);

	//returns the price of the drug
	int get_price() const noexcept;

	//changes the price of the drug
	void set_price(int price) noexcept;

	bool const compare(const Drug &a, const Drug &b) noexcept;

	friend ostream & operator << (ostream &out, const Drug &drug);

	bool const operator==(const Drug &b)const  noexcept;
};

