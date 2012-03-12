#ifndef _CONTROLLER_test_H_
#define _CONTROLLER_test_H_

#include <iostream>
#include <string>
#include <list>
#include "tools.hh"

controller test
{
public:
  view  Main
  {
  };

  action Main()
  {
    layout_->Disable();
    std::cout << "SERVER ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::serverDatas)
      {
        if (var.first != "CONTENT")
          std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;

    std::cout << "<br />\nGET ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::getDatas)
      {
          std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;

    std::cout << "<br />\nPOST ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::postDatas)
      {
          std::cout << "<tr><td>" << var.first << "</td><td>" << var.second << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;
    std::cout << "<br />\nFILES ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::files)
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second.Name() << "</td></tr>" << std::endl;
        var.second.Copy("/var/www/lol/static/test/" + var.second.Name());
      }
    std::cout << "</tbody></table>" << std::endl;

    std::cout << "<br />\nCOOKIES ::<br />" << std::endl;
    std::cout << "<table border=\"1\">\n"
      "<thead><tr><td>VAR</td><td>VALUE</td></tr></thead>\n"
      "<tbody>";
    for (auto var: Rafale::cookies)
      {
        std::cout << "<tr><td>" << var.first << "</td><td>" << var.second.Value() << "</td></tr>" << std::endl;
      }
    std::cout << "</tbody></table>" << std::endl;


    Rafale::cookies["PetitCookieDeTest0"] = "l'ancien contenu du PetitCookieDeTest1 est {" +
      Rafale::cookies["PetitCookieDeTest1"].Value() + "}";
    Rafale::cookies["PetitCookieDeTest1"] = "le timestamp actuel est " + Rafale::ToString(time(0));

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

  view upload
  {
  };

  action upload()
  {
    layout_->Disable();
  }
};

#endif /* _CONTROLLER_test_H_ */
