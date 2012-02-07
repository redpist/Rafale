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

#ifndef _RAFALE_GENERATOR_H_
#define _RAFALE_GENERATOR_H_

#include <r0x/system/directory.h>
#include "controller_generator.hh"
#include "view_generator.hh"
#include <list>

class Generator
{
public:
  Generator() : controllerGenerator_(controllers_)
  {
  }

  void  Generate(const std::string &path)
  {
    R0x::System::Directory      controllersDirectory(path + "/controllers");
    auto fileList = controllersDirectory.List();
    for (auto file: fileList)
      {
        if (file.Path() != "." && file.Path() != "..")
          {
            std::cout << file.Path() << std::endl;
            controllerGenerator_.Generate(path + "/controllers/" + file.Path(),path + "/generated/._controller_" + file.Path() );
  // std::ifstream *viewFile_;
  //   std::ifstream viewFile(fileName);
  //   viewFile_ = &viewFile;

  //   if (!viewFile_->is_open())
  //     throw ("no view file");
          }
      }
    for (auto controller : controllers_)
      {
        std::cout << "controller : " << controller.first << std::endl;
        for (auto action : controller.second)
          {
            std::cout << "action : " << action << std::endl;
          }
      }
  }

  ~Generator() { }
private:
  std::map<std::string, std::list<std::string> >        controllers_;
  ControllerGenerator   controllerGenerator_;
  ViewGenerator         ViewGenerator_;
};


#endif /* _RAFALE_GENERATOR_H_ */
