#include "process.h"

Process::Process(std::function<void(State*)> method)
{
  this->method = method;
}

void Process::start(State* state)
{
  this->future = std::async(std::launch::async, [this, state]()
                            { return this->method(state); });
}

std::future_status Process::getStatus()
{
  if (this->future.valid())
  {
    return this->future.wait_for(std::chrono::milliseconds(1));
  }
  else
  {
    throw std::logic_error("You need to start the process before calling getStatus");
  }
}