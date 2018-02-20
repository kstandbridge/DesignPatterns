#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
using namespace std;

namespace Strategy
{
    // Motivation
    // Many algorithms can be decomposed into
    // higher- and lower-level parts
    // Making tea can be decomposed into
    // - The process of making a hit beverage
    // - Tea-specific things (get a teabag)
    // The high-level algorithm can then be
    // reused for making coffee or hot chocolate
    // - Supported by beverage-specific strategies

    // Strategy
    // Enables the exact behavior of a system to be selected at
    // either run-time (dynamic) or compile-time (static). Also
    // known as a policy.

    // Lets create a text processor, that can output to either MarkDown or Html

    namespace Dynamic
    {
        // Substitute the strategy we are using at runtime

        // We'll create an enum to help identify which formatter to use
        enum class OutputFormat
        {
            Markdown,
            Html
        };

        // We'll do Lists for now, we would need to create strats for any element type to support
        // Build against an interface/pure virtual for polymorphism
        struct ListStrategy
        {
            virtual ~ListStrategy() = default;
            virtual void start(ostringstream& oss) = 0;
            virtual void end(ostringstream& oss) = 0;
            virtual void add_list_item(ostringstream& oss, const string& item) = 0;
        };

        // Mark down text is fairly simple, it just prefixes list items with an asterisk
        struct MarkdownListStrategy : ListStrategy
        {
            void start(ostringstream& oss) override
            {
            }

            void end(ostringstream& oss) override
            {
            }

            void add_list_item(ostringstream& oss, const string& item) override
            {
                oss << " * " << item << endl;
            }
        };

        // Html is more complex, as it defines where the list begins and ends, 
        // as well as where each list item begins and ends
        struct HtmlListStrategy : ListStrategy
        {
            void start(ostringstream& oss) override
            {
                oss << "<ul>" << endl;
            }

            void end(ostringstream& oss) override
            {
                oss << "</ul>" << endl;
            }

            void add_list_item(ostringstream& oss, const string& item) override
            {
                oss << "<li>" << item << "</li>" << endl;
            }
        };

        // Our main processor
        struct TextProcessor
        {
            // So we want to re-use the processor
            void clear()
            {
                oss.str(""); // Set to empty string
                oss.clear(); // clears the buffer
            }

            string str() const { return oss.str(); } // for outputting our stringsteam

            // our strategy has a start() and end(), as well as a call for each item
            void append_list(const vector<string> items)
            {
                list_strategy->start(oss);
                for(auto&& item : items)
                    list_strategy->add_list_item(oss, item);
                list_strategy->end(oss);
            }

            // here we choose which strategy to use, this can be done at runtime, dynamically
            void set_output_format(OutputFormat format)
            {
                switch(format)
                {
                    case OutputFormat::Markdown:
                        list_strategy = make_unique<MarkdownListStrategy>();
                        break;
                    case OutputFormat::Html:
                        list_strategy = make_unique<HtmlListStrategy>();
                        break;
                }
            }

        private:
            ostringstream oss;
            unique_ptr<ListStrategy> list_strategy;
        };

        int main(int argc, char* argv[])
        {
            // Creating a markdown list
            TextProcessor tp;
            tp.set_output_format(OutputFormat::Markdown);
            tp.append_list({"foo", "bar", "baz"});
            cout << tp.str();

            // Creating a html list
            tp.clear();
            tp.set_output_format(OutputFormat::Html);
            tp.append_list({"foo", "bar", "baz"});
            cout << tp.str();

            getchar();
            return EXIT_SUCCESS;
        }
    }

    namespace Static
    {
        // Hardcode a particual strategy at compile time
        
        // We no longer need an enum to identify the strategy
        
        struct ListStrategy
        {
            virtual ~ListStrategy() = default;
            virtual void start(ostringstream& oss) = 0;
            virtual void end(ostringstream& oss) = 0;
            virtual void add_list_item(ostringstream& oss, const string& item) = 0;
        };

        struct MarkdownListStrategy : ListStrategy
        {
            void start(ostringstream& oss) override
            {
            }

            void end(ostringstream& oss) override
            {
            }

            void add_list_item(ostringstream& oss, const string& item) override
            {
                oss << " * " << item << endl;
            }
        };

        struct HtmlListStrategy : ListStrategy
        {
            void start(ostringstream& oss) override
            {
                oss << "<ul>" << endl;
            }

            void end(ostringstream& oss) override
            {
                oss << "</ul>" << endl;
            }

            void add_list_item(ostringstream& oss, const string& item) override
            {
                oss << "<li>" << item << "</li>" << endl;
            }
        };

        // We use a template to define the strategy
        template<typename LS> struct TextProcessor
        {
            TextProcessor()
                : list_strategy{make_unique<LS>()} // Which initalizes the strategy to be used
            {
            }

            void clear()
            {
                oss.str("");
                oss.clear();
            }

            string str() const { return oss.str(); }

            void append_list(const vector<string> items)
            {
                list_strategy->start(oss);
                for(auto&& item : items)
                    list_strategy->add_list_item(oss, item);
                list_strategy->end(oss);
            }


        private:
            ostringstream oss;
            unique_ptr<LS> list_strategy;
        };

        int main(int argc, char* argv[])
        {
            // To create a mark down list
            TextProcessor<MarkdownListStrategy> tpm;
            tpm.append_list({"foo", "bar", "baz"});
            cout << tpm.str() << endl;

            // We cannot reuse tpm, as we cannot switch the strategy
            // and thus need to create a new one for html
            TextProcessor<HtmlListStrategy> tph;
            tph.append_list({"foo", "bar", "baz"});
            cout << tph.str() << endl;

            getchar();
            return EXIT_SUCCESS;
        }
    }
}
