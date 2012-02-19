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
#include "layout_generator.hh"
#include <list>

class Generator
{
public:
  Generator() : controllerGenerator_(controllers_, layouts_)
  {
  }

  void  Generate(const std::string &path)
  {
    // config
    {
      std::ifstream inputConfigFile(path + "/config.cc");
      if (inputConfigFile.is_open())
        {
          std::string   configBuffer;
          std::ofstream outputConfigFile(path + "/generated/.config.cc");
          if (!outputConfigFile.is_open())
            throw ("problem while opening : " + path + "/generated/.config.cc");
          outputConfigFile << "#include \"config.hh\"\n";
          while (!inputConfigFile.eof())
            {
              getline(inputConfigFile, configBuffer);
              outputConfigFile << configBuffer + '\n';
            }
          files_[path + "/generated/.config.cc"];
        }
    }
    R0x::System::Directory      layoutsDirectory(path + "/layouts");
    {
      auto fileList = layoutsDirectory.List();
      for (auto file: fileList)
        {
          if (file.Path() != "." && file.Path() != ".." && file.Extension() == ".cchtml")
          {
            std::cout << "layout : " << file.ShortName() << std::endl;
            layoutGenerator_.Generate(path + "/layouts/" + file.Path(), path + "/generated/._layout_" + file.ShortName() + ".hh", file.ShortName());
            layouts_[file.ShortName()] = "._layout_" + file.ShortName() + ".hh";
          }
        }
    }
    {
      std::ofstream layoutsHeaderFile(path + "/generated/._layouts.hh");
      if (!layoutsHeaderFile.is_open())
        throw ("problem while opening : " + path + "/generated/._layouts.hh");

      layoutsHeaderFile << "#ifndef _LAYOUTS_H_\n";
      layoutsHeaderFile << "# define _LAYOUTS_H_\n\n";
      for (auto layout : layouts_)
        {
          layoutsHeaderFile << "#include \"" + layout.second + "\"\n";
        }
      layoutsHeaderFile << "#endif\n";
    }
    R0x::System::Directory      controllersDirectory(path + "/controllers");
    auto fileList = controllersDirectory.List();
    for (auto file: fileList)
      {
        if (file.Path() != "." && file.Path() != ".." && file.Extension() == ".hh")
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
      mainFile << "#include <cstdlib>\n"
        "#include <map>\n#include <string>\n#include <dispatcher.hh>\n\n";
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
      mainFile << "#include \"fcgi_stdio.h\"\n"
        "int main(void)\n"
        "{\n"
        "while(FCGI_Accept() >= 0)"
        "{\n"
         "try\n{\n"
          "FCGI_printf(\"Content-type: text/html\\r\\n\\r\\n\");"
          "Dispatcher dispatcher(getenv(\"SCRIPT_FILENAME\"));\n"
          "Rafale::Controller    *p = Caller::Make(dispatcher.Controller());\n"
          "FCGI_printf(\"%s\", p->Action(dispatcher.Action()).c_str());\n"
          "delete p;\n"
         "}\n"
         "catch(const char*s) {\n"
          "std::cerr << s << std::endl;"
          "}\n"
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
        "NAME = public/index.fcgi\n"
        "\n"
        "all : $(OBJ)\n"
        "	@$(CXX) $(OBJ) -l fcgi -o $(NAME)\n"
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
  std::map<std::string, std::string>        layouts_;
  struct        Null_ { };
  std::map<std::string, Null_>                        files_;
  ControllerGenerator   controllerGenerator_;
  ViewGenerator         viewGenerator_;
  LayoutGenerator         layoutGenerator_;
};


#endif /* _RAFALE_GENERATOR_H_ */
