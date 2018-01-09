#include "di.hpp"

#include <cstdio>
#include <iostream>
#include <memory>


namespace DependencyInversionPrinciple
{
	// Dependency Inversion Principle
	// High-level modules should not depend on low-level modules. Both should depend on abstractions.
	// Abstractions should not depend upon details. Details should depend upon abstractions.

	// Inversion on Control (IoC) - the actual process of creating abstractions and getting them to replace dependencies
	// Dependency Injection - use of software frameworks to ensure that a component's dependencies are satisfied

	struct ILogger
	{
		virtual ~ILogger()
		{	
		}

		virtual void Log(const std::string& s) = 0;
	};

	struct ConsoleLogger : ILogger
	{
		void Log(const std::string& s) override
		{
			std::cout << "LOG: " << s.c_str() << std::endl;
		}
	};

	struct Engine
	{
		float volume = 5;
		int horse_power = 400;
		std::shared_ptr<ILogger> logger;


		explicit Engine(const std::shared_ptr<ILogger>& logger)
			: logger(logger)
		{
			logger->Log("Created an engine");
		}

		friend std::ostream& operator<<(std::ostream& os, const Engine& obj)
		{
			return os
					<< "volume: " << obj.volume
					<< " horse_power: " << obj.horse_power;
		}
	};

	struct Car
	{
		std::shared_ptr<Engine> engine;
		std::shared_ptr<ILogger> logger;

		Car(const std::shared_ptr<Engine>& engine, const std::shared_ptr<ILogger>& logger)
			: engine{ engine },
			  logger{ logger }
		{
			logger->Log("Created a car");
		}

		friend std::ostream& operator<<(std::ostream& os, const Car& obj)
		{
			return os << "car with engine: " << *obj.engine;
		}
	};

	int main(int argc, char* argv[])
	{
		//	auto e = std::make_shared<Engine>();
		//	auto c = std::make_shared<Car>(e);

		using namespace boost;
		auto injector = di::make_injector(di::bind<ILogger>().to<ConsoleLogger>());
		auto c = injector.create<std::shared_ptr<Car>>();

		std::cout << *c << std::endl;

		getchar();
		return 0;
	}
}
