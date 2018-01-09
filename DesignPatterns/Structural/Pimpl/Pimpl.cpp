#include <iostream>

// Bridge - a mechanism that decouples an interface hierarchy 
// from an implementation hierarchy

// Pimpl Idiom (Pointer to implementation) - speeding up compilation and hiding implementation details
// Any time you change a header file, any files that #include said file need to be recompiled
// the pimpl idiom hides the private members from the header file, so you can make changes without
// needing to recompile

#include "User.h"
#include <memory>

using namespace Pimpl;

int Pimpl_main(int argc, char* argv[])
{
	User f;
	f.name = "Kenneth";
	f.sayHello();

	getchar();
	return 0;
}
