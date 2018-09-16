#ifndef EXPRESSION_H
#define EXPRESSION_H

#include"gpl_type.h"
#include"animation_block.h"
#include"constant.h"
#include<string>

class Variable;
class expression{
  private:
    Operator_type m_oper = NO_OP;
    Gpl_type m_type = NO_TYPE; 
    Constant *m_constant = NULL;
    Variable *m_variable = NULL;
    expression *m_left = NULL;
    expression *m_right = NULL;
  public:
    expression(expression*, Operator_type,expression*); //binary expression
    expression(int);                                    //int constant
    expression(double);                                 //double constant
    expression(std::string);                            //string constant
    expression(Operator_type, expression*);             //unary expression
    expression(Variable*);                              //variable
    int eval_int(); 
    double eval_double();
    std::string eval_string();
    Animation_block* eval_animation_block();
    Game_object* eval_game_object();
    int get_type();
};




#endif
