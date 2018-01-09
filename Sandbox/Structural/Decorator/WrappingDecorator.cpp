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

		Circle(){}

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

		void resize(float factor)
		{
			radius *= factor;
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

	// In the above examples, lets say we created a colored circle.
	// Then we wanted call resize, or set the radius
	void limitations()
	{
		Circle circle{5};
		ColoredShape redCircle { circle, "red" };
		// redCircle.resize(7)
		// redCircle.radius = 7;
		// This is not possible because ColoredShape is a Shape, not a circle
	}

	// We can solve this with mixin inheritence
	template <typename T> struct ColoredShape2 : T 
	{
		std::string color;
		
		ColoredShape2() { } // Empty as inheriters will call base constructors

		explicit ColoredShape2(const std::string& color)
			: color{color}
		{
		}

		std::string str() const override
		{
			std::ostringstream oss;
			oss << T::str() << " has the color " << color;
			return oss.str();
		}
	};

	template <typename T> struct TransparentShape2 : T
	{
		uint8_t transparency;


		TransparentShape2() { } // Empty as inheriters will call base constructors

		TransparentShape2(uint8_t transparency)
			: transparency{transparency}
		{
		}

		std::string str() const override
		{
			std::ostringstream oss;
			oss << T::str() << " has "
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

	// Using our template classes, mixin inheritence
	ColoredShape2<Circle> red_circle2{ "red" };
	red_circle2.radius = 5;
	std::cout << red_circle2.str() << std::endl;

	TransparentShape2<ColoredShape2<Circle>> red_half_transparent_circle { 128 };
	red_half_transparent_circle.color = "red";
	red_half_transparent_circle.radius = 100;
	std::cout << red_half_transparent_circle.str() << std::endl;

	return 0;
}
