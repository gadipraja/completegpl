// updated 2/14/16

#include "statement_block.h"
#include "gpl_assert.h"
using namespace std;


Statement_block::Statement_block()
{

}

// this function is called for all non-animation_block statement_blocks
// Implement it for p7
void Statement_block::execute()
{
  // This function should be defined before it is ever called
  // This assert will cause the program to abort if this function 
  // is called before it is implemented.
  for(auto iter = statements.begin(); iter != statements.end();iter++)
  {
    (*iter)->execute();
  }
}

void Statement_block::add(Statement *new_statement)
{
  statements.push_back(new_statement);
}
