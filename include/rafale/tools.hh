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

#ifndef _RAFALE_TOOLS_H_
#define _RAFALE_TOOLS_H_

#include <cstring>
#include <string>
#include <sstream>
#include <assert.h>
#include <ctime>

#include <string>
#include <functional>

#include <curl/curl.h>
#include <curl/easy.h>

namespace Rafale
{
  time_t        Now();

  std::string UriDecode(const std::string & sSrc);
  std::string UriEncode(const std::string & sSrc);

  std::string Replace(const std::string &str, const std::string &token, const std::string &newToken);
  std::string DeleteEOS(const std::string &str);

  template <typename T>
  int   ToInt(const T &value)
  {
    std::stringstream ss;
    ss << value;
    int i;
    ss >> i;
    return i;
  }

  template <typename T>
  float   ToFloat(const T &value)
  {
    std::stringstream ss;
    ss << value;
    float i;
    ss >> i;
    return i;
  }

  template <typename T>
  std::string   ToString(const T &value)
  {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }

  std::string   ToLower(const std::string &value);
  std::string   ToUpper(const std::string &value);
  bool          Exist(const std::string &fileName);

  struct Mail
  {
    std::string from;
    std::string fromTitle;
    std::string subject;
    std::string message;
    void        Send(const std::string &to);
    static bool IsValid(const std::string &addr) // thx to oreilly =)
    {
      const char *address = addr.c_str();
      int        count = 0;
      const char *c, *domain;
      const char *rfc822_specials = "()<>@,;:\\\"[]";

      /* first we validate the name portion (name@domain) */
      for (c = address;  *c;  c++) {
        if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) ==
                           '\"')) {
          while (*++c) {
            if (*c == '\"') break;
            if (*c == '\\' && (*++c == ' ')) continue;
            if (*c <= ' ' || *c >= 127) return false;
          }
          if (!*c++) return false;
          if (*c == '@') break;
          if (*c != '.') return false;
          continue;
        }
        if (*c == '@') break;
        if (*c <= ' ' || *c >= 127) return false;
        if (strchr(rfc822_specials, *c)) return false;
      }
      if (c == address || *(c - 1) == '.') return false;

      /* next we validate the domain portion (name@domain) */
      if (!*(domain = ++c)) return false;
      do {
        if (*c == '.') {
          if (c == domain || *(c - 1) == '.') return false;
          count++;
        }
        if (*c <= ' ' || *c >= 127) return false;
        if (strchr(rfc822_specials, *c)) return false;
      } while (*++c);

      return (count >= 1);
    }
  };
}


#endif /* _RAFALE_TOOLS_H_ */
