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

#ifndef _RAFALE_COOKIES_H_
#define _RAFALE_COOKIES_H_

#include "rafale.h"
#include "tools.hh"
#include <string>
#include <map>

namespace Rafale
{
  class Cookie
  {
  public:
    Cookie(const std::string &value = "") : modified(true), value_(value)
    {
    }

    Cookie &operator=(const std::string &value)
    {
      modified = true;
      value_ = Rafale::Replace(value, ";", "");
      value_ = Rafale::Replace(value_, "\r\n", "");
      return *this;
    }

    Cookie &operator=(const Cookie &oth)
    {
      modified = true;
      value_ = oth.value_;
      return *this;
    }

    const std::string &Value()
    {
      return value_;
    }

    ~Cookie() { }

    bool        modified;
  private:
    std::string value_;
  };


  extern std::map<std::string, Rafale::Cookie>       cookies;
}

#endif /* _RAFALE_COOKIES_H_ */
