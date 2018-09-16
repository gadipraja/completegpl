#include"symbol_table.h"
#include<vector>
#include<algorithm>
#include"symbol.h"
using std::vector;

/* static */ Symbol_table *Symbol_table::m_instance = 0;

/* static */ Symbol_table * Symbol_table::instance()
{
  if (m_instance == 0)
    m_instance = new Symbol_table();
  return m_instance;
}

bool Symbol_table::compare_symbol(Symbol *a, Symbol *b)
{
  return a->getname() < b->getname() ? true : false; 
}

Symbol *Symbol_table::lookup(string name)
{
  auto find = table.find(name);
  if(find!=table.end())
    return find->second;
  return NULL;
}

void Symbol_table::print(ostream &os)
{
  vector <Symbol*> symbol;
  for(auto iter = table.begin() ; iter != table.end() ; iter++)
  {
    symbol.push_back(iter->second);
  }
  std::sort(symbol.begin(),symbol.end(),compare_symbol); 
  for(auto iter = symbol.begin() ; iter != symbol.end() ; iter++)
  {
    (*iter)->print(std::cout);
  }
}

bool Symbol_table::insert(Symbol* symbol)
{
  if(table.find(symbol->getname()) == table.end())
  {
    table.insert({symbol->getname(),symbol});
    return true;
  }
  else
    return false;
}
