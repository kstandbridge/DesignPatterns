#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <ostream>
using namespace std;

namespace Factory
{
	class Point
	{
	public:
		float x, y;

		friend std::ostream& operator<<(std::ostream& os, const Point& obj)
		{
			return os << "x: " << obj.x << " y: " << obj.y;
		}

		friend class PointFactory;

	private:
		Point(const float x, const float y) : x(x), y(y) {}
	};

	// We can extract the creation to a seperate factory class
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

	int main()
	{
		auto c = PointFactory::NewCartesian(1,2);
		cout << c << endl;

		auto p = PointFactory::NewPolar(5, M_PI_4);
		cout << p << endl;

		getchar();
		return 0;
	}
	
}
