#include <iostream>
#include <vector>
#include <string>

namespace Composite
{
	// Composite - A mechanism for treating individual (scalar) 
	// and compositions of objects in a uniform manner

	// Think of a draw program, you draw multiple shapes but you 
	// need to group them, and draw groups of shapes So lets attempt
	// to treat both single and composite objects uniformly

	// This is our interface, which all composite objects will inherit from
	struct IGraphicObject
	{
		virtual void draw() = 0;
	};

	// Our scalar class
	struct Circle : IGraphicObject
	{
		void draw() override
		{
			// For the sake of simplicity we'll just output some text
			std::cout << "Circle" << std::endl;
		}
	};

	// Our container class, also inherits from interface for hierarchical drawing
	struct Group : IGraphicObject
	{
		std::string name;
		std::vector<IGraphicObject*> objects; // Notice collection of interfaces
	
		Group(const std::string& name) : name{name} {}

		void draw() override
		{
			std::cout << "Group " << name.c_str() << " contains:" << std::endl;
			for (auto o : objects)
				o->draw();
		}

	};
}

int Composite_main(int argc, char* argv[])
{
	using namespace Composite;

	Group root("root");
	Circle c1, c2;
	root.objects.push_back(&c1);

	Group subGroup("sub");
	subGroup.objects.push_back(&c2);

	root.objects.push_back(&subGroup);

	root.draw();

	getchar();
	return 0;
}

