#include <iostream>
#include <vector>
#include <memory>
using namespace std;
#include <boost/lexical_cast.hpp>

namespace Interpreter
{
	// Interpreter

	// Motivation
	// Textual input needs to be processed, turned into OOP structures
	// Think compilers, HTML, XML, regular expressions

	// A component that processes structured text data. Does
	// so by turning it into separate lexical tokens (laxing) and
	// then interpreting sequences of said tokens (parsing).

	// In this example we want to make a string calculator.
	// This will require analyzing the input and turning it into tokens
	// Then intrepeting these tokens using a parser

	// Our token
	struct Token
	{
		enum Type { integer, plus, minus, lparen, rparen } type;
		string text;
		
		Token(Type type, const string& text)
			: type{type},
			  text{text}
		{
		}

		friend ostream& operator<<(ostream& os, const Token& obj)
		{
			return os << "`" << obj.text << "`";
		}
	};

	// Our interface for parsing
	struct Element
	{
		virtual ~Element() = default;
		virtual int eval() const = 0;
	};

	struct Integer : Element
	{
		int value;

		explicit Integer(int value)
			: value{value}
		{
		}

		int eval() const override { return value; }
	};

	
	struct BinaryOperation : Element
	{
		// We'll just cover +/- for simplicity
		enum Type { addition, subtraction } type;
		shared_ptr<Element> lhs, rhs;

		int eval() const override
		{
			if(type == addition)
			{
				return lhs->eval() + rhs->eval();
			}
			else
			{
				return lhs->eval() - rhs->eval();
			}
		}
	};

	// Refered to as laxing, where we seperate the sequence into lexical tokens
	vector<Token> lex(const string& input)
	{
		vector<Token> result;

		for(int i = 0; i < input.length(); ++i)
		{
			switch(input[i])
			{
				case '+':
					result.push_back(Token{Token::plus, "+"});
					break;
				case '-':
					result.push_back(Token{Token::minus, "-"});
					break;
				case '(':
					result.push_back(Token{Token::lparen, "("});
					break;
				case ')':
					result.push_back(Token{Token::rparen, ")"});
					break;
				default:
					// As a number can be multiple digits keep reading
					ostringstream buffer;
					buffer << input[i];
					for(int j = i + 1; j < input.size(); ++j)
					{
						if(isdigit(input[j]))
						{
							buffer << input[j];
							++i;
						}
						else
						{	// Until we are no longer seeing digits
							result.push_back(Token{Token::integer, buffer.str()});
							break;
						}
					}
					break;
			}
		}

		return result;
	}

	// Here we interprate the sequences tokens, parsing
	shared_ptr<Element> parse(const vector<Token>& tokens)
	{
		auto result = make_shared<BinaryOperation>();
		bool have_lhs = false;

		for(int i = 0; i < tokens.size(); ++i)
		{
			auto token = tokens[i];
			switch(token.type)
			{
				case Token::integer: 
					{	// Create a scope as we are declaring variables
						int value = boost::lexical_cast<int>(token.text);
						auto integer = make_shared<Integer>(value);
						if(!have_lhs)
						{
							result->lhs = integer;
							have_lhs = true;
						}
						else
							result->rhs = integer;
					}
					break;
				case Token::plus: 
					result->type = BinaryOperation::addition;
					break;
				case Token::minus: 
					result->type = BinaryOperation::subtraction;
					break;
				case Token::lparen:
					{
						// We don't need a switch for rparen, but
						// we need to know where the rparen is in relation to the lparen
						int j = i;
						for(; j < tokens.size(); ++j)
							if(tokens[j].type == Token::rparen)
								break;

						// This is a ranged initializer, its creating a vector from position X and Y
						// which is a subset of tokens between the lparen and lparen
						vector<Token> subexpression(&tokens[i + 1], &tokens[j]); 

						auto element = parse(subexpression); // recusion

						if(!have_lhs)
						{
							result->lhs = element;
							have_lhs = true;
						}
						else
							result->rhs = element;
						i = j;
					}
					break;
			}
		}

		return result;
	}
}

using namespace Interpreter;

int Interpreter_main(int argc, char* argv[])
{
	string input { "(13-4)-(12+1)" };
	auto tokens = lex(input);

	for(auto&& token : tokens)
		cout << token << "\t";
	cout << endl;

	auto parsed = parse(tokens);

	cout << input << " = " << parsed->eval() << endl;
	
	getchar();
	return EXIT_SUCCESS;
}


