#include"if_statement.h"
#include"error.h"

If_stmt::If_stmt(expression* e, Statement_block* t, Statement_block* eb)
{
  then = t;
  else_block = eb;
  if(e->get_type() != INT)
  {
    Error::error(Error::INVALID_TYPE_FOR_IF_STMT_EXPRESSION);
    expr = new expression(false);
  }
  else
    expr = e;
}

void If_stmt::execute()
{
  if(expr->eval_int() != 0)
  {
    then->execute();
  }
  else
  {
    if(else_block != NULL)
    {
     else_block->execute();
    }
  }
}
