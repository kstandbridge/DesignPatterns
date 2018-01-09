#include <functional>
#include <iostream>
#include <string>

namespace FunctionalDecorator
{
	// Dectorator AKA Wrapper
	// Allows for adding behaviour to individual objects without affecting the behaviour of other objects of the same class
	// Helps following the Open-Closed Principle, as well as Single Responsibility Principle

	// Here we are going to create a functional logger using the decorator pattern

	struct Logger
	{
		// Very basic logger
		std::function<void()> func;
		std::string name;

		Logger(const std::function<void()>& func, const std::string& name)
			: func{func},
			  name{name}
		{
		}

		void operator()() const 
		{
			std::cout << "Logger: Entering " << name << std::endl;
			func();
			std::cout << "Logger: Exiting " << name << std::endl;
		}
	};

	template <typename Func>
	struct Logger2
	{
		Func func;
		std::string name;

		Logger2(Func func, const std::string& name)
			: func{func},
			  name{name}
		{
		}

		void operator()() const 
		{
			std::cout << "Logger2: Entering " << name << std::endl;
			func();
			std::cout << "Logger2: Exiting " << name << std::endl;
		}
	};

	// In this case we need a helper function
	template <typename Func> auto make_logger2(Func func, const std::string& name)
	{
		return Logger2<Func>{func, name};
	}

	template <typename> struct Logger3; // Partial specialization

	template <typename R, typename... Args>
	struct Logger3<R(Args...)> // Partial specialization
	{
		std::function<R(Args...)> func;
		std::string name;
	
		Logger3(const std::function<R(Args...)>& func, const std::string& name)
			: func{func},
			  name{name}
		{
		}

		R operator()(Args ...args)
		{
			std::cout << "Entering " << name << std::endl;
			R result = func(args...);
			std::cout << "Exiting " << name << std::endl;
			return result;
		}
	};

	template <typename R, typename... Args>
	auto make_logger3(R (*func)(Args...), const std::string& name)
	{
		return Logger3<R(Args...)> { std::function<R(Args...)>(func), name };
	}

	// The underlying function we are wrapping logging around
	double add(double a, double b)
	{
		std::cout << a << "+" << b << "=" << (a + b) << std::endl;
		return a + b;
	}

}


int decotator_main(int argc, char* argv[])
{
	using namespace FunctionalDecorator;
	Logger logger{[](){ std::cout << "Logger: Hello" << std::endl; }, "HelloFunc"};
	logger();

//	Logger2 logger2{[](){ std::cout << "Hello" << std::endl; }, "HelloFunc"};
	auto logger2 = make_logger2([](){ std::cout << "Logger2: Hello" << std::endl; }, "HelloFunc");
	logger2();

	auto logged_add = make_logger3(add, "Add");
	auto result = logged_add(2, 3);
	std::cout << "The result is " << result << std::endl;

	return 0;
}
