#include"error.h"
#include"variable.h"
#include"expression.h"

Variable::Variable(Symbol *ns)
{
  m_symbol = ns;
  m_type = ns->get_type();
}

Variable::Variable(Symbol *ns, string gomember)
{
  m_symbol = ns;
  Game_object* go = ns->get_game_object_value();
  Status status = go->get_member_variable_type(gomember,m_type);
  assert(status == OK);
  m_gomember = gomember;
}

Variable::Variable(Symbol *ns, expression *expr, string gomember)
{
  m_symbol = ns;
  Game_object* go = ns->get_game_object_value(0);
  Status status = go->get_member_variable_type(gomember,m_type);
  assert(status == OK);
  m_expr = expr;
  m_gomember = gomember;
}

Variable::Variable(Symbol *ns, expression *expr)
{
  m_symbol = ns;
  m_expr = expr;
  m_type = ns->get_base_type();
}

Animation_block* Variable::get_animation_block_value()
{
  return m_symbol->get_animation_block_value();
}

Game_object* Variable::get_game_object_value()
{
  if(m_expr == NULL)
    return m_symbol->get_game_object_value();
  else if(m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      return m_symbol->get_game_object_value(index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_game_object_value(0);
    }
  }
  return m_symbol->get_game_object_value();
}

int Variable::get_int_value()
{
  if(m_expr==NULL && m_gomember == "")
    return m_symbol->get_int_value();
  else if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    int value;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK && type == INT && "Game object member not INT");
    status = go->get_member_variable(m_gomember,value);
    assert(status == OK);
    return value;
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      Gpl_type type;
      int value;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK && type == INT && "Game object member not INT");
      status = go->get_member_variable(m_gomember,value);
      assert(status == OK);
      return value;
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_int_value(0);
    }
  }
  else
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
      return m_symbol->get_int_value(index);
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_int_value(0);
    }
  }
  return 0;
}

double Variable::get_double_value()
{
  if(m_expr==NULL && m_gomember == "")
    return m_symbol->get_double_value();
  else if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    double value;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK && type == DOUBLE && "Game object member not DOUBLE");
    status = go->get_member_variable(m_gomember,value);
    assert(status == OK);
    return value;
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize()&& index >= 0) 
    {
      Gpl_type type;
      double value;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK && type == DOUBLE && "Game object member not DOUBLE");
      status = go->get_member_variable(m_gomember,value);
      assert(status == OK);
      return value;
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_double_value(0);
    }
  }
  else
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
      return m_symbol->get_double_value(index);
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_double_value(0);
    }
  }
  return 0.0;
}

string Variable::get_string_value()
{
  if(m_expr==NULL && m_gomember == "")
    return m_symbol->get_string_value();
  else if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    string value;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK && type == STRING && "Game object member not STRING");
    status = go->get_member_variable(m_gomember,value);
    assert(status == OK);
    return value;
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      Gpl_type type;
      string value;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK && type == STRING && "Game object member not STRING");
      status = go->get_member_variable(m_gomember,value);
      assert(status == OK);
      return value;
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_string_value(0);
    }
  }
  else
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
      return m_symbol->get_string_value(index);
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      return m_symbol->get_string_value(0);
    }
  }
  return "";
}

Gpl_type Variable::get_type()
{
  return m_type;
}

void Variable::set_animation_block(Animation_block* value)
{
  if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK);
    m_symbol->set(value,m_gomember);
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      Gpl_type type;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK);
      m_symbol->set(m_gomember,value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
    }
  }
  else
  {
    assert(false&&"animation_block");
  }
}

void Variable::set_value(int value)
{
  if(m_expr == NULL && m_gomember == "")
  {
    m_symbol->set(value);
  }
  else if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK && type == INT && "Game object member not INT");
    m_symbol->set(value,m_gomember);
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      Gpl_type type;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK && type == INT && "Game object member not INT");
      m_symbol->set(m_gomember,value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
    }
  }
  else
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >=  0)
    {
      m_symbol->set(value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      m_symbol->set(value,0);
    }
  }
}

void Variable::set_value(double value)
{
  if(m_expr == NULL && m_gomember == "")
  {
    m_symbol->set(value);
  }
  else if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK && type == DOUBLE && "Game object member not DOUBLE");
    m_symbol->set(value,m_gomember);
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      Gpl_type type;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK && type == DOUBLE && "Game object member not DOUBLE");
      m_symbol->set(m_gomember,value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
    }
  }
  else
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >=  0)
    {
      m_symbol->set(value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      m_symbol->set(value,0);
    }
  }
}

void Variable::set_value(string value)
{
  if(m_expr == NULL && m_gomember == "")
  {
    m_symbol->set(value);
  }
  else if(m_gomember != "" && m_expr == NULL)
  {
    Gpl_type type;
    Game_object* go = m_symbol->get_game_object_value();
    Status status = go->get_member_variable_type(m_gomember,type);
    assert(status == OK && type == STRING && "Game object member not STRING");
    m_symbol->set(value,m_gomember);
  }
  else if(m_gomember != "" && m_expr != NULL)
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      Gpl_type type;
      Game_object* go = m_symbol->get_game_object_value(index);
      Status status = go->get_member_variable_type(m_gomember,type);
      assert(status == OK && type == STRING && "Game object member not STRING");
      m_symbol->set(m_gomember,value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
    }
  }
  else
  {
    int index = m_expr->eval_int();
    if(index < m_symbol->getsize() && index >= 0)
    {
      m_symbol->set(value,index);
    }
    else
    {
      Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,m_symbol->getname(),std::to_string(index));
      m_symbol->set(value,0);
    }
  }
}

std::string Variable::get_name()
{
  if(m_gomember =="")
    return m_symbol->getname();
  else
    return m_symbol->getname() + "." + m_gomember;
}

bool Variable::is_animation_block()
{
  if(m_gomember == "" && m_type == ANIMATION_BLOCK)
    return true;
  return false;
}
