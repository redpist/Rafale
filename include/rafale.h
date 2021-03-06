//////////////////
// Copyright (c) 2012, Jeremy Lecerf
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Rafale nor the
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

#ifndef _RAFALE_H_
#define _RAFALE_H_

#include <map>
#include <string>

namespace Rafale
{
  extern std::map<std::string, std::string>       serverDatas;
  extern std::map<std::string, std::string>       getDatas;
  extern std::map<std::string, std::string>       postDatas;
  extern std::string                              tmpDirectory;
  extern std::string                              filesDirectory;
  extern std::string                              sessionsDirectory;
  extern int                                      cookiesMaxAge;
}

#define DEFAULT_COOKIES_MAX_AGE 3600 // 1 hour

#include "rafale/model.hh"
#include "rafale/controller.hh"
#include "rafale/cookies.hh"
#include "rafale/tools.hh"

#include "rafale/file.hh"
#include "rafale/sessions.hh"

#include "rafale/server.hh"

#endif /* _RAFALE_H_ */
