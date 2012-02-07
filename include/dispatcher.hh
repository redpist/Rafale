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

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include <string>
#include <iostream>
#include "controller.hh"

class Dispatcher
{
public:
  Dispatcher(std::string url)
  {
    {
      std::size_t offset = url.find("http://");
      if (offset == 0)
        url = url.substr(sizeof("http://") - 1, url.size() - sizeof("http://") + 1);
    }
    std::size_t startController = url.find("/");
    if (startController == std::string::npos)
      {
        controller_ = "Default";
        action_ = "Main";
      }
    else
      {
        std::size_t startAction = url.find("/", startController + 1);
        if (startAction == std::string::npos)
          {
            controller_ = url.substr(startController + 1, url.size() - startController - 1);
            action_ = "Main";
          }
        else
          {
            controller_ = url.substr(startController + 1, startAction - startController - 1);
            std::size_t endAction = url.find("/", startAction + 1);
            if (endAction == std::string::npos)
              {
                action_ = url.substr(startAction + 1, url.size() - startAction - 1);
              }
            else
              {
                action_ = url.substr(startAction + 1, endAction - startAction - 1);
              }
          }
      }
    if (!controller_.size())
      controller_ = "Default";
    if (!action_.size())
      action_ = "Main";
  }

  const std::string &Controller()
  {
  return controller_;
  }

  const std::string &Action()
  {
    return action_;
  }

  ~Dispatcher()
  {
  }


private:
  std::string   controller_;
  std::string   action_;
};


#endif /* _DISPATCHER_H_ */
