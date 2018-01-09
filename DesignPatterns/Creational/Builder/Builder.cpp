#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

namespace Builder
{
	class HtmlBuilder;

	struct HtmlElement
	{
		string name;
		string text;
		vector<HtmlElement> elements;
		const size_t indent_size = 2;
		HtmlElement() {}

		HtmlElement(const string& name, const string& text)
			: name(name),
			  text(text)
		{
		}

		string str(int indent = 0) const
		{
			ostringstream oss;
			string i(indent_size * indent, ' '); // Initalize a string of spaces
			oss << i << "<" << name << ">" << endl;

			if(text.size() > 0)
				oss << string(indent_size * (indent + 1), ' ') << text << endl;

			for (const auto& e : elements)
				oss << e.str(indent + 1);

			oss << i << "</" << name << ">" << endl;
			return oss.str();
		}

		static HtmlBuilder build(const string& root_name);
		static unique_ptr<HtmlBuilder> create(const string& root_name);
	};

	struct HtmlBuilder
	{
		HtmlBuilder(const string& root_name)
		{
			root.name = root_name;
		}

		HtmlBuilder& add_child(const string& child_name, const string& child_text)
		{
			HtmlElement e{child_name, child_text};
			root.elements.emplace_back(e);
			return *this;	// Provides fluent builder: .add_child(foo,bar).add_child(bar,foo)
		}

		HtmlBuilder* add_child_2(const string& child_name, const string& child_text)
		{
			HtmlElement e{child_name, child_text};
			root.elements.emplace_back(e);
			return this;	// return pointer instead
		}

		string str() const{ return root.str(); }

		operator HtmlElement() { return root; }

		HtmlElement root;
	};

	HtmlBuilder HtmlElement::build(const string& root_name)
	{
		return HtmlBuilder{root_name};
	}

	unique_ptr<HtmlBuilder> HtmlElement::create(const string& root_name)
	{
		return make_unique<HtmlBuilder>(root_name);
	}


	int builder_main()
	{
		HtmlBuilder builder{"ul"};
		builder.add_child("li","hello")
		       .add_child("li","world");
		cout << builder.str() << endl;

		HtmlElement e = HtmlElement::build("ul").add_child("li", "hello")
		                                        .add_child("li", "world");

		cout << e.str() << endl;

		HtmlBuilder* ee = HtmlElement::create("ul")->add_child_2("li", "hello")
		                                           ->add_child_2("li", "world");

		cout << ee->str() << endl;

		getchar();
		return 0;
	}
	
}
