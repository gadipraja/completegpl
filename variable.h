#ifndef VARIABLE_H
#define VARIABLE_H

#include"symbol.h"
#include "gpl_type.h"
#include <string>

class expression;
class Variable{
  private:
    Gpl_type m_type = NO_TYPE;
    Symbol* m_symbol = NULL;
    expression* m_expr = NULL;
    string m_gomember = "";
  public:
    Variable() {};
    Variable(Symbol*);
    Variable(Symbol*, string);
    Variable(Symbol*,expression*);
    Variable(Symbol*,expression*,string);
    int get_int_value();
    double get_double_value();
    string get_string_value();
    void set_value(int);
    void set_value(string);
    void set_value(double);
    void set_animation_block(Animation_block*);
    Animation_block* get_animation_block_value();
    Game_object* get_game_object_value();
    Gpl_type get_type();
    std::string get_name();
    bool is_animation_block();
};
#endif
