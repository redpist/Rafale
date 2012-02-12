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
            controllerGenerator_.Generate(path + "/controllers/" + file.Path(), path + "/generated/._controller_" + file.Path() );
          }
      }
    for (auto controller : controllers_)
      {
        std::cout << "controller : " << controller.first << std::endl;
        // R0x::System::Directory      controllersDirectory();
        for (auto action : controller.second)
          {
            files_[path + "/generated/._view_" + controller.first + "_" + action + ".cc"];
            viewGenerator_.Generate(controller.first, action,
                                   path + "/views/" + controller.first + "/"
                                   + action + ".cchtml",
                                   path + "/generated/._view_" + controller.first
                                   + "_" + action + ".cc");
            std::cout << "+ action : " << action << std::endl;
          }
      }
    {
      files_[path + "/generated/.main.cc"];
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
        "std::cout << \"Content-type: text/html\\n\";\n"
        "try\n{\n"
        "Dispatcher dispatcher(argv[1]);\n"
        "std::string s;"
        "Rafale::Controller    *p = Caller::Make(dispatcher.Controller());\n"
        "std::cout << p->Action(dispatcher.Action());\n"
        "}\n"
        "catch(const char*s) {\n"
        "std::cerr << s << std::endl;"
        "}\n"
        "}\n";
    }

    {
      std::ofstream makefile(path + "/Makefile");
      makefile <<
        "##############################################\n"
        "# This is a Generated Makefile dont edit it. #\n"
        "##############################################\n"
        "\n"
        "CXX							= g++-4.6\n"
        "CXXFLAGS				= -std=c++0x -O3 -Wall -Wextra\n"
        "INCPATH					= ~/dev/Rafale/include/\n"
        "SOURCES					= ";
        for (auto file: files_)
          {
            makefile << file.first << " ";
          }
      makefile << "\n"
        "OBJ	=	$(SOURCES:.cc=.o)\n"
        "NAME = public/index.cgi\n"
        "\n"
        "all : $(OBJ)\n"
        "	@$(CXX) $(OBJ) -o $(NAME)\n"
        "	@echo 'Building target : ' $(NAME)\n"
        "\n"
        ".cc.o :\n"
        "	@echo 'Building objet  : ' $@\n"
        "	@$(CXX) $(CXXFLAGS) -I $(INCPATH) -c $< -o $@\n"
        "\n"
        "clean:\n"
        "	$(RM) $(OBJ)\n"
        "\n"
        "fclean : clean\n"
        "	$(RM) $(NAME)\n"
        "\n"
        "re: fclean all\n"
        "\n"
        ".PHONY: all clean fclean re\n";
    }
}

  ~Generator() { }
private:
  std::map<std::string, std::list<std::string> >        controllers_;
  struct        Null_ { };
  std::map<std::string, Null_>                        files_;
  ControllerGenerator   controllerGenerator_;
  ViewGenerator         viewGenerator_;
};


#endif /* _RAFALE_GENERATOR_H_ */
