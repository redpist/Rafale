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
    std::string lulz;
  };

  action Main()
  {
    view.str = "view index =)";
    view.str = view.str + "/view index =)";
    view.lulz = "plimuk";
  }

};

#endif /* _DEFAULT_H_ */
