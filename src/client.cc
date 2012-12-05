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

#include <memory>
#include "rafale/client.hh"
#include "rafale/tools.hh"

void  Rafale::Client::GetRequestDatas_()
{
  char** environ;
  for (environ = request_.envp; *environ != 0; environ++)
    {
      std::string var(*environ);
      std::size_t offset = var.find("=");
      Rafale::Env().requestDatas[var.substr(0, offset)] =
        var.substr(offset + 1, var.size() - offset);
    }
}

void    Rafale::Client::GetGetDatas_()
{
  Rafale::ExplodeVariables(Rafale::Env().requestDatas["QUERY_STRING"], Rafale::Env().getDatas);
}

void    Rafale::Client::GetCookies_()
{
  if (Rafale::Env().requestDatas["HTTP_COOKIE"].size())
    {
      std::size_t prevOffset = 0;
      for (std::size_t offset = 2; (offset = Rafale::Env().requestDatas["HTTP_COOKIE"].find("; ", offset)); prevOffset = offset)
        {
          if (offset == std::string::npos)
            offset = Rafale::Env().requestDatas["HTTP_COOKIE"].size();
          std::size_t equal = Rafale::Env().requestDatas["HTTP_COOKIE"].find("=", prevOffset);
          (Rafale::Env().cookies[Rafale::UriDecode(Rafale::Env().requestDatas["HTTP_COOKIE"].substr(prevOffset, equal - prevOffset))] = Rafale::UriDecode(Rafale::Env().requestDatas["HTTP_COOKIE"].substr(equal + 1, offset - equal - 1))).modified = false;
          if (offset == Rafale::Env().requestDatas["HTTP_COOKIE"].size())
            break;
          offset += 2;
        }
    }
}

void    Rafale::Client::GetContent_()
{
  if (Rafale::Env().requestDatas["CONTENT_LENGTH"].size())
    {
      int   length = Rafale::ToInt(Rafale::Env().requestDatas["CONTENT_LENGTH"]);
      if (length)
        {
          char  *rawBuffer = new (std::nothrow) char [length];
          if (!rawBuffer)
            goto emptyContent;

          fcgi_streambuf  clientBuffer(request_.in);
          std::istream client(static_cast<std::streambuf *>(&clientBuffer));

          client.read(rawBuffer, length);
          Rafale::Env().requestDatas["CONTENT"].append(rawBuffer, length);
        }
      else
emptyContent:
        Rafale::Env().requestDatas["CONTENT"];
    }
}


std::string     Rafale::Client::ParseValue(const std::string &name,
                                             const std::string &str,
                                             std::size_t offset,
                                             char assign,
                                             char startDelim,
                                             char endDelim)
{
  std::size_t startValueOffset = str.find(name + assign + startDelim, offset) + name.size() + sizeof(assign) + sizeof(startDelim);
  std::size_t endValueOffset = str.find(endDelim, startValueOffset); // TO DO : fix \delim
  return str.substr(startValueOffset, endValueOffset - startValueOffset);
}


void                   Rafale::Client::FileUpload_(std::string &name, const std::string &content)
{
  Rafale::Env().files[name] = std::move(Rafale::File(ParseValue("filename", content),
                             ParseValue("Content-Type", content, 0, ':', ' ', '\r'),
                             GetContent(content)));
}

size_t    Rafale::Client::ParseOnepart(std::size_t offset, std::size_t nextOffset)
{
  std::string currentData = Rafale::Env().requestDatas["CONTENT"].substr(offset, nextOffset - offset);
  std::string name = ParseValue("name", currentData, currentData.find("name"));
  if (IsFileUpload_(currentData))
    FileUpload_(name, currentData);
  else
    Rafale::Env().postDatas[name] = GetContent(currentData);
  return nextOffset + (sizeof("\r\n--") - 1) + (Rafale::Env().requestDatas["boundary"].size() - 1) + (sizeof("\r\n") - 1);
}

void    Rafale::Client::ParseMultipart_() // FULL OF FAIL GO PATCH QUICKLY BRO
{
  std::size_t offset = (sizeof("--") - 1)+ Rafale::Env().requestDatas["boundary"].size() + (sizeof("\r\n") - 1);
  std::size_t nextOffset = 0;

  // {
  //   std::string newContent;
  //   for (char c: Rafale::Env().requestDatas["CONTENT"])
  //     {
  //       if (c < 32)
  //         {
  //           newContent += '\\';
  //           if (c == 13)
  //             newContent += 'r';
  //           else if (c == 10)
  //             newContent += 'n';
  //           else
  //             newContent += Rafale::ToString(static_cast<int>(c));
  //         }
  //       else
  //         newContent += c;
  //     }
  //   Rafale::Env().requestDatas["CONTENT_ESCAPED"] = newContent;
  // }

  int i = 0;
  while ((nextOffset = Rafale::Env().requestDatas["CONTENT"].find("\r\n--" + Rafale::Env().requestDatas["boundary"] + "\r\n", offset)) != std::string::npos)
    {
      ++i;
      offset = ParseOnepart(offset, nextOffset);
    }
  if ((nextOffset = Rafale::Env().requestDatas["CONTENT"].find("\r\n--" + Rafale::Env().requestDatas["boundary"] + "--\r\n", offset)) != std::string::npos)
    {
      ++i;
      offset = ParseOnepart(offset, nextOffset);
    }
  Rafale::Env().requestDatas["MULTIPART_COUNTER"] = Rafale::ToString(i);
  //         -----------------------------  TO DO !!!
}

void    Rafale::Client::GetPostDatas_()
{
  // reminder : something weird with int convertion to string
  if (Rafale::Env().requestDatas["CONTENT_TYPE"].find("multipart/form-data; boundary=") == 0)
    {
      Rafale::Env().requestDatas["boundary"] =
        Rafale::Env().requestDatas["CONTENT_TYPE"].substr(
                                            sizeof("multipart/form-data; boundary=") - 1,
                                            Rafale::Env().requestDatas["CONTENT_TYPE"].size()
                                              - sizeof("multipart/form-data; boundary=") + 1);
      ParseMultipart_();
      // Rafale::Env().requestDatas["FILE"] = Rafale::Env().requestDatas["CONTENT"];
    }
  else
    {
      Rafale::ExplodeVariables(Rafale::Env().requestDatas["CONTENT"], Rafale::Env().postDatas);
    }
};

std::string    Rafale::Client::SerializeHeaders_()
{
  std::string result;
  for (auto header: Rafale::Env().headers)
    {
      result +=  Rafale::UriEncode(header.first) + ": " + Rafale::UriEncode(header.second) + "\r\n";
    }
  return result;
}

std::string    Rafale::Client::SerializeCookies_()
{
  std::string result;
  for (auto cookie: Rafale::Env().cookies)
    {
      if (cookie.second.modified)
        {
          result += cookie.second.Serialize(cookie.first);
        }
    }
  std::fstream  log;
  log.open("/var/log/rafale/cookies.log", std::fstream::out | std::fstream::app);
  if (log.is_open() && result.size())
    log << result << std::endl;
  return result;
}
