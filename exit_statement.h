#ifndef EXIT_STATEMENT_H
#define EXIT_STATEMENT_H

#include"statement.h"
#include"expression.h"

class Exit_stmt : public Statement
{
  private:
    expression* expr;
    int m_line;
  public :
    Exit_stmt(expression*,int);
    virtual void execute();
};

#endif
