#ifndef SYMBOL
#define SYMBOL
#include<iostream>
#include<string>
#include"gpl_type.h"
#include"game_object.h"
#include"animation_block.h"

using std::string;

class Symbol{
  public :
    Symbol() {};
    Symbol(string,int);
    Symbol(string,double);
    Symbol(string,string);
    Symbol(string,Gpl_type);
    Symbol(string, Gpl_type, int);
    Gpl_type get_type();
    Gpl_type get_base_type(); // return the type w/o the _ARRAY: INT return INT, INT_ARRAY returns INT
    int getsize();
    bool isarray();
    bool is_game_object();
    
    int get_int_value() const;
    double get_double_value() const;
    string get_string_value() const;
    Game_object* get_game_object_value() const;
    Game_object* get_game_object_value(int index) const;
    Animation_block* get_animation_block_value() const;

    int get_int_value(int index) const;
    double get_double_value(int index) const;
    string get_string_value(int index) const;
    
    void set(int);
    void set(int,int);
    void set(double);
    void set(double,int);
    void set(string);
    void set(string,int);
    void set(Animation_block*,string);
    void set(string,int,int);
    void set(string,Animation_block*,int);
    void set(string,string,int);
    void set(string,double,int);
    void set(int,string);
    void set(double,string);
    void set(string,string);
    void set_game_object(Game_object*);

    void print(std::ostream &os);
    string getname();
  private :
    string m_name; 
    void* m_value;
    Gpl_type m_type;
    int m_size;
};
#endif
