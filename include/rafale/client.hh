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

#ifndef _RAFALE_CLIENT_H_
# define _RAFALE_CLIENT_H_

#include <utility>
#include <map>
#include <string>
#include "rafale/fcgi.hh"
#include "rafale/environment.hh"
#include "rafale/controller.hh"
#include "rafale/environment.hh"

#include <mutex>

namespace Rafale
{
  class Client
  {
  private:
    void    GetRequestDatas_();
    void    GetGetDatas_();
    void    GetCookies_();
    void    GetContent_();
    std::string     ParseValue(const std::string &name,
                                 const std::string &str,
                                 std::size_t offset = 0,
                                 char assign = '=',
                                 char startDelim = '"',
                                 char endDelim = '"');
    void    FileUpload_(std::string &name, const std::string &content);
    size_t  ParseOnepart(std::size_t offset, std::size_t nextOffset);
    void    ParseMultipart_();
    void    GetPostDatas_();
    std::string    SerializeHeaders_();
    std::string    SerializeCookies_();
  public:
    Client(FCGX_Request &request) : request_(request)
    {
      Environment::env_ = new Rafale::Environment();
      GetRequestDatas_();
      GetGetDatas_();
      GetCookies_();
      GetContent_();
      GetPostDatas_();
    }

    ~Client()
    {
      delete Environment::env_;
    }

    inline std::string     GetContent(const std::string &part)
    {
      std::size_t start = part.find("\r\n\r\n") + (sizeof("\r\n\r\n") - 1);
      return part.substr(start, part.size() - start);
    }

private:
    inline bool            IsFileUpload_(const std::string &part)
    {
      return (part.find("filename") < part.find("\r\n\r\n"));
    }

    inline std::string ContentType_(Rafale::Controller *controller)
    {
      return "Content-type: " + controller->ContentType() + "\r\n";
    }

    inline const std::string &RafaleAnswer_()
    {
      Dispatcher dispatcher(Rafale::Env().requestDatas["SCRIPT_FILENAME"]);

      Rafale::Controller    *p = Rafale::Controller::Make(dispatcher.String());
      std::string data = p->Render();
      answer_ = SerializeHeaders_();
      answer_ += SerializeCookies_();
      answer_ += ContentType_(p);
      answer_ += "\r\n";
      answer_ += data;
      return answer_;
    }

public:
    inline void GetRafaleAnswer()
    {
      fcgi_streambuf  clientBuffer(request_.out);
      std::ostream client(static_cast<std::streambuf *>(&clientBuffer));
      client << RafaleAnswer_();
    }

private:
    FCGX_Request                            &request_;
    std::string                              answer_;
  };
}

#endif /* _RAFALE_CLIENT_H_ */
