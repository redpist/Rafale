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
#include "rafale/client.hh"
#include "rafale/r0x/system/directory.h"
namespace Rafale
{
  std::string                              tmpDirectory;
  std::string                              filesDirectory;
  std::string                              sessionsDirectory;
}

int  Rafale::cookiesMaxAge = DEFAULT_COOKIES_MAX_AGE;

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

sql::Driver *Rafale::BasicModel::driver_  = sql::mysql::get_driver_instance();
std::string Rafale::BasicModel::host_ = Rafale::config["db.host"];
std::string Rafale::BasicModel::user_ = Rafale::config["db.user"];
std::string Rafale::BasicModel::dataBase_ = Rafale::config["db.database"];
std::string Rafale::BasicModel::password_ = Rafale::config["db.password"];

std::shared_ptr<sql::Connection>    Rafale::BasicModel::con_;

void LogSegfault(int)
{
  Debug::Log("Segfault");
  exit(9);
}

void    Rafale::Server::Clean_(int)
{
  R0x::System::Directory directory(Rafale::tmpDirectory);
  directory.RecursiveRemove();
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
  (void)chown(Rafale::tmpDirectory.c_str(), -1, 33);
  mkdir(Rafale::sessionsDirectory.c_str(), 0770);
  (void)chown(Rafale::sessionsDirectory.c_str(), -1, 33);
  mkdir(Rafale::filesDirectory.c_str(), 0770);
  (void)chown(Rafale::filesDirectory.c_str(), -1, 33);
}


std::size_t Rafale::Server::nbOfThreads = 50;

void Rafale::Server::WorkingLoop_(std::mutex &mutex)
{
  FCGX_Request request;
  FCGX_InitRequest(&request, 0, 0);

  while (true)
  {
    {
      std::lock_guard<std::mutex> lockGuard(mutex);
      if (FCGX_Accept_r(&request) < 0)
        break;
    }
    Rafale::Client client(request);
    client.GetRafaleAnswer();
    FCGX_Finish_r(&request);
  }
}

void    Rafale::Server::Run()
{
  Debug::Log("Run", "log");
  FCGX_Init();
  std::mutex mutex;
  for (size_t i = 0; i < nbOfThreads; ++i)
  {
    Debug::Log("thread :" + Rafale::ToString(i), "log");
    threads_.push_back(std::thread([this, &mutex]() -> void { this->WorkingLoop_(mutex); }));
  }
  for (auto &thread: threads_)
    thread.join();
  Debug::Log("End Of Rafale::Server::Run()", "log");
}
