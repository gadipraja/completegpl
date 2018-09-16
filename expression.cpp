#include"expression.h"
#include<math.h>
#include"gpl_assert.h"
#include"variable.h"
#include<iostream>
#include"error.h"
using std::string; 

expression::expression(int value)
{
  m_type = INT;
  m_constant = new Constant(value);
  //std::cout << "creating INT constant\n";
}

expression::expression(double value)
{
  m_type = DOUBLE;
  m_constant = new Constant(value);
  //std::cout << "creating DOUBLE constant\n";
}

expression::expression(std::string value)
{
  m_type = STRING;
  m_constant = new Constant(value);
  //std::cout << "creating STRING constant\n";
}

expression::expression(Operator_type op, expression* expr)
{
  m_oper = op;
  m_right = expr;
  if(m_oper == SIN || m_oper == COS || m_oper == TAN || m_oper == SQRT || m_oper == ACOS || m_oper == ASIN || m_oper == ATAN)
    m_type = DOUBLE;
  else if(m_oper == FLOOR || m_oper == RANDOM || m_oper == NOT)
    m_type = INT;
  else
    m_type = (Gpl_type)expr->get_type();
}

expression::expression(expression* expr1, Operator_type op, expression* expr2)
{
  m_oper = op;
  m_left = expr1;
  m_right = expr2;
  if(op == AND || op == OR || op == EQUAL || op == NOT_EQUAL
      || op == LESS_THAN || op == LESS_EQUAL || op == GREATER_THAN
      || op == GREATER_EQUAL || op == NEAR || op == TOUCHES)
    m_type = INT;
  else if(expr1->get_type() == STRING || expr2->get_type() == STRING)
    m_type = STRING;
  else if(expr1->get_type() == DOUBLE || expr2->get_type() == DOUBLE)
    m_type = DOUBLE;
  else if(expr1->get_type() == INT && expr2->get_type() == INT)
    m_type = INT;
  else
    assert(false && "not implemented yet");
  //std::cout << "creating BINARY EXPRESSION, expr1 = " << m_left->m_constant->get_int_value() << " operator = " << m_oper << " expr2 = " <<m_right->m_constant->get_int_value()<<" oper = " << op<<"\n";
}

expression::expression(Variable* var)
{
  m_variable = var; 
  m_type = m_variable->get_type();
}

int expression::eval_int()
{
  assert(m_type == INT);
  if(m_constant != NULL)
  {
    return m_constant->get_int_value();
  }
  else if(m_variable != NULL)
  {
    return m_variable->get_int_value();
  }
  else
  {
    switch (m_oper){
      case PLUS:
        return m_left->eval_int() + m_right->eval_int();

      case MULTIPLY:
        return m_left->eval_int() * m_right->eval_int();

      case DIVIDE:
        return m_left->eval_int() / m_right->eval_int();

      case MINUS:
        return m_left->eval_int() - m_right->eval_int();

      case MOD:
        return m_left->eval_int() % m_right->eval_int();

      case FLOOR:
        if(m_right->m_type == INT)
          return floor(m_right->eval_int());
        else
          return floor(m_right->eval_double());

      case RANDOM:
        if(m_right->m_type == INT)
        {
          int arg = m_right->eval_int();
          if(arg >= 1)
            return rand() % arg;
          else
          {
            Error::error(Error::INVALID_ARGUMENT_FOR_RANDOM,std::to_string(arg));
            return rand() % 2;
          }
        }
        else
        {
          double arg = m_right->eval_double();
          if(arg >= 1)
            return rand() % (int)arg;
          else 
            Error::error(Error::INVALID_ARGUMENT_FOR_RANDOM,std::to_string(arg));
          return rand() % 2;
        }

      case ABS:
        return abs(m_right->eval_int());

      case UNARY_MINUS:
        return m_right->eval_int() * (-1);

      case EQUAL:
        if(m_left->m_type == STRING || m_right->m_type == STRING)
          return (m_left->eval_string()) == (m_right->eval_string());
        else if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) == (m_right->eval_double());
        else
          return (m_left->eval_int()) == (m_right->eval_int());

      case NOT_EQUAL:
        if(m_left->m_type == STRING || m_right->m_type == STRING)
          return (m_left->eval_string()) != (m_right->eval_string());
        else if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) != (m_right->eval_double());
        else
          return (m_left->eval_int()) != (m_right->eval_int());

      case LESS_EQUAL:
        if(m_left->m_type == STRING || m_right->m_type == STRING)
          return (m_left->eval_string()) <= (m_right->eval_string());
        else if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) <= (m_right->eval_double());
        else
          return (m_left->eval_int()) <= (m_right->eval_int());

      case LESS_THAN:
        if(m_left->m_type == STRING || m_right->m_type == STRING)
          return (m_left->eval_string()) < (m_right->eval_string());
        else if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) < (m_right->eval_double());
        else
          return (m_left->eval_int()) < (m_right->eval_int());

      case GREATER_EQUAL:
        if(m_left->m_type == STRING || m_right->m_type == STRING)
          return (m_left->eval_string()) >= (m_right->eval_string());
        else if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) >= (m_right->eval_double());
        else
          return (m_left->eval_int()) >= (m_right->eval_int());
      case GREATER_THAN:
        if(m_left->m_type == STRING || m_right->m_type == STRING)
          return (m_left->eval_string()) > (m_right->eval_string());
        else if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) > (m_right->eval_double());
        else
          return (m_left->eval_int()) > (m_right->eval_int());
      case AND:
        if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) && (m_right->eval_double());
        else
          return (m_left->eval_int()) && (m_right->eval_int());
      case OR:
        if(m_left->m_type == DOUBLE || m_right->m_type == DOUBLE)
          return (m_left->eval_double()) || (m_right->eval_double());
        else
          return (m_left->eval_int()) || (m_right->eval_int());
      case NOT:
        if(m_right->m_type == DOUBLE)
          return !(m_right->eval_double());
        else
          return !(m_right->eval_int());
      case NEAR:
        return(m_left->eval_game_object())->near(m_right->eval_game_object());
      case TOUCHES:
        return(m_left->eval_game_object())->touches(m_right->eval_game_object());
      default:
        std::cout <<"m_oper = " << operator_to_string(m_oper);
        assert(false);
    }
  }
  return 0;
}

double expression::eval_double()
{
  if(m_type == INT)
  {
    return (double)eval_int();
  }
  else
  {
    if(m_constant != NULL)
    {
      //std::cout <<"m_value = " << m_constant->get_int_value();
      return m_constant->get_double_value();
    }
    else if(m_variable != NULL)
    {
      return m_variable->get_double_value();
    }
    else
    {
      switch (m_oper){
        case PLUS:
          return m_left->eval_double() + m_right->eval_double();

        case MULTIPLY:
          return m_left->eval_double() * m_right->eval_double();

        case DIVIDE:
          return m_left->eval_double() / m_right->eval_double();

        case MINUS:
          return m_left->eval_double() - m_right->eval_double();

        case SIN:
          return sin((m_right->eval_double() * M_PI)/180);

        case ASIN:
          return asin(m_right->eval_double())*180/M_PI;

        case COS:
          return cos((m_right->eval_double() * M_PI)/180);

        case ACOS:
          return acos(m_right->eval_double())*180/M_PI;

        case TAN:
          return tan((m_right->eval_double() * M_PI)/180);

        case ATAN:
          return atan(m_right->eval_double())*180/M_PI;

        case SQRT:
          return sqrt(m_right->eval_double());

        case ABS:
          return fabs(m_right->eval_double());

        case UNARY_MINUS:
          return m_right->eval_double() * (-1);

        default:
          //std::cout <<"m_oper = " << m_oper;
          assert(false&&"not implemented yet");
      }
    }
  }
  return 0.0;
}

string expression::eval_string()
{
  if(m_type == INT)
  {
    return std::to_string(eval_int());
  }
  else if(m_type == DOUBLE)
  {
    return std::to_string(eval_double());
  }
  else
  {
    if(m_constant != NULL)
    {
      //std::cout <<"m_value = " << m_constant->get_int_value();
      return m_constant->get_string_value();
    }
    else if(m_variable != NULL)
    {
      return m_variable->get_string_value();
    }
    else
    {
      switch (m_oper){
        case PLUS:
          return m_left->eval_string() + m_right->eval_string();
        default:
          // std::cout <<"m_oper = " << m_oper;
          assert(false&&"not implemented yet");
      }
    }
  }
  return "";
}

int expression::get_type()
{
  return m_type;
}

Animation_block* expression::eval_animation_block()
{
  return m_variable->get_animation_block_value();
}

Game_object* expression::eval_game_object()
{
  return m_variable->get_game_object_value();
}





