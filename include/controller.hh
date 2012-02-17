//////////////////
// Copyright (c) 2011, Jeremy Lecerf
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of R0x nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL JEREMY LECERF BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//////////////////

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <iterator>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

namespace Rafale
{
  class Controller
  {
  public:
    Controller() { }
    virtual ~Controller() { }

    std::string        Action(const std::string &actionName)
    {
      std::string (Rafale::Controller::*actionPtr)(void);

      if ((actionPtr = Rafale::Controller::actions_[actionName]))
        {
          return (this->*actionPtr)();
        }
      return "No Action.";
    }

  protected:
    template <class T>
    void Register(const std::string &actionName, std::string (T::*actionPtr)(void))
    {
      actions_[actionName] = static_cast<std::string (Rafale::Controller::*)(void)>(actionPtr);
    }

    std::map<std::string, std::string (Rafale::Controller::*)(void)>    actions_;

  };
}

#endif /* _CONTROLLER_H_ */
