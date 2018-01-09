#include <string>
#include <sstream>
#include <iostream>

namespace WrappingDecorator
{
	// Dectorator AKA Wrapper
	// Allows for adding behaviour to individual objects without affecting the behaviour of other objects of the same class
	// Helps following the Open-Closed Principle, as well as Single Responsibility Principle

	// We want to add additional traits to Shapes, but we don't want to intefere with the base class definition
	// This is the Open-Closed Principle, its open for extension but closed for modification

	struct Shape
	{
		virtual std::string str() const = 0;
	};

	struct Circle : Shape
	{
		float radius;

		explicit Circle(float radius)
			: radius{radius}
		{
		}

		std::string str() const override
		{
			std::ostringstream oss;
			oss << "A circle of radius " << radius;
			return oss.str();
		}
	};

	struct Square : Shape
	{
		float side;

		explicit Square(float side)
			: side{side}
		{
		}

		std::string str() const override
		{
			std::ostringstream oss;
			oss << "A square with side = " << side;
			return oss.str();
		}
	};

	// We create a new class that has a reference to the base class
	struct ColoredShape : Shape
	{
		// We can inherit from Shape in this case, because Shape is a pure abstract class, it doesn't hold any state

		Shape& shape; // We have a reference to the underlying Shape
		std::string color; // We add functionality to it
	
		ColoredShape(Shape& shape, const std::string& color)
			: shape{shape},
			  color{color}
		{
		}

		std::string str() const override
		{
			std::ostringstream oss;
			oss << shape.str() << " has the color " << color;
			return oss.str();
		}
	};

	struct TransparentShape : Shape
	{
		Shape& shape;
		uint8_t transparency;

		TransparentShape(Shape& shape, uint8_t transparency)
			: shape{shape},
			  transparency{transparency}
		{
		}

		std::string str() const override
		{
			std::ostringstream oss;
			oss << shape.str() << " has "
					<< static_cast<float>(transparency) / 255.0f * 100.0f
					<< "% transparency";
			return oss.str();
		}
	};
}

int WrappingDecorator_main(int argc, char* argv[])
{
	using namespace WrappingDecorator;
	Circle circle { 5 };
	std::cout << circle.str() << std::endl;

	ColoredShape red_circle { circle, "red" };
	std::cout << red_circle.str() << std::endl;

	TransparentShape half_transparency_circle{ circle, 128 };
	std::cout << half_transparency_circle.str() << std::endl;

	TransparentShape half_tr_red_circle{ red_circle, 128 };
	std::cout << half_tr_red_circle.str() << std::endl;

	return 0;
}
