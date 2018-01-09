#include <iostream>
#include <memory>
#include <map>
#include "HotDrink.h"
#include "DrinkFactory.h"
using namespace std;

using namespace AbstractFactory;

unique_ptr<HotDrink> make_drink(string type)
{
	unique_ptr<HotDrink> drink;
	if(type == "tea")
	{
		drink = make_unique<Tea>();
		drink->prepare(200);
	}
	else
	{
		drink = make_unique<Coffee>();
		drink->prepare(50);
	}
	return drink;
}

int AbstractFactory_main()
{
	auto d = make_drink("tea");

	DrinkFactory df;
	df.make_drink("coffee");

	DrinkWithVolumeFactory dvf;
	dvf.make_drink("tea");

	getchar();
	return 0;
}
