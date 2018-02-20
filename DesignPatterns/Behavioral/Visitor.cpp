#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

namespace Visitor
{
    // Motivation
    // Need to define a new operation on an entire class hierarchy
    // - Make a document model printable to HTML/Markdown
    // Do not want to keep modifying every class in the hierarchy
    // Create external component to handle the rendering
    // - But avoid type checks

    // Visitor
    // A pattern where a component (visitor) is allowed to
    // traverse the entire inheritance hierarchy. Implemented by
    // propagating a single visit() function through the
    // entire hierarchy.

    // Dispatch
    // Which function to call?
    // Single dispatch: depends on name of request and type of receiver.
    // Double dispatch: depends on name of request and type of two receivers
    //                  (type of visitor, type of element being visited).

    // Lets use a text render as an example. We want to output html/markdown

    // Some forward declarations of the elements we want to support
    struct Paragraph;
    struct ListItem;
    struct List;

    // The idea of a vistor, that can visit our elements to perform a function
    struct Visitor
    {
        virtual ~Visitor() = default;

        virtual void visit(const Paragraph& p) = 0;
        virtual void visit(const ListItem& p) = 0;
        virtual void visit(const List& p) = 0;

        virtual std::string str() const = 0;
    };

    // All our elements need to accept the visitor
    struct Element
    {
        virtual ~Element() = default;
        virtual void accept(Visitor& v) const = 0;
    };

    struct TextElement : Element
    {
        std::string text;

        explicit TextElement(const std::string& text)
            : text{text}
        {
        }
    };

    struct Paragraph : TextElement
    {
        explicit Paragraph(const std::string& text)
            : TextElement{text} // Proxy over to base
        {
        }


        void accept(Visitor& v) const override
        {
            v.visit(*this); // The implementation simply calls the visitor passing itself as reference
        }
    };

    struct ListItem : TextElement
    {
        explicit ListItem(const std::string& text)
            : TextElement{text} // Proxy over to base
        {
        }


        void accept(Visitor& v) const override
        {
            v.visit(*this);
        }
    };

    struct List : std::vector<ListItem>, Element
    {
        List(const initializer_list<ListItem>& _Ilist)
            : vector<ListItem>{_Ilist}
        {
        }

        void accept(Visitor& v) const override
        {
            v.visit(*this);
        }
    };

    // Now inheriting the base members we can simple include the implementation for rendering Html
    struct HtmlVisitor : Visitor
    {
        void visit(const Paragraph& p) override
        {
            oss << "<p>" << p.text << "</p>" << endl;
        }

        void visit(const ListItem& p) override
        {
            oss << "<li>" << p.text << "</li>" << endl;
        }

        void visit(const List& p) override
        {
            oss << "<ul>" << endl;
            for(auto& x : p)
                x.accept(*this);
            oss << "</ul>" << endl;
        }

        std::string str() const override
        {
            return oss.str();
        }

    private:
        ostringstream oss;
    };

    // Then if we wanted to add support for Markdown, we create another vistor
    struct MarkdownVisitor : Visitor
    {
        void visit(const Paragraph& p) override
        {
            oss << p.text << endl;
        }

        void visit(const ListItem& p) override
        {
            oss << " * " << p.text << endl;
        }

        void visit(const List& p) override
        {
            for(auto& x : p)
                x.accept(*this);
        }

    private:
        ostringstream oss;
    };
}

using namespace Visitor;

int Visitor_main(int argc, char* argv[])
{
    Paragraph p{"Here are some colors: "};
    ListItem red{"Red"};
    ListItem green{"Green"};
    ListItem blue{"Blue"};
    List colors{red, green, blue};

    vector<Element*> document{&p, &colors};
    HtmlVisitor v;
    for(auto x : document)
    {
        x->accept(v);
    }

    cout << v.str() << endl;

    getchar();
    return EXIT_SUCCESS;
}
