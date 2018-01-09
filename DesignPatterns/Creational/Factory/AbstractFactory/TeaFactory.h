#pragma once
#include "HotDrinkFactory.h"

namespace AbstractFactory
{
	struct TeaFactory : HotDrinkFactory
	{
		std::unique_ptr<HotDrink> make() override {
			return std::make_unique<Tea>();
		}
	};
}
