#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

namespace Builder
{
	class HtmlBuilder; // forward declaration so we can use before in HtmlElement

	struct HtmlElement
	{
		string name; // the type of name/type of tag ie P for <p>
		string text; // the value you want inside the tag, eg Foo for <p>foo</p>
		vector<HtmlElement> elements; // Notice same type as base, recursion
		const size_t indent_size = 2; // number of spaces we'll use for intenting

	    HtmlElement() {} // empty constructor so we can declare 'HtmlElement root' below

		HtmlElement(const string& name, const string& text)
			: name(name),
			  text(text)
		{
		}

		string str(int indent = 0) const // default value of 0, so we can call str()
		{
			ostringstream oss;
			string i(indent_size * indent, ' '); // Initalize a string of spaces for indentation
			oss << i << "<" << name << ">" << endl; // eg <p>

			if(text.length()) // There is no bool, its either 0 for false, !0 for true, same as text.length() > 0
				oss << string(indent_size * (indent + 1), ' ') << text << endl; // Add extra indentation

			for (const auto& e : elements)
				oss << e.str(indent + 1); // Recursion, calling str() on all the children, but increment the indent

			oss << i << "</" << name << ">" << endl; // eg </p>
			return oss.str();
		}

        // These are forward declarions, the implementations are below
        // This is due to circuldependancies betwen HtmlElement>HtmlBuilder>HtmlElement>etc
		static HtmlBuilder build(const string& root_name); // Encapsulating the builder
		static unique_ptr<HtmlBuilder> build_2(const string& root_name); // example using pointers instead
        // We could avoid this by seperating them into files, HtmlBuilder.h, HtmlBuilder.cpp, HtmlElement.h, HtmlElement.cpp
	};

	struct HtmlBuilder
	{
		HtmlBuilder(const string& root_name) // Notice we are passing a string, not a HtmlElement
		{
			root.name = root_name;
		}

		HtmlBuilder& add_child(const string& child_name, const string& child_text)
		{
			root.elements.emplace_back(child_name, child_text); // emplace back for move semantics/transplanting the memory
			return *this;	// Provides fluent builder: .add_child(foo,bar).add_child(bar,foo)
		}

		HtmlBuilder* add_child_2(const string& child_name, const string& child_text) // Example using pointers instead
		{
			HtmlElement e{child_name, child_text};
			root.elements.emplace_back(e);
			return this;	// return pointer instead
		}

		string str() const{ return root.str(); } // proxy over to the HtmlElement

		operator HtmlElement() { return root; } // Automatic conversations, so we can convert between a HtmlBuilder and a HtmlElement
        // This way we can hide the builder and basically declare the HtmlElement using the returned builder

		HtmlElement root; // The top level element
	};

	HtmlBuilder HtmlElement::build(const string& root_name)
	{   // calling build will builder with the root element created
		return HtmlBuilder{root_name};
	}

	unique_ptr<HtmlBuilder> HtmlElement::build_2(const string& root_name)
	{   // Example using pointers instead
		return make_unique<HtmlBuilder>(root_name);
	}


	int builder_main()
	{
        // Example using the builder directly
		HtmlBuilder builder{"ul"};
		builder.add_child("li","hello")
		       .add_child("li","world");
		cout << builder.str() << endl;

        // Example where the builder is encapsulated within the HtmlElement
        // Notice it returns a HtmlElement even thoigh add_child returns a builder
        // This is due to the automatic conversion above
		HtmlElement e = HtmlElement::build("ul").add_child("li", "hello")
		                                        .add_child("li", "world");

		cout << e.str() << endl;

        // An example if we were using pointers
		HtmlBuilder* ee = HtmlElement::build_2("ul")->add_child_2("li", "hello")
		                                           ->add_child_2("li", "world");

		cout << ee->str() << endl;

		getchar();
		return 0;
	}
	
}
