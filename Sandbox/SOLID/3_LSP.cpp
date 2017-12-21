#include <cstdio>
#include <iostream>

namespace LiskovSubstitutionPrinciple
{
	// Liskov Substitution Principle

	// Objects in a program should be replaceable with instances of their subtypes
	// w/o altering the correctness of the program

	class Rectangle
	{
	protected:
		int width, height;
	public:
		Rectangle(const int width, const int height)
			: width(width), height(height)
		{
		}

		virtual int get_width() const { return width; }
		virtual void set_width(const int width)
		{
			this->width = width;
		}

		virtual int get_height() const { return height; }
		virtual void set_height(const int height)
		{
			this->height = height;
		}

		int Area() const { return width*height; }
	};

	// We may think a square is a rectangle just with equal dimensions
	class Square : public Rectangle
	{
	public:
		Square(int size) : Rectangle(size, size) {	}


		void set_width(const int width) override
		{
			this->width = height = width;
		}
		void set_height(const int height) override
		{
			this->height = width = height;
		}
	};

	// However parts of our are built against the dimensions not being eual
	void process(Rectangle& r) // not a const
	{
		int w = r.get_width();
		r.set_height(10);

		std::cout << "expect area = " << (w * 10)
				<< ", got " << r.Area() << std::endl;
	}

	int main(int argc, char* argv[])
	{
		// Thus we get the expected result for Rectangle
		Rectangle r{ 5,5 };
		process(r);

		// However undesired behaviour for Square
		Square s{ 5 };
		process(s); // fails
	
		getchar();
		return 0;
	}

	// This could be fixed using a factory pattern
	struct RectangleFactory
	{
		static Rectangle CreateRectangle(int w, int h);
		static Rectangle CreateSquare(int size);
	};
	// The process(Rectangle) function should work regardless of what was passed to it
}
