#include "validation.h"


bool validation::validate(const Drug &drug)
{
	bool ok = true;
	if (drug.get_name() == "")
		ok = false;
	if (drug.get_producer() == "")
		ok = false;
	if (drug.get_active_substance() == "")
		ok = false;
	if (drug.get_price() < 0)
		ok = false;
	return ok;
}

