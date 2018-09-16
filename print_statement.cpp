#include"print_statement.h"
#include"iostream"
#include"error.h"

Print_stmt::Print_stmt(expression* e,int line)
{
  m_line = line;
  Gpl_type ee = (Gpl_type)e->get_type();
  if(ee != INT && ee != DOUBLE && ee != STRING)
  {
    Error::error(Error::INVALID_TYPE_FOR_PRINT_STMT_EXPRESSION);
    expr = new expression("");
  }
  else
    expr = e;
}

void Print_stmt::execute()
{
  std::cout <<"print["<<m_line<<"]: "<< expr->eval_string()<<std::endl;
}
