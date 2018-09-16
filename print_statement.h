#ifndef PRINT_STATEMENT_H
#define PRINT_STATEMENT_H

#include"statement.h"
#include"expression.h"

class Print_stmt : public Statement
{
  private:
    expression* expr;
    int m_line;
  public :
    Print_stmt(expression*,int);
    virtual void execute();
};

#endif
