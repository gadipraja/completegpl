/*

This file contains the input to the bison compiler generator.
  bison will use this file to generate a C/C++ parser.

The default output file for bison is: y.tab.c

$ bison record.y    will produce the file y.tab.c

This grammar describes a language that is a collection of record
  declarations.  Each has a name and zero or more fields.

record sally
    {
      height = 63;
      weight = 132;
      age = 42;
    }

record george
    {
      age = 31;
      phone = 5551212;
    }

This example DOES NOT demonstrate how a real language would
  create records.  It is a contrived and overly simplified 
  example to demonstrate the bison/flex syntax.

*/
%left T_OR
%left T_AND
%left T_EQUAL T_NOT_EQUAL
%left T_LESS T_LESS_EQUAL T_GREATER T_GREATER_EQUAL 
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_MOD
%nonassoc T_NOT T_UNARY_MINUS 
%nonassoc IF_NO_ELSE
%nonassoc T_ELSE
%left T_NEAR T_TOUCHES
%{  // bison syntax to indicate the start of the header
    // the header is copied directly into y.tab.c (the generated parser)

extern int yylex();               // this lexer function returns next token
extern int yyerror(const char *); // used to print errors
extern int line_count;            // current line in the input; from gpl.l

#include "error.h"      // class for printing errors (used by gpl)
#include "print_statement.h"
#include "exit_statement.h"
#include "assignment_statement.h"
#include "if_statement.h"
#include "for_statement.h"
#include "parser.h"
#include <iostream>
#include <string>
#include "symbol_table.h"
#include "gpl_assert.h"
#include "event_manager.h"
#include <stack>
#include <map>
using namespace std;
// bison syntax to indicate the end of the header

expression *cbe(expression* lhs, Operator_type op, expression* rhs, int legal){
	if((lhs->get_type() & legal) && (rhs->get_type() & legal))
	{
		if(op == DIVIDE)
		{
			if(rhs->eval_double() == 0)
			{
				Error::error(Error::DIVIDE_BY_ZERO_AT_PARSE_TIME);
				return new expression(0);
			}
		}
		if(op == MOD)
		{
			if(rhs->eval_double() ==0)
			{
				Error::error(Error::MOD_BY_ZERO_AT_PARSE_TIME);
				return new expression(0);
			}
		}
		return new expression(lhs,op,rhs);
	}
	else
	{
		//error checking for which one is not valid (rhs/lhs)
		if(!(lhs->get_type() & legal))
			if(op == TOUCHES || op == NEAR)
				Error::error(Error::OPERAND_MUST_BE_A_GAME_OBJECT,"Left");
			else
				Error::error(Error::INVALID_LEFT_OPERAND_TYPE,operator_to_string(op));
		if(!(rhs->get_type() & legal))
			if(op == TOUCHES || op == NEAR)
				Error::error(Error::OPERAND_MUST_BE_A_GAME_OBJECT,"Right");
			else
				Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,operator_to_string(op));
		return new expression(0);
	}
}
expression *cue(Operator_type op, expression* rhs, int legal){
	if(rhs->get_type() & legal)
		return new expression(op,rhs);
	else
	{
		//error checking for which one is not valid (rhs/lhs)
		Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,operator_to_string(op));
		return new expression(0);
	}
}
Symbol *dummy = new Symbol("@_undefined", 0);
Symbol *dummy_go = new Symbol("@_undefined",TRIANGLE);
Variable *dummy_variable = new Variable(dummy);
Statement_block* dummy_sb = new Statement_block();
Game_object *cur_game_object;
string cur_name;
stack<Statement_block*>global_stack;
map<Animation_block*,int>global_map;
std::pair<Gpl_type,string> obj (RECTANGLE,"hello");
%} 

// The union is used to declare the variable yylval which is used to
// pass data between the flex generated lexer and the bison generated parser, 
// and to pass values up/down the parse tree.
// 
// A union is kind of like a structure or class, but you can only use one 
// field at a time.  Each line describes one item in the union.  The left hand
// side is the type, the right hand side is a name for the type.
// 
// Unions do not have any error checking.  For example, if you put an int in
// the following union  (my_union.union_int = 42)  and then attempt to use it
// as a string (cout << my_union.union_string) you will get garbage.

// The "%union" is bison syntax
// The "union_" is my convention to indicate a member of the union 
//     (it can be hard to tell what is a union field and what is not)
//
// In this example, the union only has one member (union_int).  You will
// be adding a double declaration to the union.

%union {
 int            union_int;
 std::string    *union_string;  // MUST be a pointer to a string (this sucks!)
 double         union_double;
 Gpl_type	union_gpl_type;
 expression	*union_expression;
 Operator_type	union_oper;
 Variable	*union_var;
 Symbol		*union_symbol;
 Window::Keystroke	union_key_stroke;
 Statement_block	*union_statement_block;
}


// turn on verbose (longer) error messages
%error-verbose

// A token is like an enumerated type.  It is used to pass information
// between the lexer and the parser.
// Each token in the language is defined here.  By convention, all tokens
// start with T_ and are upper case.

// if a token has a type associated with it, put that type (as named in the
// union) inside of <> after the %token
// the value is put in the union by the scanner (in the .l file ) so it can be
// used by the parser (in the .y file)

%token <union_int>    T_INT_CONSTANT      "int constant"
%token <union_string> T_ID                "identifier"
%token <union_string> T_ERROR             "error"
%token <union_string> T_STRING_CONSTANT   "string constant"
%token <union_double> T_DOUBLE_CONSTANT   "double constant"

%token T_INT                 "int"
%token T_DOUBLE              "double"
%token T_STRING              "string"
%token T_TRIANGLE            "triangle"
%token T_PIXMAP              "pixmap"
%token T_CIRCLE              "circle"
%token T_RECTANGLE           "rectangle"
%token T_TEXTBOX             "textbox"
%token <union_int> T_FORWARD         "forward" // value is line number
%token T_INITIALIZATION      "initialization"
%token T_TERMINATION          "termination"

%token T_TRUE                "true"
%token T_FALSE               "false"

%token T_TOUCHES             "touches"
%token T_NEAR                "near"

%token T_ANIMATION           "animation"

%token T_IF                  "if"
%token T_FOR                 "for"
%token T_ELSE                "else"
%token <union_int> T_PRINT           "print" // value is line number
%token <union_int> T_EXIT            "exit" // value is line number

%token T_LPAREN              "("
%token T_RPAREN              ")"
%token T_LBRACE              "{"
%token T_RBRACE              "}"
%token T_LBRACKET            "["
%token T_RBRACKET            "]"
%token T_SEMIC               ";"
%token T_COMMA               ","
%token T_PERIOD              "."

%token T_ASSIGN              "="
%token T_PLUS_ASSIGN         "+="
%token T_MINUS_ASSIGN        "-="
%token T_PLUS_PLUS           "++"
%token T_MINUS_MINUS         "--"

%token <union_oper> T_MULTIPLY            "*"
%token <union_oper> T_DIVIDE              "/"
%token <union_oper> T_MOD                 "%"
%token <union_oper> T_PLUS                "+"
%token <union_oper> T_MINUS               "-"
%token <union_oper> T_SIN                 "sin"
%token <union_oper> T_COS                 "cos"
%token <union_oper> T_TAN                 "tan"
%token <union_oper> T_ASIN                "asin"
%token <union_oper> T_ACOS                "acos"
%token <union_oper> T_ATAN                "atan"
%token <union_oper> T_SQRT                "sqrt"
%token <union_oper> T_FLOOR               "floor"
%token <union_oper> T_ABS                 "abs"
%token <union_oper> T_RANDOM              "random"

%token <union_oper> T_LESS                "<"
%token <union_oper> T_GREATER             ">"
%token <union_oper> T_LESS_EQUAL          "<="
%token <union_oper> T_GREATER_EQUAL       ">="
%token <union_oper> T_EQUAL               "=="
%token <union_oper> T_NOT_EQUAL           "!="

%token <union_oper> T_AND                 "&&"
%token <union_oper> T_OR                  "||"
%token <union_oper> T_NOT                 "!"

%token T_ON                  "on"
%token T_SPACE               "space"
%token T_LEFTARROW           "leftarrow"
%token T_RIGHTARROW          "rightarrow"
%token T_UPARROW             "uparrow"
%token T_DOWNARROW           "downarrow"
%token T_LEFTMOUSE_DOWN      "leftmouse_down"
%token T_MIDDLEMOUSE_DOWN    "middlemouse_down"
%token T_RIGHTMOUSE_DOWN     "rightmouse_down"
%token T_LEFTMOUSE_UP        "leftmouse_up"
%token T_MIDDLEMOUSE_UP      "middlemouse_up"
%token T_RIGHTMOUSE_UP       "rightmouse_up"
%token T_MOUSE_MOVE          "mouse_move"
%token T_MOUSE_DRAG          "mouse_drag"
%token T_F1                  "f1"
%token T_AKEY                "akey"
%token T_SKEY                "skey"
%token T_DKEY                "dkey"
%token T_FKEY                "fkey"
%token T_HKEY                "hkey"
%token T_JKEY                "jkey"
%token T_KKEY                "kkey"
%token T_LKEY                "lkey"
%token T_WKEY                "wkey"
%token T_ZKEY                "zkey"

%type <union_gpl_type> simple_type
%type <union_expression> expression
%type <union_expression> primary_expression
%type <union_expression> optional_initializer
%type <union_var> variable
%type <union_oper> math_operator
%type <union_gpl_type> object_type
%type <union_symbol> animation_parameter
%type <union_key_stroke> keystroke
%type <union_statement_block> end_of_statement_block
%type <union_statement_block> statement_block
%type <union_statement_block> if_block;
// special token that does not match any production
// used for characters that are not part of the language


// Just like tokens, grammar symbols can be associated with a type
// This allows values to be passed up (and down) the parse tree

%% // indicates the start of the rules
//---------------------------------------------------------------------
program:
       declaration_list block_list
{
	for(auto iter = global_map.begin() ; iter != global_map.end() ; iter++)
	{
		if(iter->second == 0)
			Error::error(Error::NO_BODY_PROVIDED_FOR_FORWARD,(iter->first)->name());
	}
}
    ;

//---------------------------------------------------------------------
declaration_list:
		declaration_list declaration
    | empty
    ;

//---------------------------------------------------------------------
declaration:
	   variable_declaration T_SEMIC
    | object_declaration T_SEMIC
    | forward_declaration T_SEMIC
    ;

//---------------------------------------------------------------------
variable_declaration:
	simple_type  T_ID  optional_initializer
{
	Symbol_table *symbol_table = Symbol_table::instance();
	if(symbol_table->lookup(*$2) == NULL)
	{
		if($1 == INT)
		{	
			int initial_value = 0;
			if($3 != NULL)
			{	
				if($3->get_type() == INT)
				{
					initial_value = $3->eval_int();
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
				else
				{
					Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE,gpl_type_to_string((Gpl_type)$3->get_type()),*$2,"int");
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
			}
			else
			{	
				Symbol *s = new Symbol(*$2,initial_value);
				symbol_table->insert(s);
			}
		}		
		else if($1 == DOUBLE)
		{
			double initial_value = 0.0;
			if($3 != NULL)
			{	
				if($3->get_type() == DOUBLE)
				{
					initial_value = $3->eval_double();
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
				else if($3->get_type() == INT)
				{
					initial_value = (double)$3->eval_int();
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
				else
				{
					Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE,gpl_type_to_string((Gpl_type)$3->get_type()),*$2,"double");
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
			}
			else
			{	
				Symbol *s = new Symbol(*$2,initial_value);
				symbol_table->insert(s);
			}
		}
		else if($1 == STRING)	
		{
			string initial_value = "";
			if($3 != NULL)
			{	
				if($3->get_type() == STRING)
				{
					initial_value = $3->eval_string();
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
				else if($3->get_type() == DOUBLE)
				{
					initial_value = std::to_string($3->eval_double());
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
				else if($3->get_type() == INT)
				{
					initial_value = std::to_string($3->eval_int());
					Symbol *s = new Symbol(*$2,initial_value);
					symbol_table->insert(s);
				}
				else if($3->get_type() == ANIMATION_BLOCK)
				{
          				Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE,"animation_block", *$2, "string");
				}
				else
				{
					assert(false&&"not implemented yet");
				}
			}
			else
			{	
				Symbol *s = new Symbol(*$2,initial_value);
				symbol_table->insert(s);
			}
		}
		else
		{
			assert(false && "not implemented yet");
		}
	}
	else
	{
		Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
	}
}
    	| simple_type  T_ID  T_LBRACKET expression T_RBRACKET
{
	expression *expr = $4;
	if ($4->get_type() != INT)
	{
		Error::error(Error::ARRAY_SIZE_MUST_BE_AN_INTEGER,gpl_type_to_string((Gpl_type)$4->get_type()),*$2);
	}
	else /*if ($4->get_type()==INT)*/
	{
		Symbol_table *symbol_table = Symbol_table::instance();
		int size = $4->eval_int(); 
		//check the type of expression here.
		if(size < 1)
		{
			Error::error(Error::INVALID_ARRAY_SIZE,*$2,to_string(size)); 
		}
		else if(symbol_table->lookup(*$2) == NULL)
		{
			Symbol *s = new Symbol(*$2, $1, size);
			symbol_table->insert(s);
		}
		else
		{
			Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
		}
	}
}
    ;

//---------------------------------------------------------------------
simple_type:
	   T_INT
{
$$ = INT;
}
    | T_DOUBLE
{
$$ = DOUBLE;
}
    | T_STRING
{
$$ = STRING;
}
    ;

//---------------------------------------------------------------------
optional_initializer:
		    T_ASSIGN expression
{
	$$ = $2;
}
    | empty
{
	$$ = NULL;
}
    ;

//---------------------------------------------------------------------
object_declaration:
		  object_type T_ID {
					Symbol_table *symbol_table = Symbol_table::instance();
					if(!(symbol_table->lookup(*$2)))
					{
						Symbol *ns = new Symbol(*$2,$1);
						symbol_table->insert(ns);
						cur_game_object = ns->get_game_object_value(); 
					}
					else
					{
						Symbol *ns = new Symbol(*$2,$1);
						cur_game_object = ns->get_game_object_value(); 
						Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
					}
					cur_name = *$2;
				   } T_LPAREN parameter_list_or_empty T_RPAREN
    | object_type T_ID T_LBRACKET expression T_RBRACKET
{
	expression *expr = $4;
	if ($4->get_type() != INT)
	{
		Error::error(Error::ARRAY_SIZE_MUST_BE_AN_INTEGER,gpl_type_to_string((Gpl_type)$4->get_type()),*$2);
	}
	else
	{
		Symbol_table *symbol_table = Symbol_table::instance();
		int size = $4->eval_int(); 
		//check the type of expression here.
		if(size < 1)
		{
			Error::error(Error::INVALID_ARRAY_SIZE,*$2,to_string(size)); 
		}
		else if(symbol_table->lookup(*$2) == NULL)
		{
			Symbol *s = new Symbol(*$2, $1, size);
			symbol_table->insert(s);
		}
		else
		{
			Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
		}
	}
}
    ;

//---------------------------------------------------------------------
object_type:
	   T_TRIANGLE
{
	$$ = TRIANGLE;
}
    | T_PIXMAP
{
	$$ = PIXMAP;
}
    | T_CIRCLE
{
	$$ = CIRCLE;
}
    | T_TEXTBOX
{
	$$ = TEXTBOX;
}
    | T_RECTANGLE
{
	$$ = RECTANGLE;
}
    ;

//---------------------------------------------------------------------
parameter_list_or_empty :
			parameter_list
    | empty
    ;

//---------------------------------------------------------------------
parameter_list :
	       parameter_list T_COMMA parameter
    | parameter
    ;

//---------------------------------------------------------------------
parameter:
	 T_ID T_ASSIGN expression
{
	Status status;
	string parameter = *$1;
	Gpl_type parameter_type;
	Gpl_type type = (Gpl_type)$3->get_type();
	status = cur_game_object->get_member_variable_type(parameter,parameter_type);
 	if(status==MEMBER_NOT_DECLARED)
  	{
 	   Error::error(Error::UNKNOWN_CONSTRUCTOR_PARAMETER,gpl_type_to_string(cur_game_object->get_type()),*$1);
 	}
	switch(parameter_type)
	{
	case INT:
		if(type != INT)
 		{
		   Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE,cur_name,*$1);
		}
		else
		{
			int value = $3->eval_int();
			status = cur_game_object->set_member_variable(*$1,value);
		}
		break;
	case DOUBLE:
		if(type != INT && type != DOUBLE)
 		{
		   Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE,cur_name,*$1);
		}
		else
		{
			double value = $3->eval_double();
			status = cur_game_object->set_member_variable(*$1,value);
		}
		break;
	case STRING:
		if(type != INT && type != DOUBLE && type != STRING)
 		{
		   Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE,cur_name,*$1);
		}
		else
		{
			string value = $3->eval_string();
			status = cur_game_object->set_member_variable(*$1,value);
		}
		break;
	case ANIMATION_BLOCK:
		Animation_block* value = $3->eval_animation_block();
		if((value->get_parameter_symbol())->get_type() != cur_game_object->get_type())
		{
			Error::error(Error::TYPE_MISMATCH_BETWEEN_ANIMATION_BLOCK_AND_OBJECT,cur_name,value->name());
		}
		else
			status = cur_game_object->set_member_variable(*$1,value);
		break;
	/*default:
		assert(false&&"not implemented yet");	
		break;*/
	}
}
    ;

//---------------------------------------------------------------------
forward_declaration:
		   T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN
{
	Symbol_table *symbol_table = Symbol_table::instance();
	if(!(symbol_table->lookup(*$3)))
	{
		Symbol *ns = new Symbol(*$3,ANIMATION_BLOCK);
		Animation_block *cur_animation = ns->get_animation_block_value();
		symbol_table->insert(ns);
		cur_animation->initialize($5,*$3);
		if($5->getname() != "@_undefined")
		global_map.insert(std::pair<Animation_block*,int>(cur_animation,0));
	}
	else
	{
		Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$3);
	}	
}
    ;

//---------------------------------------------------------------------
block_list:
	  block_list block
    | empty
    ;

//---------------------------------------------------------------------
block:
     initialization_block
    | termination_block
    | animation_block
    | on_block
    ;

//---------------------------------------------------------------------
initialization_block:
		    T_INITIALIZATION statement_block
{
	Event_manager* em = Event_manager::instance();
	em->add($2,Window::Keystroke::INITIALIZE);
}
    ;

//---------------------------------------------------------------------
termination_block:
		 T_TERMINATION statement_block
{
	Event_manager* em = Event_manager::instance();
	em->add($2,Window::Keystroke::TERMINATE);
}
    ;

//---------------------------------------------------------------------
animation_block:
	       T_ANIMATION T_ID T_LPAREN check_animation_parameter 
{
	Symbol_table *symbol_table = Symbol_table::instance();
	Symbol *s = symbol_table->lookup(*$2);
	if(s)
	{
		if(s->get_type() == ANIMATION_BLOCK)
		{
			Animation_block* ab = s->get_animation_block_value();
			string name = ab->get_parameter_symbol()->getname();
			Gpl_type a = (Gpl_type)ab->get_parameter_symbol()->get_type();
			Gpl_type b = obj.first;
			string sname = obj.second;
			if(global_map[ab] == 0 && name == sname && a == b)
			{
				global_stack.push(ab);
				global_map[ab] = 1;	
			}
			else
			{
				global_stack.push(dummy_sb);
				if(global_map[ab] == 1)
				{
					Error::error(Error::PREVIOUSLY_DEFINED_ANIMATION_BLOCK,*$2);
				}
				else
				{
					global_map[ab] = 1;	
					Error::error(Error::ANIMATION_PARAM_DOES_NOT_MATCH_FORWARD);
				}
			} 
		}
		else
		{
			Error::error(Error::NO_FORWARD_FOR_ANIMATION_BLOCK,*$2);
			global_stack.push(new Animation_block());	
		}
	}
	else
	{
		global_stack.push(dummy_sb);
		Error::error(Error::NO_FORWARD_FOR_ANIMATION_BLOCK,*$2);
	}
}
	T_RPAREN T_LBRACE statement_list T_RBRACE end_of_statement_block
    ;

//---------------------------------------------------------------------
animation_parameter:
		   object_type T_ID
{
	Symbol_table *symbol_table = Symbol_table::instance();
	Symbol* ns = new Symbol(*$2,$1);
	Game_object *no = ns->get_game_object_value();
	no->never_animate();
	no->never_draw();
	if(!(symbol_table->lookup(*$2)))
	{
		symbol_table->insert(ns);
		$$ = ns;
	}
	else
	{	
		$$ = dummy_go;
		Error::error(Error::ANIMATION_PARAMETER_NAME_NOT_UNIQUE, *$2);
	}
}
    ;

//---------------------------------------------------------------------
check_animation_parameter:
			 object_type T_ID
{
	std::pair<Gpl_type,string> temp ($1,*$2);
	swap(obj,temp);
}
    ;

//---------------------------------------------------------------------
on_block:
	T_ON keystroke statement_block
{
	Event_manager* em = Event_manager::instance();
	em->add($3,$2);
}
    ;

//---------------------------------------------------------------------
keystroke:
	 T_SPACE
{
	$$ = Window::Keystroke::SPACE;
}
    | T_UPARROW
{
	$$ = Window::Keystroke::UPARROW;
}
    | T_DOWNARROW
{
	$$ = Window::Keystroke::DOWNARROW;
}
    | T_LEFTARROW
{
	$$ = Window::Keystroke::LEFTARROW;
}
    | T_RIGHTARROW
{
	$$ = Window::Keystroke::RIGHTARROW;
}
    | T_LEFTMOUSE_DOWN
{
	$$ = Window::Keystroke::LEFTMOUSE_DOWN;
}
    | T_MIDDLEMOUSE_DOWN
{
	$$ = Window::Keystroke::MIDDLEMOUSE_DOWN;
}
    | T_RIGHTMOUSE_DOWN
{
	$$ = Window::Keystroke::RIGHTMOUSE_DOWN;
}
    | T_LEFTMOUSE_UP
{
	$$ = Window::Keystroke::LEFTMOUSE_UP;
}
    | T_MIDDLEMOUSE_UP
{
	$$ = Window::Keystroke::MIDDLEMOUSE_UP;
}
    | T_RIGHTMOUSE_UP
{
	$$ = Window::Keystroke::RIGHTMOUSE_UP;
}
    | T_MOUSE_MOVE
{
	$$ = Window::Keystroke::MOUSE_MOVE;
}
    | T_MOUSE_DRAG
{
	$$ = Window::Keystroke::MOUSE_DRAG;
}
    | T_AKEY
{
	$$ = Window::Keystroke::AKEY;
}
    | T_SKEY
{
	$$ = Window::Keystroke::SKEY;
}
    | T_DKEY
{
	$$ = Window::Keystroke::DKEY;
}
    | T_FKEY
{
	$$ = Window::Keystroke::FKEY;
}
    | T_HKEY
{
	$$ = Window::Keystroke::HKEY;
}
    | T_JKEY
{
	$$ = Window::Keystroke::JKEY;
}
    | T_KKEY
{
	$$ = Window::Keystroke::KKEY;
}
    | T_LKEY
{
	$$ = Window::Keystroke::LKEY;
}
    | T_WKEY
{
	$$ = Window::Keystroke::WKEY;
}
    | T_ZKEY
{
	$$ = Window::Keystroke::ZKEY;
}
    | T_F1
{
	$$ = Window::Keystroke::F1;
}
    ;

//---------------------------------------------------------------------
if_block:
	statement_block_creator statement end_of_statement_block
{
	$$ = $3;
}
    | statement_block
{
	$$ = $1;
}
    ;

//---------------------------------------------------------------------
statement_block:
	       T_LBRACE statement_block_creator statement_list T_RBRACE end_of_statement_block
{
	$$ = $5;
}
    ;

//---------------------------------------------------------------------
statement_block_creator:
		       // this goes to nothing so that you can put an action here in p7
{
	Statement_block* sb = new Statement_block();
	global_stack.push(sb);
}
    ;

//---------------------------------------------------------------------
end_of_statement_block:
		      // this goes to nothing so that you can put an action here in p7
{
	$$ = global_stack.top();
	global_stack.pop();
}
    ;

//---------------------------------------------------------------------
statement_list:
	      statement_list statement
    | empty
    ;

//---------------------------------------------------------------------
statement:
	 if_statement
    | for_statement
    | assign_statement T_SEMIC
    | print_statement T_SEMIC
    | exit_statement T_SEMIC
    ;

//---------------------------------------------------------------------
if_statement:
	    T_IF T_LPAREN expression T_RPAREN if_block %prec IF_NO_ELSE
{
	Statement_block* cursb = global_stack.top();
	If_stmt *curs = new If_stmt($3,$5);
	cursb->add(curs);
}
    | T_IF T_LPAREN expression T_RPAREN if_block T_ELSE if_block
{
	Statement_block* cursb = global_stack.top();
	If_stmt *curs = new If_stmt($3,$5,$7);
	cursb->add(curs);
}
    ;

//---------------------------------------------------------------------
for_statement:
	     T_FOR T_LPAREN statement_block_creator assign_statement_or_empty end_of_statement_block T_SEMIC expression T_SEMIC statement_block_creator assign_statement_or_empty end_of_statement_block T_RPAREN statement_block
{
	Statement_block* cursb = global_stack.top();
	For_stmt *curs = new For_stmt($5,$7,$11,$13);
	cursb->add(curs);
}
    ;

//---------------------------------------------------------------------
print_statement:
	       T_PRINT T_LPAREN expression T_RPAREN
{
	Statement_block* cursb = global_stack.top();
	Print_stmt *curs = new Print_stmt($3,$1);
	cursb->add(curs);
}
    ;

//---------------------------------------------------------------------
exit_statement:
	      T_EXIT T_LPAREN expression T_RPAREN
{
	Statement_block* cursb = global_stack.top();
	Exit_stmt *curs = new Exit_stmt($3,$1);
	cursb->add(curs);
}
    ;

//---------------------------------------------------------------------
assign_statement_or_empty:
			 assign_statement
    | empty
    ;

//---------------------------------------------------------------------
assign_statement:
		variable T_ASSIGN expression
{
	Statement_block* cursb = global_stack.top();
	Assignment_stmt* curs = new Assignment_stmt($1,ASSIGN,$3);
	cursb->add(curs);
}
    | variable T_PLUS_ASSIGN expression
{
	Statement_block* cursb = global_stack.top();
	Assignment_stmt* curs = new Assignment_stmt($1,PLUS_ASSIGN,$3);
	cursb->add(curs);
}
    | variable T_MINUS_ASSIGN expression
{
	Statement_block* cursb = global_stack.top();
	Assignment_stmt* curs = new Assignment_stmt($1,MINUS_ASSIGN,$3);
	cursb->add(curs);
}
    | variable T_PLUS_PLUS
{
	Statement_block* cursb = global_stack.top();
	Assignment_stmt* curs = new Assignment_stmt($1,PLUS_PLUS);
	cursb->add(curs);
}
    | variable T_MINUS_MINUS
{
	Statement_block* cursb = global_stack.top();
	Assignment_stmt* curs = new Assignment_stmt($1,MINUS_MINUS);
	cursb->add(curs);
}
    ;

//---------------------------------------------------------------------
variable:
	T_ID
{
	Symbol_table *symbol_table = Symbol_table::instance();
	Symbol* s = symbol_table->lookup(*$1);
	//check to make sure it's not an array.
	if(s != NULL && !(s->isarray()))
	{
		$$ = new Variable(s);
	}
	else
	{
		if(s==NULL)
			Error::error(Error::UNDECLARED_VARIABLE,*$1);
		else
		{
			if(s->isarray())
				Error::error(Error::VARIABLE_IS_AN_ARRAY,*$1);
		}
		$$ = dummy_variable;
	}
	//else issue error and make a dummy variable.
}
    | T_ID T_LBRACKET expression T_RBRACKET
{
	/*if ($3->get_type() != INT)
	{
		Error::error(Error::ARRAY_SIZE_MUST_BE_AN_INTEGER,gpl_type_to_string((Gpl_type)$3->get_type()),*$1);
		$$ = dummy_variable;
	}*/
	if ($3->get_type() == DOUBLE)
	{
		Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A double expression");
		$$ = dummy_variable;
	}
	else if ($3->get_type() == STRING)
	{
		Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A string expression");
		$$ = dummy_variable;
	}
	else
	{
		Symbol_table *symbol_table = Symbol_table::instance();
		Symbol* s = symbol_table->lookup(*$1);
		//check if it's an array or not
		if(s!=NULL && s->isarray())
			$$ = new Variable(s,$3);
		else
		{
			if(s==NULL)
				Error::error(Error::UNDECLARED_VARIABLE,(*$1+"[]"));
			else
			{
				if(!(s->isarray()))
					Error::error(Error::VARIABLE_NOT_AN_ARRAY,*$1);
			}
			$$ = dummy_variable;
		}
	}
	//else issue error and make a dummy variable -> variable that hold a dummy symbol, the same dummy symbol as normal variable
}
    | T_ID T_PERIOD T_ID
{
	Symbol_table* st = Symbol_table::instance();
	Symbol* s = st->lookup(*$1);
	if(s)
	{
		if(s->isarray())
		{
			Error::error(Error::VARIABLE_IS_AN_ARRAY,*$1);
			$$ = dummy_variable;
		}
		else if(!(s->get_type() & GAME_OBJECT))
		{
			Error::error(Error::LHS_OF_PERIOD_MUST_BE_OBJECT,*$1);
			$$ = dummy_variable;
		}
		else
		{
			Game_object* go = s->get_game_object_value();
			Gpl_type type;
			Status status = go->get_member_variable_type(*$3,type);
 			if(status==MEMBER_NOT_DECLARED)
  			{
 	  			Error::error(Error::UNDECLARED_MEMBER,*$1,*$3);
				$$ = dummy_variable;
	 		}
			else
			{
				$$ = new Variable(s,*$3);
			}
		}
	}
	else
	{
		Error::error(Error::UNDECLARED_VARIABLE,*$1);
		$$ = dummy_variable;
	}
}
    | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID
{
	Symbol_table* st = Symbol_table::instance();
	Symbol* s = st->lookup(*$1);
	if(s)
	{
		if(!(s->isarray()))
		{
			Error::error(Error::VARIABLE_NOT_AN_ARRAY,*$1);
			$$ = dummy_variable;
		}
		else if(!(s->get_type() & GAME_OBJECT))
		{
			Error::error(Error::LHS_OF_PERIOD_MUST_BE_OBJECT,*$1);
			$$ = dummy_variable;
		}
		else
		{
			if ($3->get_type() == DOUBLE)
			{
				Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A double expression");
				$$ = dummy_variable;
			}
			else if ($3->get_type() == STRING)
			{
				Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A string expression");
				$$ = dummy_variable;
			}
			else
			{
				/*int index = $3->eval_int();
	 			if(index < s->getsize())
				{
					Game_object* go = s->get_game_object_value(0);
					Gpl_type type;
					Status status = go->get_member_variable_type(*$6,type);
	 				if(status==MEMBER_NOT_DECLARED)
  					{
 	  					Error::error(Error::UNDECLARED_MEMBER,*$1,*$6);
						$$ = dummy_variable;
	 				}
					else
					{*/
						$$ = new Variable(s,$3,*$6);
					//}
				//}
			}	
		}
	}
	else
	{
		Error::error(Error::UNDECLARED_VARIABLE,*$1);
		$$ = dummy_variable;
	}
}
    ;

//---------------------------------------------------------------------
expression:
	  primary_expression
{
	$$ = $1;
}
    | expression T_OR expression
{
	$$ = cbe($1,OR,$3,(INT|DOUBLE));
}
    | expression T_AND expression
{
	$$ = cbe($1,AND,$3,(INT|DOUBLE));
}
    | expression T_LESS_EQUAL expression
{
	$$ = cbe($1,LESS_EQUAL,$3,(INT|DOUBLE|STRING));
}
    | expression T_GREATER_EQUAL  expression
{
	$$ = cbe($1,GREATER_EQUAL,$3,(INT|DOUBLE|STRING));
}
    | expression T_LESS expression
{
	$$ = cbe($1,LESS_THAN,$3,(INT|DOUBLE|STRING));
}
    | expression T_GREATER  expression
{
	$$ = cbe($1,GREATER_THAN,$3,(INT|DOUBLE|STRING));
}
    | expression T_EQUAL expression
{
	$$ = cbe($1,EQUAL,$3,(INT|DOUBLE|STRING));
}
    | expression T_NOT_EQUAL expression
{
	$$ = cbe($1,NOT_EQUAL,$3,(INT|DOUBLE|STRING));
}
    | expression T_PLUS expression
{
	$$ = cbe($1,PLUS,$3,(STRING|DOUBLE|INT));
}
    | expression T_MINUS expression
{
	$$ = cbe($1,MINUS,$3,(DOUBLE|INT));
}
    | expression T_MULTIPLY expression
{
	$$ = cbe($1,MULTIPLY,$3,(DOUBLE|INT));
}
    | expression T_DIVIDE expression
{
	
	$$ = cbe($1,DIVIDE,$3,(INT|DOUBLE));
	
}
    | expression T_MOD expression
{
	$$ = cbe($1,MOD,$3,(INT));
}
    | T_MINUS  expression %prec T_UNARY_MINUS
{
	$$ = cue(UNARY_MINUS,$2,(INT|DOUBLE));
}
    | T_NOT  expression
{
	$$ = cue(NOT,$2,(INT|DOUBLE));
}
    | math_operator T_LPAREN expression T_RPAREN
{
	//$$ = new expression($1,$3);
	$$ = cue($1,$3,INT|DOUBLE);
}
    | expression T_NEAR expression
{
	$$ = cbe($1,NEAR,$3,GAME_OBJECT);
}
    | expression T_TOUCHES expression
{
	$$ = cbe($1,TOUCHES,$3,GAME_OBJECT);
}
    ;

//---------------------------------------------------------------------
primary_expression:
		  T_LPAREN  expression T_RPAREN
{
	$$ = $2;
}
    | variable
{
	$$ = new expression($1);
}
    | T_INT_CONSTANT
{
	$$ = new expression($1);
}
    | T_TRUE
{
	$$ = new expression(1);
}
    | T_FALSE
{
	$$ = new expression(0);
}
    | T_DOUBLE_CONSTANT
{
	$$ = new expression($1);
}
    | T_STRING_CONSTANT
{
	$$ = new expression(*$1);
}
    ;

//---------------------------------------------------------------------
math_operator:
	     T_SIN
{
	$$ = SIN;
}
    | T_COS
{
	$$ = COS;
}
    | T_TAN
{
	$$ = TAN;
}
    | T_ASIN
{
	$$ = ASIN;
}
    | T_ACOS
{
	$$ = ACOS;
}
    | T_ATAN
{
	$$ = ATAN;
}
    | T_SQRT
{
	$$ = SQRT;
}
    | T_ABS
{
	$$ = ABS;
}
    | T_FLOOR
{
	$$ = FLOOR;
}
    | T_RANDOM
{
	$$ = RANDOM;
}
    ;

//---------------------------------------------------------------------
empty:
     // empty goes to nothing so that you can use empty in productions
    // when you want a production to go to nothing
    ;

