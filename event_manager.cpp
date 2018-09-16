// updated 2/14/2016

#include "event_manager.h"
#include "gpl_assert.h"
#include "statement_block.h"
using namespace std;

/* static */ Event_manager *Event_manager::m_instance = 0;

/* static */ Event_manager * Event_manager::instance()
{
  if (!m_instance)
    m_instance = new Event_manager();
  return m_instance;
}

Event_manager::Event_manager()
{
}

Event_manager::~Event_manager()
{
}


void 
Event_manager::execute_handlers(Window::Keystroke key)
{
  vector<Statement_block*> sb = m_sb[key];
  for(auto iter = sb.begin(); iter != sb.end() ; iter++)
  {
    (*iter)->execute();
  }
}

void Event_manager::add(Statement_block *nsb, Window::Keystroke key)
{
  m_sb[key].push_back(nsb);
}
