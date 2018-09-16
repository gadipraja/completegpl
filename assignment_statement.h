#ifndef ASSIGNMENT_STATEMENT_H
#define ASSIGNMENT_STATEMENT_H

#include"statement.h"
#include"expression.h"
#include"gpl_type.h"
#include"variable.h"

class Assignment_stmt : public Statement
{
  private:
    expression* rhs;
    Variable* lhs;
    Assign_operator oper;
  public :
    Assignment_stmt(Variable*,Assign_operator,expression*i = NULL);
    virtual void execute();
};

#endif
