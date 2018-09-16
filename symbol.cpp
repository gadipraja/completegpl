#include"symbol.h"
#include"gpl_assert.h"
#include"rectangle.h" 
#include"circle.h"
#include"triangle.h"
#include"pixmap.h"
#include"textbox.h"


using std::endl;
using std::to_string;

Symbol::Symbol(string name, Gpl_type type)
{
  m_name = name;
  m_type = type; 
  m_size = 0;
  switch(type){
    case RECTANGLE:
      m_value = (void*) new Rectangle();
      break;
    case CIRCLE:
      m_value = (void*) new Circle();
      break;
    case PIXMAP:
      m_value = (void*) new Pixmap();
      break;
    case TEXTBOX:
      m_value = (void*) new Textbox();
      break;
    case TRIANGLE:
      m_value = (void*) new Triangle();
      break;
    case ANIMATION_BLOCK:
      m_value = (void*) new Animation_block();
      break;
    default:
      assert(false&&"not implemented yet");
  }
}

Symbol::Symbol(string name, int value)
{
  m_name = name;
  m_type = INT;
  m_value = (void*) new int(value);
  m_size = 0;
}

Symbol::Symbol(string name, double value)
{
  m_name = name;
  m_type = DOUBLE;
  m_value = (void*) new double(value);
  m_size = 0;
}

Symbol::Symbol(string name, string value)
{
  m_name = name;
  m_type = STRING;
  m_value = (void*) new string(value);
  m_size = 0;
}

Symbol::Symbol(string name, Gpl_type type, int size)
{
  m_name = name;
  m_type = (Gpl_type) (type + ARRAY);
  m_size = size;
  if(type == INT)
  {
    int *value = new int [size]; 
    for(int i = 0 ; i < m_size ; i++)
    {
      value[i] = 0;
    }
    m_value = (void*) value;
  }
  else if(type == DOUBLE)
  {
    double *value = new double[size];
    for(int i = 0 ; i < m_size ; i++)
    {
      value[i] = 0.0;
    }
    m_value = (void*) value;
  }
  else if(type == STRING)
  {
    string *value = new string[size];
    for(int i = 0 ; i < m_size ; i++)
    {
      value[i] = "";
    }
    m_value = (void*) value;
  }
  else if(type == CIRCLE)
  {
    Circle *value = new Circle[size];
    m_value = (void*) value;
  }
  else if(type == RECTANGLE)
  {
    Rectangle *value = new Rectangle[size];
    m_value = (void*) value;
  }
  else if(type == TRIANGLE)
  {
    Triangle *value = new Triangle[size];
    m_value = (void*) value;
  }
  else if(type == PIXMAP)
  {
    Pixmap *value = new Pixmap[size];
    m_value = (void*) value;
  }
  else if(type == TEXTBOX)
  {
    Textbox *value = new Textbox[size];
    m_value = (void*) value;
  }
  else
  {
    assert(false && "not implemented yet");
  }
}

void Symbol::print(std::ostream &os)
{
  if(m_size == 0)
  {
    if(m_type == INT)
    {
      int *value = (int*)m_value;
      os <<gpl_type_to_string(m_type) << " " << m_name << " = " << *value << endl;
    }
    else if(m_type == DOUBLE)
    {
      double *value = (double*)m_value;
      os <<gpl_type_to_string(m_type) << " " << m_name << " = " << *value << endl;
    }
    else if(m_type == STRING)
    {
      string *value = (string*)m_value;
      os <<gpl_type_to_string(m_type) << " " << m_name << " = \"" << *value << "\"" << endl;
    }
    else if(m_type&GAME_OBJECT)
    {
      Game_object *value = (Game_object*)m_value;
      value->print(m_name,os);
      os << endl;
    }
    else if(m_type == ANIMATION_BLOCK)
    {
      os <<gpl_type_to_string(m_type) << " " << m_name << endl;
    }
    else
    {
      assert(false && "not implemented yet");
    }
  }
  else
  {
    for(int i = 0 ; i < m_size ; i++)
    {
      if(m_type == INT_ARRAY)
        os << "int " << m_name << "[" << i << "] = " <<get_int_value(i)<< endl;
      else if(m_type == DOUBLE_ARRAY)
        os <<"double " << m_name << "[" << i << "] = " <<get_double_value(i)<< endl;
      else if(m_type == STRING_ARRAY)
        os <<"string " << m_name << "[" << i << "] = \""<<get_string_value(i)<<"\"" << endl;
      else if(m_type & GAME_OBJECT_ARRAY)
      {
        Game_object* go = get_game_object_value(i);
        string name = m_name + "[" + to_string(i) + "]";
        go->print(name,os);
        os<<endl;
      }
      else
      {
        assert(false&&"why here?");
      }
    }
  }
}

string Symbol::getname()
{
  return m_name;
}

Game_object* Symbol::get_game_object_value() const
{
  assert(m_type&GAME_OBJECT);
  assert(m_size == 0);
  return (Game_object*)m_value;
}

Game_object* Symbol::get_game_object_value(int index) const
{
  if(m_type == RECTANGLE_ARRAY)
  {
    Rectangle *obj_arr = (Rectangle*)m_value;
    return &(obj_arr[index]);
  }
  else if(m_type == CIRCLE_ARRAY)
  {
    Circle *obj_arr = (Circle*)m_value;
    return &(obj_arr[index]);
  }
  else if(m_type == TRIANGLE_ARRAY)
  {
    Triangle *obj_arr = (Triangle*)m_value;
    return &(obj_arr[index]);
  }
  else if(m_type == TEXTBOX_ARRAY)
  {
    Textbox *obj_arr = (Textbox*)m_value;
    return &(obj_arr[index]);
  }
  else if(m_type == PIXMAP_ARRAY)
  {
    Pixmap *obj_arr = (Pixmap*)m_value;
    return &(obj_arr[index]);
  }
  else
  {
    return 0;
    assert(false&&"no idea why reach this point");
  }
}

Animation_block* Symbol::get_animation_block_value() const
{
  assert(m_type == ANIMATION_BLOCK);
  assert(m_size == 0);
  return (Animation_block*)m_value;
}

int Symbol::get_int_value() const
{
  assert(m_type == INT);
  assert(m_size == 0);
  return *((int*)m_value);
}
double Symbol::get_double_value() const
{
  assert(m_type == DOUBLE);
  assert(m_size ==0);
  return *((double*)m_value);
}
string Symbol::get_string_value() const
{
  assert(m_type == STRING);
  assert(m_size ==0);
  return *((string*)m_value);
}

int Symbol::get_int_value(int index) const
{
  assert(m_type == INT_ARRAY);
  assert(m_size !=0);
  return *(((int*)m_value)+index);
}
double Symbol::get_double_value(int index) const
{
  assert(m_type == DOUBLE_ARRAY);
  assert(m_size !=0);
  return *(((double*)m_value)+index);
}
string Symbol::get_string_value(int index) const
{
  assert(m_type == STRING_ARRAY);
  assert(m_size !=0);
  return *(((string*)m_value)+index);
}

Gpl_type Symbol::get_type()
{
  return m_type;
}

Gpl_type Symbol::get_base_type()
{
  return (Gpl_type)(m_type-ARRAY);
}

int Symbol::getsize() //change this to int index_in_range(int index) -> return index if it is, return 0 and issue error if it's not
{
  return m_size;
}

bool Symbol::isarray()
{
  if(m_size > 0)
    return true;
  return false;
}

bool Symbol::is_game_object()
{
  if(m_type&GAME_OBJECT)
    return true;
  return false;
}

void Symbol::set_game_object(Game_object* value)
{
  m_value = (void*) value;
}

void Symbol::set(int value)
{
  m_value = (void*) new int(value);
}
void Symbol::set(int value, int index)
{
  *((int*)m_value+index) = value;
}
void Symbol::set(double value)
{
  m_value = (void*) new double(value);
}
void Symbol::set(double value, int index)
{  
  *((double*)m_value+index) = value;
}
void Symbol::set(string value)
{
  m_value = (void*) new string(value);
}
void Symbol::set(string value, int index)
{  
  *((string*)m_value+index) = value;
}
void Symbol::set(Animation_block* value, string member)
{
  Game_object* go = (Game_object*)m_value;
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(int value, string member)
{
  Game_object* go = (Game_object*)m_value;
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(string member, Animation_block* value, int index)
{
  Game_object* go = get_game_object_value(index);
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(string member, int value, int index)
{
  Game_object* go = get_game_object_value(index);
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(double value, string member)
{
  Game_object* go = (Game_object*)m_value;
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(string member, double value, int index)
{
  Game_object* go = get_game_object_value(index);
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(string value, string member)
{
  Game_object* go = (Game_object*)m_value;
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
void Symbol::set(string member, string value, int index)
{
  Game_object* go = get_game_object_value(index);
  Status status = go->set_member_variable(member,value);
  assert(status == OK);
}
