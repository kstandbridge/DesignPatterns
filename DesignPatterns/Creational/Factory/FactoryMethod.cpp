#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <ostream>
using namespace std;

namespace FactoryMethod
{
	class Point
	{
	public:
		float x, y;

		// Consumors use static methods to create an instance
		static Point NewCartesian(const float x, const float y)
		{
			return {x,y};
		}

		// Better naming, notice we are using the same parameter types
		static Point NewPolar(const float r, const float theta)
		{
			return{r*cos(theta), r*sin(theta)};
		}
	
		friend std::ostream& operator<<(std::ostream& os, const Point& obj)
		{
			return os << "x: " << obj.x << " y: " << obj.y;
		}

	private:
		// Stop consumors creating instances directly
		Point(const float x, const float y) : x(x), y(y) {}
	};

	int main()
	{
		auto c = Point::NewCartesian(1,2);
		cout << c << endl;

		auto p = Point::NewPolar(5, M_PI_4);
		cout << p << endl;

		getchar();
		return 0;
	}
	
}
