#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

namespace Iterator
{
    // Motivation
    // Iteration (traversal) is a core functionality of various data structures
    // An iterator is a calss that favilitates this traversal
    // - Keeps pointer to an element
    // - Knows how to move to a different element

    // Iterator types
    // Forward (such as List)
    // Bidirectional (doubly linked list)
    // Random access (such as vector)

    // Iterator
    // An ojbect that favilitates the traversal of a data structure

    void std_iterators()
    {
        vector<string> names{"john", "jane", "jill", "jack"};
        vector<string>::iterator it = names.begin(); // Iterator pointing to the first element
        cout << "first name is " << *it << endl; // have to dereference, like a pointer

        ++it; // Now the iterator points to jane;
        it->append(" goodall"); // not const, we can edit elements
        cout << "second name is " << *it << endl;

        while(++it != names.end())
            // end is NOT the last elements, its the one after. Like a null-terminator on a string
        {
            cout << "another name: " << *it << endl;
        }

        for(auto ri = rbegin(names); // reverse end, so the last element
            ri != rend(names); // reverse end, so the beginning really
            ++ri) // we don't use --, ++ is overloaded so it can be used to advance the iterator
        {
            cout << *ri;
            if(ri + 1 != rend(names)) // pointer arithmetic
                cout << ", ";
        }
        cout << endl;

        vector<string>::const_reverse_iterator jack = crbegin(names);
        // *jack += " reacher"; totally invalid, because const
    }

    // Lets create our own iterator!
    // In this example are creating a tree container
    // Each node can spawn off into two further nodes, we'll call them left and right

    template<typename T> struct BinaryTree; // Forward declatrion

    template<typename T> struct Node
    {
        T value = T(); // a tree of T

        Node<T>* left = nullptr; // the left node below
        Node<T>* right = nullptr; // the right node below
        Node<T>* parent = nullptr; // the node above
        BinaryTree<T>* tree = nullptr; // the whole tree

        explicit Node(const T& value)
            : value{value}
        {
        }

        ~Node()
        {   // Redundant check, deleting nullptr is fine
            if(left) delete left;
            if(right) delete right;
        }

        Node(const T& value, Node<T>* left, Node<T>* right)
            : value{value},
              left{left},
              right{right}
        {
            this->left->parent = this->right->parent = this;
        }

        void set_tree(BinaryTree<T>* t)
        {
            tree = t;
            if(left) left->set_tree(t);
            if(right) right->set_tree(t);
        }
    };

    template<typename T> struct BinaryTree
    {
        Node<T>* root = nullptr;

        // Where the magic happens
        template <typename U> struct BinaryTreeIterator
        {
            Node<U>* current;

            explicit BinaryTreeIterator(Node<U>* current)
                : current{current}
            {
            }

            // needed when we traverse the iterator, so we can break a for loop
            bool operator!=(const BinaryTreeIterator<U>& other)
            {
                return current != other.current;
            }

            Node<U>& operator*() { return *current; } // Dereference operator.

            // How we traverse the iterator, reading from left to right
            BinaryTreeIterator<U>& operator++()
            {
                if(current->right)
                {
                    current = current->right;
                    while(current->left)
                        current = current->left;
                }
                else
                {
                    Node<T>* p = current->parent;
                    while(p && current == p->right)
                    {
                        current = p;
                        p = p->parent;
                    }
                    current = p;
                }

                return *this;
            }
        };
        typedef BinaryTreeIterator<T> iterator; // much easier to just type iterator

        explicit BinaryTree(Node<T>* root)
            : root{root}
        {
            root->set_tree(this);
        }

        ~BinaryTree()
        {
            if(root) delete root;
        }

        iterator end()
        {   // How would we like to stop traversing, we'll use a nullptr
            return iterator{nullptr};
        }

        // To start traversing, we'll grab the left-most
        iterator begin()
        {
            Node<T>* n = root;
            if(n)
                while(n->left)
                    n = n->left;
            return iterator { n };
        }
    };
}

using namespace Iterator;

int Iterator_main(int argc, char* argv[])
{
    BinaryTree<string> family {
        new Node<string>{ "me",
            new Node<string>{ "mother",
                new Node<string>{"mother's mother"},
                new Node<string>{"mother's father"}
            },
            new Node<string>{"father"}
        }
    };

    for(auto it = family.begin(); it != family.end(); ++it) // This is using all the overloaded methods we wrote
    {
        cout << (*it).value << endl;
    }

    cout << endl << endl;

    for(auto& node : family) // Becase we have a begin() and end() we can now use a range based for
    {
        cout << node.value << endl;
    }

    getchar();
    return EXIT_SUCCESS;
}

// you can simplify this approach with boost
// naming can cause some confusion. think more facade as in it hides away the complexities of the iterator pattern
#include <boost/iterator/iterator_facade.hpp>

namespace Iterator_Facade
{
    // Lets just have a simple parent/child structure
    struct Node
    {
        string value;
        Node* next = nullptr;
        
        explicit Node(const string& value)
            : value{value}
        {
        }
        
        Node(const string& value, Node* parent)
            : value{value}
        {
            parent->next = this;
        }
    };

    struct ListIterator : boost::iterator_facade<ListIterator,  // The class to derive from, itself
                                                 Node, // What are we iteratoring
                                                 boost::forward_traversal_tag> // How are we traversing?
    {
        Node* current = nullptr; // Which element are we currently pointing to

        ListIterator() // empty constructer to quickly generate a nullptr iterator, so we can end a traverse
        {            
        }

        explicit ListIterator(Node* current)
            : current{current}
        {
        }

    private:
        friend class boost::iterator_core_access; // Helper class for granting access to the iterator core interface

        // We need to give boost increment, equal and dereference functions

        void increment() { current = current->next; } // How do we traverse to the next?

        bool equal(const ListIterator& other) const // How do we compare two elements?
        {
            return other.current == current;
        }

        Node& dereference() const // How do we dereference?
        {
            return *current;
        }
    };

    int Iterator_Facade_main(int argc, char* argv[])
    {
        Node alpha{ "alpha" };
        Node beta{ "beta", &alpha };
        Node gamma{ "gamma", &beta};

        // We can use for_each from <algorithm> to traverse using our ListIterator
        for_each(ListIterator{&alpha}, 
                 ListIterator{},
                 [](const Node& n)
        {
            cout << n.value << endl;
        });


	    getchar();
	    return EXIT_SUCCESS;
    }
}


