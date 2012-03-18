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

#ifndef _RAFALE_SESSIONS_H_
#define _RAFALE_SESSIONS_H_

#include "cookies.hh"
#include <ctime>
#include <map>
#include <string>

namespace Rafale
{
  class Session
  {
    Session(const std::string &id, time_t expire) : id_(id), expire_(expire), exist_(false), upToDate_(true)
    {
    }

  public:
    ~Session()
    {
      Destroy();
      Session::sessionMap_[id_] = 0;
    }

    std::string &operator[](const std::string &key)
    {
      saveUpToDate_ = false;
      return datas_[key];
    }

    bool        IsUpToDate() { return upToDate_; }

    void        Destroy()
    {
      expire_ = 0;
      datas_.clear();
      std::ofstream tmpFile(Rafale::sessionsDirectory + "/_session_" + id_);
      if (tmpFile.is_open())
        {
          tmpFile.close();
          remove(std::string(Rafale::sessionsDirectory + "/_session_" + id_).c_str());
        }
    }

    bool        IsValid()
    {
      return exist_;
    }

    static Session &Get()
    {
      if (Rafale::cookies["sessionid"].Value().size()
          && Session::sessionMap_.find(Rafale::cookies["sessionid"].Value()) != end(Session::sessionMap_))
        {
          if (Session::sessionMap_[Rafale::cookies["sessionid"].Value()])
            {
              auto it = Session::sessionMap_.find(Rafale::cookies["sessionid"].Value());
              it->second->Load();
              it->second->expire_ = time(0) + 1800;
              return *(it->second);
            }
          Rafale::serverDatas["SESSION_EXPIRE"] = "true";
        }
      std::string id = Rafale::ToString(rand()) + Rafale::ToString(rand()) + Rafale::ToString(rand());
      Rafale::cookies["sessionid"] = id;
      time_t expire = time(0) + 1800;
      auto session = sessions_.insert(std::pair<time_t, Session>(expire, Session(id, expire)));
      sessionMap_.insert(std::pair<std::string, Session*>(id, &(session.first->second)));
      return session.first->second;
    }

    static void Clean()
    {
      auto it = begin(sessions_);
      for (; (it != end(sessions_)) && (it->first > time(0)); ++it)
        {
          it->second.Save();
        }
      sessions_.erase(it, end(sessions_));
    }

    const std::map<std::string, std::string> &Datas()
    {
      return datas_;
    }
  private:
    void        Save()
    {
      std::ofstream tmpFile(Rafale::sessionsDirectory + "/_session_" + id_);
      if (tmpFile.is_open())
        {
          for (auto &data: datas_)
            {
              if (data.first.size() && data.second.size())
                tmpFile << Rafale::UriEncode(data.first) << "\n" << Rafale::UriEncode(data.second) << "\n";
            }
        }
      upToDate_ = false;
      datas_.clear();
    }

    void        Load()
    {
      std::ifstream tmpFile(Rafale::sessionsDirectory + "/_session_" + id_);
      if (tmpFile.is_open())
        {
          while (!tmpFile.eof())
            {
              std::string   name;
              getline(tmpFile, name);
              std::string   value;
              getline(tmpFile, value);
              datas_[Rafale::UriDecode(name)] = Rafale::UriDecode(value);
              std::ofstream tmpFile2(Rafale::sessionsDirectory + "/SAVE");
              tmpFile2 << "PLI";
            }
        }
      upToDate_ = true;
    }

    std::string id_;
    time_t      expire_;
    bool        exist_;
    bool        saveUpToDate_;
    bool        upToDate_;
    std::map<std::string, std::string>  datas_;

    static std::map<std::string, Session*>              sessionMap_;
    static std::map<time_t, Session>                    sessions_;
  };

}

#endif /* _RAFALE_SESSIONS_H_ */
