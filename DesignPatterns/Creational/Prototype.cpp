#include <vector>
#include <iostream>
#include <string>
#include <memory>
using namespace std;

namespace Prototype
{
	struct Address
	{
		string street;
		string city;
		int suite;


		friend std::ostream& operator<<(std::ostream& os, const Address& obj)
		{
			return os
					<< "street: " << obj.street
					<< " city: " << obj.city
					<< " suite: " << obj.suite;
		}
	};

	struct Contact
	{
		string name;
		Address* work_address;


		Contact(const string& name, Address* const work_address)
			: name{name},
			  work_address{ new Address{*work_address} }
		{
		}

		~Contact()
		{
			delete work_address;
		}

		Contact(const Contact& other)
			: name{ other.name },
			  work_address(new Address{*other.work_address}) {}

		friend std::ostream& operator<<(std::ostream& os, const Contact& obj)
		{
			return os
					<< "name: " << obj.name
					<< " work_address: " << *obj.work_address;
		}
	};

	struct EmployeeFactory
	{
		static Contact main, aux;

		static unique_ptr<Contact> NewMainOfficeEmployee(string name, int suite)
		{
			return NewEmployee(name, suite, main);
		}

		static unique_ptr<Contact> NewAuxOfficeEmployee(string name, int suite)
		{
			return NewEmployee(name, suite, aux);
		}
	private:
		static unique_ptr<Contact> NewEmployee(string name, int suite,
		                                       Contact& proto)
		{
			auto result = make_unique<Contact>(proto);
			result->name = name;
			result->work_address->suite = suite;
			return result;
		}
	};

	Contact EmployeeFactory::main{ "", new Address{"123 East Dr", "London", 0} };
	Contact EmployeeFactory::aux{ "", new Address{"123B East Dr", "London", 0} };
}

using namespace Prototype;

int Prototype_main()
{
  

  /*Contact john{ employee };
  john.name = "John";
  john.work_address->suite = 100;

  Contact jane{ employee };
  jane.name = "Jane";
  jane.work_address->suite = 123;*/

  auto john = EmployeeFactory::NewMainOfficeEmployee("John", 100);
  auto jane = EmployeeFactory::NewAuxOfficeEmployee("Jane", 123);

  cout << *john << endl << *jane << endl;

  getchar();
  return 0;
}