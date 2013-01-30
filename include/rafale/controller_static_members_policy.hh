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

#ifndef _RAFALE_CONTROLLER_STATIC_MEMBER_POLICY_H_
#define _RAFALE_CONTROLLER_STATIC_MEMBER_POLICY_H_

#include <list>
#include <string>

namespace Rafale
{
  class Controller_;
  namespace Policy
  {
    template <class T>
    class ControllerStaticMembers
    {
    private:
      static void SetScopes_(const std::list<std::string> &scopes)
      {
        scopes_ = scopes;
      }

      static void SetName_(const std::string &name)
      {
        name_ = name;
      }

      friend class Rafale::Controller_;

    protected:
      static std::list<std::string>    scopes_;
      static std::string               name_;

      static const std::list<std::string> &GetScopes()
      {
        return scopes_;
      }

      static std::string GetName()
      {
        return name_;
      }

      static std::string GetPath()
      {
        std::string path;
        for (auto &scope: scopes_)
          {
            path += scope;
          }
        path += name_;
        return path;
      }

    };
  }
}


template <class T>
std::list<std::string>    Rafale::Policy::ControllerStaticMembers<T>::scopes_;

template <class T>
std::string               Rafale::Policy::ControllerStaticMembers<T>::name_;


#endif /* _RAFALE_CONTROLLER_STATIC_MEMBER_POLICY_H_ */
