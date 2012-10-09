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

#ifndef _RAFALE_ENVIRONMENT_HH_
# define _RAFALE_ENVIRONMENT_HH_

#include "rafale/file.hh"
#include "rafale/cookies.hh"

namespace Rafale
{

  class Client;

  struct Environment
  {
    std::map<std::string, Rafale::File>      files;
    std::map<std::string, std::string>       headers;
    std::map<std::string, Rafale::Cookie>    cookies;
    std::map<std::string, std::string>       requestDatas;
    std::map<std::string, std::string>       getDatas;
    std::map<std::string, std::string>       postDatas;
  private:
    static __thread Environment *env_;
    friend Environment &Env();
    friend class Rafale::Client;
  };


  inline Environment &Env()
  {
    return *Environment::env_;
  }

  inline const std::string &ServerName()
  {
    return Env().requestDatas["SERVER_NAME"];
  }
}

#endif /* _RAFALE_ENVIRONMENT_HH_ */