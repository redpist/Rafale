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

#ifndef _RAFALE_ABSTRACT_H_
#define _RAFALE_ABSTRACT_H_

#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include "rafale/tools.hh"

namespace Rafale
{
  class DateTime
  {
  public:

    DateTime(const std::string &dateTime, const std::string &format = "%Y-%m-%d %H:%M:%S") : tm_(), isValid_(false)
    {
      if (strptime(dateTime.c_str(), format.c_str(), &tm_))
        isValid_ = true;
    }

    DateTime(const time_t &time = Rafale::Now()) : tm_(*localtime(&time))
    {
    }

    bool        IsValid()
    {
      return isValid_;
    }

    std::string ToString(const std::string &format = "%Y-%m-%d %H:%M:%S")
    {
      char buf[255];
      strftime(buf, sizeof(buf), format.c_str(), &tm_);
      return buf;
    }

    bool        operator<(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) < mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    bool        operator>(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) > mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    bool        operator<=(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) <= mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    bool        operator>=(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) >= mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    bool        operator==(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) == mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    ~DateTime() { }
  private:
    struct tm   tm_;
    bool        isValid_;
  };

  typedef std::vector<std::string>              Array;
  typedef std::string                           String;
  typedef std::map<std::string, std::string>    Config;
}

#endif /* _RAFALE_ABSTRACT_H_ */
