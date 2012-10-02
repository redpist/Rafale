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

#ifndef _RAFALE_SERVER_H_
#define _RAFALE_SERVER_H_

#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <fstream>
#include <map>
#include <cstdint>
#include <rafale.h>

#include <rafale/fcgi.hh>

namespace Rafale
{
  extern std::map<std::string, Rafale::File>      files;
  extern std::map<std::string, std::string>       headers;
  extern std::map<std::string, Rafale::Cookie>    cookies;


  class Server
  {
  public:
    static void     SetVariables(const std::string &rawData, std::map<std::string, std::string> &container);
  private:
    void    GetServerData();
    void     SetGetData(std::string &rawData, const std::map<std::string, std::string> &container);
    void    GetGetData();
    void    SetContent();
    std::string     &&ParseValue(const std::string &name, const std::string &str, std::size_t offset = 0, char assign = '=', char startDelim = '"', char endDelim = '"');

    inline bool            IsFileUpload(const std::string &part)
    {
      return (part.find("filename") < part.find("\r\n\r\n"));
    }

    inline std::string     GetContent(const std::string &part)
    {
      std::size_t start = part.find("\r\n\r\n") + (sizeof("\r\n\r\n") - 1);
      return part.substr(start, part.size() - start);
    }

    void                   FileUpload(std::string &name, const std::string &content);
    std::size_t    ParseOnepart(std::size_t offset, std::size_t nextOffset);
    void    ParseMultipart(); // FULL OF FAIL GO PATCH QUICKLY BRO
    void    GetPostData();
    void    GetCookies();
    std::string    &&SetHeaders();
    std::string    &&SetCookies();
    static int remove_directory(const char *path);

    static void        Clean_(int);

  public:
    Server();

    void        Run();


    ~Server()
    {
      Rafale::Server::Clean_(0);
        // unlink(Rafale::tmpDirectory.c_str()); // CATCH SIGNAL TO DO THIS
    }
  };
}

#endif /* _RAFALE_SERVER_H_ */
