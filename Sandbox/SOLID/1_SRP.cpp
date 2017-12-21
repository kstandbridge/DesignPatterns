#include <cstdio>
#include <string>
#include <vector>
#include <fstream>

namespace SingleResponsibilityPrinciple
{
	// Single Responsibility Principle - module or classes should only provide a single functionality
	// In this example, we have a Journal. The Journal should only be responsible for storing the entries
	// Persisting the entries(save/load) is NOT the responsibilty of the Journal, but a seperate module/class

	struct Journal
	{
		std::string title;
		std::vector<std::string> entries;

		explicit Journal(const std::string& title)
			: title(title)
		{
		}

		void add(const std::string& entry)
		{
			entries.push_back(entry);
		}

		// It is NOT the responsibility of the jornal to save
		//void save(const std::string& filename)
		//{
		//	std::ofstream ofs(filename);
		//	for(auto& s : entries)
		//	{
		//		ofs << s << std::endl;
		//	}
		//}
	};

	// Instead we create something else that is responsible for how it is saved
	struct PersistenceManager
	{
		static void save(const Journal& j, const std::string& filename)
		{
			std::ofstream ofs(filename);
			for(auto& s : j.entries)
			{
				ofs << s << std::endl;
			}
		}
	};
}
