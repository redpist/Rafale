#ifndef _ABSTRACT_H_
#define _ABSTRACT_H_

#include <map>
#include <string>
#include <vector>
#include <ctime>


namespace Rafale
{
  class DateTime
  {
  public:
    DateTime() { }
    ~DateTime() { }
  private:
    time_t      timestamp;
  };

  typedef std::vector<std::string>              Array;
  typedef std::string                           String;
  typedef std::map<std::string, std::string>    Config;
}

#endif /* _ABSTRACT_H_ */
