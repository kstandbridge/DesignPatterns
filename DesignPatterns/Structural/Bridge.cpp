#include <iostream>

// Bridge Pattern - Decouple an abstraction from its implementation so that the two can vary independently
namespace Bridge
{
	struct Renderer
	{
		virtual void render_circle(float x, float y, float radius) = 0;
	};

	struct VectorRender : Renderer
	{
		void render_circle(float x, float y, float radius) override
		{
			std::cout << "Drawing a vector circle of radius " << radius << std::endl;
		}
	};

	struct RasterRenderer : Renderer
	{
		void render_circle(float x, float y, float radius) override
		{
			std::cout << "Rasterizing a circle of radius " << radius << std::endl;
		}
	};

	// Acts as our bridge
	struct Shape
	{
	protected:
		Renderer& renderer; // Reference to the Reenderer interface
		Shape(Renderer& renderer) : renderer{renderer} {} // Initalized on construction
	public:
		virtual void draw() = 0;
		virtual void resize(float factor) = 0;
	};

	struct Circle : Shape
	{
		float x, y, radius;

		void draw() override
		{
			renderer.render_circle(x, y, radius); // We can call the underlying Reenderer
		}

		void resize(float factor) override
		{
			radius += factor;
		}


		Circle(Renderer& renderer, const float x, const float y, const float radius)
			: Shape{renderer}, // Pass the renderer to our base
			  x{x},
			  y{y},
			  radius{radius}
		{
		}
	};
	
}

using namespace Bridge;

int bridge_main(int argc, char* argv[])
{
	RasterRenderer rr;
	Circle raster_circle{rr, 10, 10, 5};
	raster_circle.draw();
	raster_circle.resize(2);
	raster_circle.draw();

	system("PAUSE");
	return 0;
}
