#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include"statement.h"
#include"expression.h"
#include"statement_block.h"

class For_stmt : public Statement
{
  private:
    expression* expr;
    Statement_block* init;
    Statement_block* incr;
    Statement_block* body;
  public :
    For_stmt(Statement_block*, expression*, Statement_block*, Statement_block*);
    virtual void execute();
};

#endif
