#include"assignment_statement.h"
#include"gpl_assert.h"
#include"error.h"

Assignment_stmt::Assignment_stmt(Variable* v, Assign_operator op, expression* expr)
{
  switch(op)
  {
    case PLUS_ASSIGN:
      if(!(v->get_type() & (INT|DOUBLE|STRING)))
        Error::error(Error::INVALID_LHS_OF_PLUS_ASSIGNMENT,v->get_name(),gpl_type_to_string(v->get_type()));
      if((v->get_type() == INT) && (expr->get_type() !=INT))
        Error::error(Error::PLUS_ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      else if((v->get_type() == DOUBLE) && (((Gpl_type)expr->get_type() == STRING) || ((Gpl_type)expr->get_type() ==ANIMATION_BLOCK)))
        Error::error(Error::PLUS_ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      else if((v->get_type() == STRING) && ((Gpl_type)expr->get_type() == ANIMATION_BLOCK))
        Error::error(Error::PLUS_ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      rhs = expr;
      lhs = v;
      oper = op;
      break;
    case MINUS_ASSIGN:
      if(!(v->get_type() & (INT|DOUBLE)))
        Error::error(Error::INVALID_LHS_OF_MINUS_ASSIGNMENT,v->get_name(),gpl_type_to_string(v->get_type()));
      if((v->get_type() == INT) && (expr->get_type() !=INT))
        Error::error(Error::MINUS_ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      else if((v->get_type() == DOUBLE) && (((Gpl_type)expr->get_type() == STRING) || ((Gpl_type)expr->get_type() ==ANIMATION_BLOCK)))
        Error::error(Error::MINUS_ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      rhs = expr;
      lhs = v;
      oper = op;
      break;
    case PLUS_PLUS:
      if(!(v->get_type() == INT))
        Error::error(Error::INVALID_LHS_OF_PLUS_PLUS,v->get_name(),gpl_type_to_string(v->get_type()));
      rhs = expr;
      lhs = v;
      oper = op;
      break;
    case MINUS_MINUS:
      if(!(v->get_type() == INT))
        Error::error(Error::INVALID_LHS_OF_MINUS_MINUS,v->get_name(),gpl_type_to_string(v->get_type()));
      rhs = expr;
      lhs = v;
      oper = op;
      break;
    case ASSIGN:
      if(v->get_type() != INT && v->get_type() != DOUBLE && v->get_type() != STRING && v->get_type() != ANIMATION_BLOCK)
        Error::error(Error::INVALID_LHS_OF_ASSIGNMENT,v->get_name(),gpl_type_to_string(v->get_type()));
      if((v->get_type() == INT) && (expr->get_type() !=INT))
        Error::error(Error::ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      else if((v->get_type() == DOUBLE) && (((Gpl_type)expr->get_type() == STRING) || ((Gpl_type)expr->get_type() ==ANIMATION_BLOCK)))
        Error::error(Error::ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      else if((v->get_type() == STRING) && ((Gpl_type)expr->get_type() == ANIMATION_BLOCK))
        Error::error(Error::ASSIGNMENT_TYPE_ERROR,gpl_type_to_string(v->get_type()),gpl_type_to_string((Gpl_type)expr->get_type()));
      else if(v->is_animation_block())
        Error::error(Error::CANNOT_ASSIGN_TO_NON_MEMBER_ANIMATION_BLOCK,v->get_name()); 
      else if(expr->get_type() == ANIMATION_BLOCK)
      {
        Game_object* ab = v->get_game_object_value();
        Gpl_type t =(Gpl_type) ab->get_type();
        Animation_block *go = expr->eval_animation_block();
        Gpl_type r = (Gpl_type) (go->get_parameter_symbol())->get_type();
        if(t != r)
          Error::error(Error::ANIMATION_BLOCK_ASSIGNMENT_PARAMETER_TYPE_ERROR,gpl_type_to_string(t),gpl_type_to_string(r));
      }
      rhs = expr;
      lhs = v;
      oper = op;
      break;
  }
}

void Assignment_stmt::execute()
{
  switch(oper)
  {
    case ASSIGN:
      if(lhs->get_type() == INT)
      {
        if(rhs->get_type() == INT)
          lhs->set_value(rhs->eval_int());
      }
      else if(lhs->get_type() == DOUBLE)
      {
        if(rhs->get_type() != STRING && rhs->get_type() != ANIMATION_BLOCK)
          lhs->set_value(rhs->eval_double());
      }
      else if(lhs->get_type() == STRING)
      {
        if(rhs->get_type()!=ANIMATION_BLOCK)
          lhs->set_value(rhs->eval_string());
      }
      else if(rhs->get_type() == ANIMATION_BLOCK)
      {
        lhs->set_animation_block(rhs->eval_animation_block());
      }
      else
      {
        assert(false&&"not yet");
      }
      break;
    case PLUS_ASSIGN:
      if(lhs->get_type() == INT)
      {
        if(rhs->get_type() == INT)
          lhs->set_value(lhs->get_int_value()+rhs->eval_int());
      }
      else if(lhs->get_type() == DOUBLE)
      {
        if(rhs->get_type() != STRING && rhs->get_type() != ANIMATION_BLOCK)
          lhs->set_value(lhs->get_double_value()+rhs->eval_double());
      }
      else if(lhs->get_type() == STRING)
      {
        if(rhs->get_type()!=ANIMATION_BLOCK)
          lhs->set_value(lhs->get_string_value()+rhs->eval_string());
      }
      else
      {
        Error::error(Error::INVALID_LHS_OF_PLUS_ASSIGNMENT,lhs->get_name(),gpl_type_to_string(lhs->get_type()));
      }
      break;
    case MINUS_ASSIGN:
      if(lhs->get_type() == INT)
      {
        if(rhs->get_type() == INT)
          lhs->set_value(lhs->get_int_value()-rhs->eval_int());
      }
      else if(lhs->get_type() == DOUBLE)
      {
        if(rhs->get_type() != STRING && rhs->get_type() != ANIMATION_BLOCK)
          lhs->set_value(lhs->get_double_value()-rhs->eval_double());
      }
      else
      {
        Error::error(Error::INVALID_LHS_OF_MINUS_ASSIGNMENT,lhs->get_name(),gpl_type_to_string(lhs->get_type()));
      }
      break;
    case PLUS_PLUS:
      if(lhs->get_type() == INT)
      {
        lhs->set_value(lhs->get_int_value()+1);
      }
      break;
    case MINUS_MINUS:
      if(lhs->get_type() == INT)
      {
        lhs->set_value(lhs->get_int_value()-1);
      }
      break;
  }
}
