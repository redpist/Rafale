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
            controllerGenerator_.Generate(path + "/controllers/" + file.Path(),path + "/generated/._controller_" + file.Path() );
          }
      }
    for (auto controller : controllers_)
      {
        std::cout << "controller : " << controller.first << std::endl;
        // R0x::System::Directory      controllersDirectory();
        for (auto action : controller.second)
          {
            viewGenerator_.Generate(controller.first, action,
                                   path + "/views/" + controller.first + "/"
                                   + action + ".cchtml",
                                   path + "/generated/._view_" + controller.first
                                   + "_" + action + ".cc");
            std::cout << "+ action : " << action << std::endl;
          }
      }
    std::ofstream mainFile(path + "/generated/.main.cc");
    mainFile << "#include <map>\n#include <string>\n#include <dispatcher.hh>\n\n";
    for (auto controller : controllers_)
      {
        mainFile << "#include \"._controller_" + controller.first + ".hh\"\n";
        mainFile << "Rafale::Controller *Make"
          + controller.first + "()\n"
          "{\n"
            "return new _Controller_" + controller.first + "();\n"
          "}\n";

      }
    mainFile << "class Caller\n{\nprivate:\n";
    mainFile << "static std::map<std::string, Rafale::Controller *(*)()> array;\n";
    mainFile << "public :\n"
      "static Rafale::Controller    *Make(const std::string &name)\n"
      "{\n"
      "if (Caller::array[name])"
      "return (Caller::array[name])();\n"
      "else\n"
      "throw(\"Controller Not Found.\");\n"
      "}\n"
      "};\n\n";
    mainFile << "std::map<std::string, Rafale::Controller *(*)()> Caller::array = {\n";
    for (auto controller : controllers_)
      {
        mainFile << "{\"" + controller.first + "\", &" + "Make" + controller.first + "},\n";
      }
    mainFile << "};\n";
    mainFile << "int main(int, char *argv[])\n"
      "{\n"
      "try\n{\n"
      "Dispatcher dispatcher(argv[1]);\n"
      "Rafale::Controller    *p = Caller::Make(dispatcher.Controller());\n"
      "p->Action(dispatcher.Action());\n"
      "}\n"
      "catch(const char*s) {\n"
      "std::cerr << s << std::endl;"
      "}\n"
      "}\n";
}

  ~Generator() { }
private:
  std::map<std::string, std::list<std::string> >        controllers_;
  ControllerGenerator   controllerGenerator_;
  ViewGenerator         viewGenerator_;
};


#endif /* _RAFALE_GENERATOR_H_ */
