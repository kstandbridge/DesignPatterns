#include "PersonBuilder.h"
#include "PersonAddressBuilder.h"
#include "PersonJobBuilder.h"

namespace Facets
{
	PersonAddressBuilder PersonBuilder::lives()
	{
		return PersonAddressBuilder{person};
	}

	PersonJobBuilder PersonBuilder::works()
	{
		return PersonJobBuilder{person};
	}
}
