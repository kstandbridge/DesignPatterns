#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <ostream>
using namespace std;

namespace InnerFactory
{
	class Point
	{
		// Factory moved, inner class of Point
		class PointFactory
		{
		public:
			static Point NewCartesian(const float x, const float y)
			{
				return {x,y};
			}

			static Point NewPolar(const float r, const float theta)
			{
				return{r*cos(theta), r*sin(theta)};
			}
		};
	public:
		float x, y;

		friend std::ostream& operator<<(std::ostream& os, const Point& obj)
		{
			return os << "x: " << obj.x << " y: " << obj.y;
		}

		// Single instance of the factory
		static PointFactory Factory;

	private:
		Point(const float x, const float y) : x(x), y(y) {}
	};

	int main()
	{
		// Now point can only be made using the static factory
		auto c = Point::Factory.NewCartesian(1,2);
		cout << c << endl;

		auto p = Point::Factory.NewPolar(5, M_PI_4);
		cout << p << endl;

		getchar();
		return 0;
	}
	
}
