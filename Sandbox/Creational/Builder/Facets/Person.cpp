#include "Person.h"
#include "PersonBuilder.h"


namespace Facets
{
	PersonBuilder Person::create()
	{
		return PersonBuilder{};
	}
}
