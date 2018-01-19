#include <iostream>
#include <string>

namespace virtual_proxy
{
	// In this example we have an image that loads from a file,
	// but we only want to load the image when it is needed

	// Here is out pure virtual image
	struct Image
	{
		virtual ~Image() = default;
		virtual void draw() = 0;
	};

	// The actual image
	struct Bitmap : Image
	{
		Bitmap(const std::string& filename)
		{
			std::cout << "Loading a file from " << filename << std::endl;
		}

		void draw() override
		{
			std::cout << "Drawing image" << std::endl;
		}
	};

	void draw_image(Image& img)
	{
		std::cout << "About to draw the image" << std::endl;
		img.draw();
		std::cout << "Done drawing the image" << std::endl;
	}

	// Our proxy to apply the lazy loading
	struct LazyBitmap : Image
	{
		LazyBitmap(const std::string& filename) 
		: filename{filename}, bmp{nullptr}
		{			
		}

		~LazyBitmap()
		{			
			delete bmp; // remember to full up the memeory of the underlying Bitmap
		}

		void draw() override
		{
			if(!bmp) // Here we load the Bitmap if it hasn't been initialized already
				bmp = new Bitmap{filename};
			bmp->draw();
		}

	private:
		Bitmap* bmp;
		std::string filename;
	};
}

using namespace virtual_proxy;
int virtual_proxy_main(int argc, char* argv[])
{
	LazyBitmap bmp{"pokemon.png"}; // We create an instance of the proxy
	draw_image(bmp); // Notice how the file is loaded when calling the draw function
	draw_image(bmp); // Notice how the file isn't loaded twice

	getchar();
	return EXIT_SUCCESS;
}


