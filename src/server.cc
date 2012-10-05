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

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <utility>

#include "rafale/server.hh"
#include "rafale/fcgi.hh"

namespace Rafale
{
  std::map<std::string, std::string>       serverDatas;
  std::map<std::string, std::string>       getDatas;
  std::map<std::string, std::string>       postDatas;
  std::map<std::string, Rafale::File>      files;
  std::string                              tmpDirectory;
  std::string                              filesDirectory;
  std::string                              sessionsDirectory;
  std::map<std::string, Session*>          Session::sessionMap_;
  std::map<time_t, Session>                Session::sessions_;
  std::map<std::string, std::string>       headers;
  std::map<std::string, Rafale::Cookie>    cookies;
}

extern char **environ;

int  Rafale::cookiesMaxAge = DEFAULT_COOKIES_MAX_AGE;

void    Rafale::Server::GetServerData()
{
  char** env;
  for (env = environ; *env != 0; env++)
    {
      std::string var(*env);
      Rafale::serverDatas[var.substr(0, var.find("="))] = var.substr(var.find("=") + 1, var.size() - var.find("="));
    }
}

void     Rafale::Server::SetVariables(const std::string &rawData, std::map<std::string, std::string> &container)
{
  std::string   queryString = rawData;
  while (queryString.find("&") != std::string::npos)
    {
      container[queryString.substr(0, queryString.find("="))] = Rafale::UriDecode(queryString.substr(queryString.find("=") + 1, queryString.find("&") - queryString.find("=") - 1));
      queryString = queryString.substr(queryString.find("&") + 1, queryString.size() - queryString.find("&") - 1);
    }
  if (queryString.size()) {
    container[queryString.substr(0, queryString.find("="))] = Rafale::UriDecode(queryString.substr(queryString.find("=") + 1, queryString.size()));
  }
}

void     Rafale::Server::SetGetData(std::string &rawData, const std::map<std::string, std::string> &container)
{
  std::string   queryString = rawData;
  for (auto it = begin(container); it != end(container); ++it)
    {
      if (it != begin(container))
        rawData += "&";
      rawData += Rafale::UriEncode(it->first) + '=' + Rafale::UriEncode(it->second);
    }
}

void    Rafale::Server::GetGetData()
{
  SetVariables(Rafale::serverDatas["QUERY_STRING"], Rafale::getDatas);
}


void    Rafale::Server::SetContent()
{
  if (Rafale::serverDatas["CONTENT_LENGTH"].size())
    {
      int   length = Rafale::ToInt(Rafale::serverDatas["CONTENT_LENGTH"]);
      if (length)
        {
          char  *rawBuffer = new char [length];
          FCGI_fread(rawBuffer, length, 1, FCGI_stdin);
          Rafale::serverDatas["CONTENT"].append(rawBuffer, length);
        }
      else
        Rafale::serverDatas["CONTENT"];
    }
}

std::string     &&Rafale::Server::ParseValue(const std::string &name, const std::string &str, std::size_t offset , char assign, char startDelim, char endDelim)
{
  std::size_t startValueOffset = str.find(name + assign + startDelim, offset) + name.size() + sizeof(assign) + sizeof(startDelim);
  std::size_t endValueOffset = str.find(endDelim, startValueOffset); // TO DO : fix \delim
  return std::move(str.substr(startValueOffset, endValueOffset - startValueOffset));
}


void                   Rafale::Server::FileUpload(std::string &name, const std::string &content)
{
  Rafale::files[name] = Rafale::File(ParseValue("filename", content), ParseValue("Content-Type", content, 0, ':', ' ', '\r'),
                                     GetContent(content));
}

std::size_t    Rafale::Server::ParseOnepart(std::size_t offset, std::size_t nextOffset)
{
  std::string currentData = Rafale::serverDatas["CONTENT"].substr(offset, nextOffset - offset);
  std::string name = ParseValue("name", currentData, currentData.find("name"));
  if (IsFileUpload(currentData))
    FileUpload(name, currentData);
  else
    Rafale::postDatas[name] = GetContent(currentData);
  return nextOffset + (sizeof("\r\n--") - 1) + (Rafale::serverDatas["boundary"].size() - 1) + (sizeof("\r\n") - 1);
}


void    Rafale::Server::ParseMultipart() // FULL OF FAIL GO PATCH QUICKLY BRO
{
  std::size_t offset = (sizeof("--") - 1)+ Rafale::serverDatas["boundary"].size() + (sizeof("\r\n") - 1);
  std::size_t nextOffset = 0;

  // {
  //   std::string newContent;
  //   for (char c: Rafale::serverDatas["CONTENT"])
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
  //   Rafale::serverDatas["CONTENT_ESCAPED"] = newContent;
  // }

  int i = 0;
  while ((nextOffset = Rafale::serverDatas["CONTENT"].find("\r\n--" + Rafale::serverDatas["boundary"] + "\r\n", offset)) != std::string::npos)
    {
      ++i;
      offset = ParseOnepart(offset, nextOffset);
    }
  if ((nextOffset = Rafale::serverDatas["CONTENT"].find("\r\n--" + Rafale::serverDatas["boundary"] + "--\r\n", offset)) != std::string::npos)
    {
      ++i;
      offset = ParseOnepart(offset, nextOffset);
    }
  Rafale::serverDatas["MULTIPART_COUNTER"] = Rafale::ToString(i);
  //         -----------------------------  TO DO !!!
}

void    Rafale::Server::GetPostData()
{
  // reminder : something weird with int convertion to string
  if (Rafale::serverDatas["CONTENT_TYPE"].find("multipart/form-data; boundary=") == 0)
    {
      Rafale::serverDatas["boundary"] =
        Rafale::serverDatas["CONTENT_TYPE"].substr(
                                                   sizeof("multipart/form-data; boundary=") - 1,
                                                   Rafale::serverDatas["CONTENT_TYPE"].size()
                                                   - sizeof("multipart/form-data; boundary=") + 1);
      ParseMultipart();
      // Rafale::serverDatas["FILE"] = Rafale::serverDatas["CONTENT"];
    }
  else
    {
      SetVariables(Rafale::serverDatas["CONTENT"], Rafale::postDatas);
    }
};

void    Rafale::Server::GetCookies()
{
  if (Rafale::serverDatas["HTTP_COOKIE"].size())
    {
      std::size_t prevOffset = 0;
      for (std::size_t offset = 2; (offset = Rafale::serverDatas["HTTP_COOKIE"].find("; ", offset)); prevOffset = offset)
        {
          if (offset == std::string::npos)
            offset = Rafale::serverDatas["HTTP_COOKIE"].size();
          std::size_t equal = Rafale::serverDatas["HTTP_COOKIE"].find("=", prevOffset);
          (Rafale::cookies[Rafale::UriDecode(Rafale::serverDatas["HTTP_COOKIE"].substr(prevOffset, equal - prevOffset))]
           = Rafale::UriDecode(Rafale::serverDatas["HTTP_COOKIE"].substr(equal + 1, offset - equal - 1))).modified = false;

          if (offset == Rafale::serverDatas["HTTP_COOKIE"].size())
            break;
          offset += 2;
        }
    }
}

std::string    &&Rafale::Server::SetHeaders()
{
  std::string result;
  for (auto header: Rafale::headers)
    {
      result +=  Rafale::UriEncode(header.first) + ": " + Rafale::UriEncode(header.second) + "\r\n";
    }
  return std::move(result);
}

std::string    &&Rafale::Server::SetCookies()
{
  std::string result;
  for (auto cookie: Rafale::cookies)
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
  return std::move(result);
}

int Rafale::Server::remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;
      r = 0;
      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;
          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }
          len = path_len + strlen(p->d_name) + 2;
          buf = (char*)malloc(len);
          if (buf)
          {
             struct stat statbuf;
             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }
   if (!r)
   {
      r = rmdir(path);
   }
   return r;
}

sql::Driver *Rafale::BasicModel::driver_  = sql::mysql::get_driver_instance();
std::string Rafale::BasicModel::host_ = Rafale::config["db.host"];
std::string Rafale::BasicModel::user_ = Rafale::config["db.user"];
std::string Rafale::BasicModel::dataBase_ = Rafale::config["db.database"];
std::string Rafale::BasicModel::password_ = Rafale::config["db.password"];

std::shared_ptr<sql::Connection>    Rafale::BasicModel::con_;


void LogSegfault(int )
{
  std::fstream  logFile;
  logFile.open("/var/log/rafale/error.log", std::fstream::out | std::fstream::app);
  if (logFile.is_open())
    {
      logFile << "Putain de segfault" << std::endl;
      logFile << "Dump::" << std::endl;
      Rafale::serverDatas.clear();
      Rafale::getDatas.clear();
      Rafale::postDatas.clear();

      std::cout << "Server Datas" << std::endl;
      for (auto data: Rafale::serverDatas)
        {
          logFile << "[" << data.first << "]=\"" << data.second << "\"" << std::endl;
        }
      std::cout << "Get Datas" << std::endl;
      for (auto data: Rafale::getDatas)
        {
          logFile << "[" << data.first << "]=\"" << data.second << "\"" << std::endl;
        }
      std::cout << "Post Datas" << std::endl;
      for (auto data: Rafale::postDatas)
        {
          logFile << "[" << data.first << "]=\"" << data.second << "\"" << std::endl;
        }
    }
  exit(2);
}

void    Rafale::Server::Clean_(int)
{
  remove_directory(Rafale::tmpDirectory.c_str());
  exit(2);
}

Rafale::Server::Server()
{
  signal(SIGSEGV, &LogSegfault);
  // signal(SIGTERM, &Rafale::Server::Clean_);
  curl_global_init(CURL_GLOBAL_ALL);
  srand(static_cast<unsigned int>(time(0)) * getpid());
  if (!Rafale::Exist("/var/log/rafale"))
    mkdir("/var/log/rafale", 0770);
  Rafale::tmpDirectory = "/tmp/rafale" + Rafale::ToString(rand());
  Rafale::filesDirectory = Rafale::tmpDirectory + "/files";
  Rafale::sessionsDirectory = Rafale::tmpDirectory + "/sessions";

  mkdir(Rafale::tmpDirectory.c_str(), 0770);
  (void)chown(Rafale::tmpDirectory.c_str(), 1000, 33);
  mkdir(Rafale::sessionsDirectory.c_str(), 0770);
  (void)chown(Rafale::sessionsDirectory.c_str(), 1000, 33);
  mkdir(Rafale::filesDirectory.c_str(), 0770);
  (void)chown(Rafale::filesDirectory.c_str(), 1000, 33);
  std::setlocale(LC_ALL, "fr_FR.UTF-8");
  std::setlocale(LC_NUMERIC, "fr_FR.UTF-8");
  std::setlocale(LC_TIME, "fr_FR.UTF-8");
}


std::string Rafale::Server::ContentType(Rafale::Controller *controller)
{
  return "Content-type: " + controller->ContentType() + "\r\n"; 
}

void    Rafale::Server::Run()
{
  while(FCGI_Accept() >= 0)
    {
      try {
        Rafale::serverDatas.clear();
        Rafale::getDatas.clear();
        Rafale::postDatas.clear();
        Rafale::cookies.clear();
        Rafale::files.clear();
        Rafale::Session::Clean();
        GetServerData();
        GetGetData();
        GetCookies();
        SetContent();
        GetPostData();
        Rafale::BasicModel::UseDatabase();

        Dispatcher dispatcher(getenv("SCRIPT_FILENAME"));

        Rafale::Controller    *p = Rafale::Controller::Make(dispatcher.String());

        std::string data = p->Render();

        FCGI_printf("%s", SetHeaders().c_str());
        FCGI_printf("%s", SetCookies().c_str());
        FCGI_printf(ContentType(p).c_str());
        FCGI_printf("\r\n");
        FCGI_printf("%s", data.c_str());
        delete p;
      }
      catch(const char*s) {
        std::fstream  logFile;
        logFile.open("/var/log/rafale/error.log", std::fstream::out | std::fstream::app);
          if (logFile.is_open())
            logFile << "error : " << s << std::endl;
      }
      catch(sql::SQLException excep) {
        std::fstream  log;
        log.open("/var/log/rafale/sql.log", std::fstream::out | std::fstream::app);
        if (log.is_open())
          log << excep.what() << std::endl;
      }
      catch (std::exception& e)
        {
          std::fstream  logFile;
          logFile.open("/var/log/rafale/error.log", std::fstream::out | std::fstream::app);
          if (logFile.is_open())
            logFile << "exception caught: " << e.what() << std::endl;
        }
    }
}
