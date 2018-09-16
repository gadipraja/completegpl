#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include"statement.h"
#include"expression.h"
#include"statement_block.h"

class If_stmt : public Statement
{
  private:
    expression* expr;
    Statement_block* then;
    Statement_block* else_block;
  public :
    If_stmt(expression*, Statement_block*, Statement_block* = NULL);
    virtual void execute();
};

#endif
