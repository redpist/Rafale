#ifndef _USERS_H_
#define _USERS_H_

#include <iostream>
#include <string>
#include <list>
#include "models/Form.hh"


controller Users
{
public:
  view  Main
  {
    Form form;
  };

  action Main()
  {
    Form::FormElement test = {
      {"type", "checkbox"},
      {"title", "Keep me signed-in."}
    };
    view.form.addElement("test", test);
    
//    auto likes = Rafale::SQL::Query<Users>({ 
//     {"userId", Rafale::ToInt(Rafale::getDatas["id"])} 
//    });
//    view.str = "view index =)";
//    view.str = view.str + "/view index =)";
//   view.lulz = "plimuk";
  }
};

#endif /* _USERS_H_ */
