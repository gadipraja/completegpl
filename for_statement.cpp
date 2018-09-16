#include"for_statement.h"
#include"error.h"

For_stmt::For_stmt(Statement_block* in, expression* e, Statement_block* inc, Statement_block* b)
{
  init = in;
  incr = inc;
  body = b;
  if(e->get_type() != INT)
  {
    Error::error(Error::INVALID_TYPE_FOR_FOR_STMT_EXPRESSION);
    e = new expression(false);
  }
  else
    expr = e;
}

void For_stmt::execute()
{
  for(init->execute();expr->eval_int();incr->execute())
  {
    body->execute();
  }
}
