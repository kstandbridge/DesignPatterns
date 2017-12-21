#pragma once
#include "HotDrink.h"

namespace AbstractFactory
{
	struct HotDrinkFactory
	{
		virtual std::unique_ptr<HotDrink> make() = 0;
	};
}
