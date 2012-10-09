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

#ifndef _RAFALE_EXAMPLE_CONTROLLER_TEST_H_
#define _RAFALE_EXAMPLE_CONTROLLER_TEST_H_

#include "rafale.h"

class Test : public Rafale::Controller
{
  #include "views/Test.inc"

  void Testing()
  {

    std::cout << "SERVER ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
     for (auto var: Rafale::Env().requestDatas)
      {
        if (var.first != "CONTENT")
          std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;

    std::cout << "<br />\nGET ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::Env().getDatas)
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;

    std::cout << "<br />\nPOST ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::Env().postDatas)
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;
    std::cout << "<br />\nFILES ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::Env().files)
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second.Name() << "</td></tr>" << std::endl;
        var.second.Copy("/var/www/leeaarn/static/test/" + var.second.Name());
      }
    std::cout << "</tbody></table>" << std::endl;


    std::cout << "<br />\nCOOKIES ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::Env().cookies)
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second.Value() << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;

    Rafale::Env().cookies["PetitCookieDeTest0"] = "l'ancien contenu du PetitCookieDeTest1 est {" +
      Rafale::Env().cookies["PetitCookieDeTest1"].Value() + "}";
    Rafale::Env().cookies["PetitCookieDeTest1"] = "le timestamp actuel est " + Rafale::ToString(time(0));

    Rafale::Session &session = Rafale::Session::Get();

    std::cout << "<br />\nSESSIONS ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto &var: session.Datas())
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;

    session["TEST"] = "Hey Ho ! Timestamp=" + Rafale::ToString(time(0));
  }

  void Control()
  {
  }

};

#endif /* _RAFALE_EXAMPLE_CONTROLLER_TEST_H_ */
