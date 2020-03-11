#pragma once
#include "domain.h"
class validation
{
public:
	validation() = default;
	bool validate(const Drug &drug);
	~validation() = default;
};

