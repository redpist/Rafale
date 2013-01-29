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

#ifndef _RAFALE_CONTROLLER_H_
#define _RAFALE_CONTROLLER_H_

#include <iostream>
#include <map>

#include "rafale/depends.hh"
#include <list>
#include <string>
#include <sstream>
#include "rafale/dispatcher.hh"
#include <dirent.h>
#include <clocale>
#include "rafale/fcgi.hh"
#include "rafale/tools.hh"
#include "rafale/controller_static_members_policy.hh"

namespace Rafale
{
  class BasicController
  {
  protected:
    BasicController() { }
    Controllers::RendererType_ render_;
    template <class DependentController, template <class Controller> class Controller>
    friend struct Controllers::Depends;
    virtual void        Control(void) = 0;
    virtual void        Print(void) = 0;

    inline void        Control_(void)
    {
      Control();
    }

    inline void        Render_(void)
    {
      Print();
    }

  public:
    inline std::string        Render() {
      std::stringstream _outputBuffer_;
      std::streambuf *_saveCoutRdbuf_;
      _saveCoutRdbuf_ = std::cout.rdbuf();
      std::cout.rdbuf(_outputBuffer_.rdbuf());
      Control();
      render_();
      std::cout.rdbuf(_saveCoutRdbuf_);
      return _outputBuffer_.str();
    }
  };

  class Controller_ : public BasicController
  {
  public:

    virtual ~Controller_() { };

    static const std::string &ContentType()
    {
      return contentType;
    }

  public:

  protected:

    Controller_()
    {
      render_ = [this] (void) -> void { this->Render_(); };
    }

    template <class DependentController, template <class Controller> class Controller>
    friend struct Controllers::Depends;

  private:
    template    <typename ChildController>
    static Controller_     *Make_(const std::list<std::string> &scopes, const std::string &name)
    {
      Rafale::Policy::ControllerStaticMembers<ChildController>::SetScopes_(scopes);
      Rafale::Policy::ControllerStaticMembers<ChildController>::SetName_(name);
      return new ChildController();
    }

    static std::string contentType;

  public:

    class Default
    {
      static std::function<Controller_* (void)> error404_;
    public:
      template <typename Error404Controller>
      static std::function<Controller_* (void)> Maker404()
      {
        return []() {return Controller_::Make_<Error404Controller>({}, "404");};
      }

      template <typename Error404Controller>
      static void SetError404()
      {
        error404_ = Maker404<Error404Controller>();
      }
      friend class Controller_;
    };

    static inline Controller_     *Make(const std::string &str)
    {
      std::fstream  log;
      log.open("/var/log/rafale/make.log", std::fstream::out);
      if (log.is_open())
        {
          log << str << std::endl;
          log.close();
        }

      if (Controller_::controllers_[str])
        return (Controller_::controllers_[str])();
      else
        return Default::error404_();
    }
  private:
    static std::list<std::string> RAIIscopes_;
    typedef std::map<std::string, std::function<Controller_* (void)> > ControllersMap_;
    static ControllersMap_ controllers_;
    template    <typename ChildController>
    friend void        Register(const std::string &str);

    friend struct Scope;
  };

  class ContainerController : public BasicController
  {
  protected:
    ContainerController() { }
    Controllers::RendererType_ dependentRender_;

    inline void        RenderContained() { dependentRender_(); }

    template <class DependentController, template <class Controller> class Controller>
    friend struct Controllers::Depends;
  };

  template <class T>
  class Controller : public Controller_, public Rafale::Policy::ControllerStaticMembers<T>
  {
  };

  struct Scope
  {
    Scope(const std::string &str)
    {
      Controller_::RAIIscopes_.push_back(str);
    }

    ~Scope()
    {
      Controller_::RAIIscopes_.pop_back();
    }
  };

  template    <typename ChildController>
  static void        Register(const std::string &str)
  {
    std::string route;
    for (auto &scope: Controller_::RAIIscopes_)
      {
        route += scope;
      }
    Controller_::controllers_[route + str] = [=]() { return Controller_::Make_<ChildController>(Controller_::RAIIscopes_, str); };
    Debug::Log("Register : \"" + route + str + "\"", "log");
  }

}
#endif /* _RAFALE_CONTROLLER_H_ */
