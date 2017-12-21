#include <cstdio>
#include <vector>

namespace InterfaceSegregationPrinciple
{
	// Interface Segregation Principle - No client should be forced to depend on methods it does not use
	
	// In this example we have a multi-function printer.
	struct Document; // Lets just forward declare a document to be used by the MFP

	// So a Monolith example would be this
	//struct IMachine
	//{
	//	virtual void print(std::vector<Document*> docs) = 0;
	//	virtual void scan(std::vector<Document*> docs) = 0;
	//	virtual void fax(std::vector<Document*> docs) = 0;
	//};
	//
	//struct MFP : IMachine
	//{
	//	void print(std::vector<Document*> docs) override; // Changing just print means recompiling the lib, bad
	//	void scan(std::vector<Document*> docs) override; // The client may not need this
	//	void fax(std::vector<Document*> docs) override; // Focing the consumer to implement too much
	//};

	// Howevr we need to seperate this into different concerns
	struct IPrinter
	{
		virtual void print(std::vector<Document*> docs) = 0;
	};

	struct IScanner
	{
		virtual void scan(std::vector<Document*> docs) = 0;
	};

	// Then we can create sub-systems to handle these responsibilities
	struct Printer : IPrinter
	{
		void print(std::vector<Document*> docs) override;
	};

	struct Scanner : IScanner
	{
		void scan(std::vector<Document*> docs) override;
	};

	// Now our MFP can inherit the sub-systems it needs to support, this is the Decorator pattern
	struct IMachine : IPrinter, IScanner {};

	// The implementation of our MFP will proxy these requests over to our subsystems
	struct Machine : IMachine
	{
		IPrinter& printer; // Notice we are using the interface, not a concrete implementation.
		IScanner& scanner;
	
		Machine(IPrinter& printer, IScanner& scanner)
			: printer(printer), // If one day we get a color printer, we just need to pass in a different implementation here
			  scanner(scanner)
		{
		}

		// We just proxy it over
		void print(std::vector<Document*> docs) override
		{
			printer.print(docs);
		}

		void scan(std::vector<Document*> docs) override
		{
			scanner.scan(docs);
		}
	};
}
