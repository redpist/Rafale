#ifndef _DEFAULT_H_
#define _DEFAULT_H_

#include <iostream>
#include <string>
#include <list>

controller Default
{
public:
  view  Main
  {
    std::string str;
  };

  action Main()
  {
    view.str = "view index =)";
    view.str = view.str + "/view index =)";
  }

  view  Test
  {
    std::list<std::string> list;
  };

  action Test()
  {
    view.list.push_back("Element 0");
    view.list.push_back("Element 1");
    view.list.push_back("Element 2");
    view.list.push_back("Element 42");
  }

};

#endif /* _DEFAULT_H_ */
