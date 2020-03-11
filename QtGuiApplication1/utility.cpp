#include "utility.h"

bool read_int(int & rez)
{
	{
		string temp;
		getline(cin, temp);
		try
		{
			rez = stoi(temp);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
}
