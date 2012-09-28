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

#ifndef _RAFALE_DEPENDS_H_
#define _RAFALE_DEPENDS_H_

#include <iostream>
#include <functional>

#include <type_traits>
#include <typeinfo>

namespace Rafale
{
  class Controller;
  class ContainerController;


  namespace Controllers
  {
    typedef std::function<void (void)> RendererType_;

    template <class DependentController, template <class DependentController> class Controller>
    struct Depends
    {
    protected:
      Controller<DependentController>   container;

      Depends()
      {
         if (!container.render_)
          container.render_ = [this] (void) -> void {
            // container.Control_();
            container.Render_();
          };
        static_cast<DependentController*>(this)->render_ = [=] (void) -> void
          {
            container.dependentRender_ = [this] (void) -> void {
              static_cast<DependentController*>(this)->Render_();
            };
            container.Control_();
            container.render_();
          };
      }
    };
  }
}

#endif /* _RAFALE_DEPENDS_H_ */
