#pragma once
#include "service.h"
#include <assert.h>
#include <iostream>
#include <sstream>

class tests
{
public:
	tests() = default;
	void run_tests();
	void test_undo();
};

