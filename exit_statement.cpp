#include"exit_statement.h"
#include"iostream"
#include"error.h"
#include"event_manager.h"

Exit_stmt::Exit_stmt(expression* e,int line)
{
  m_line = line;
  if(e->get_type() != INT)
  {
    Error::error(Error::EXIT_STATUS_MUST_BE_AN_INTEGER,gpl_type_to_string((Gpl_type)e->get_type()));
    expr = new expression(1);
  }
  else
    expr=e;
}

void Exit_stmt::execute()
{
  if(expr->get_type() == INT)
  {
    int exits = expr->eval_int();
    std::cout << "gpl[" << m_line << "]: exit(" << exits << ")\n";
    Event_manager::instance()->execute_handlers(Window::TERMINATE);
    exit(exits);
  }
  else
    std::cerr<<"damn it\n";
}
