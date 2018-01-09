#pragma once
#include "Person.h"

namespace Facets
{
	class PersonAddressBuilder;
	class PersonJobBuilder;

	class PersonBuilder
	{
		Person p;
	protected:
		Person& person;

		explicit PersonBuilder(Person& person)
			: person{person}
		{
		}

	public:
		PersonBuilder()
			: person{p}
		{
		}

		operator Person()
		{
			return std::move(person);
		}

		// builder facets

		PersonAddressBuilder lives();
		PersonJobBuilder works();
	};
}
