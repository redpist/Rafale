#ifndef _ABSTRACT_H_
#define _ABSTRACT_H_

#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include "tools.hh"

namespace Rafale
{
  class DateTime
  {
  public:

    DateTime(const std::string &dateTime, const std::string &format = "%Y-%m-%d %H:%M:%S") : tm_(), isValid_(false)
    {
      if (strptime(dateTime.c_str(), format.c_str(), &tm_))
        isValid_ = true;
    }

    DateTime(const time_t &time = Rafale::Now()) : tm_(*localtime(&time))
    {
    }

    bool        IsValid()
    {
      return isValid_;
    }

    std::string ToString(const std::string &format = "%Y-%m-%d %H:%M:%S")
    {
      char buf[255];
      strftime(buf, sizeof(buf), format.c_str(), &tm_);
      return buf;
    }

    bool        operator<(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) < mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    bool        operator>(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) > mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    bool        operator==(const Rafale::DateTime &oth)
    {
      return mktime(&tm_) == mktime(&const_cast<Rafale::DateTime&>(oth).tm_);
    }

    ~DateTime() { }
  private:
    struct tm   tm_;
    bool        isValid_;
  };

  typedef std::vector<std::string>              Array;
  typedef std::string                           String;
  typedef std::map<std::string, std::string>    Config;
}

#endif /* _ABSTRACT_H_ */
