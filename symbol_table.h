#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include<iostream>
#include<string>
#include<unordered_map>
using std::ostream;
using std::string;
// to make a singleton class
//
//   1) start with this file
//   2) replace all the Foo with My_class and
//   3) break it into my_class.h and my_class.cpp at the -------------
//   4) add real code


// class Foo is a singleton
class Symbol;
class Symbol_table
{
  public:
    static Symbol_table *instance();
    void print(ostream &os);
    Symbol* lookup(string name);
    static bool compare_symbol (Symbol*, Symbol*);
    bool insert(Symbol*);
  private:
    static Symbol_table *m_instance;
    std::unordered_map<string, Symbol*> table;
    Symbol_table() {};

    // disable default copy constructor and default assignment
    // done as a precaution, they should never be called
    Symbol_table(const Symbol_table &);
    const Symbol_table &operator=(const Symbol_table &);

};
#endif
