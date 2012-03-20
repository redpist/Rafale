/**************************************
 * This is a temporary misplacement   *
 **************************************/

#ifndef _TOOLS_HH_
# define _TOOLS_HH_ 
# include "rafale.h"

class Tools {
  public:
  // HHAA C'EST MOCHE, MET MOI AILLEURS, JE SOUFFRE
    static std::string extract(std::map<std::string, std::string> &data, const std::string &key) {
      std::string val = data[key];
      data.erase(key);
      return val;
    }    
    static std::string buidlAttributes(const std::map<std::string, std::string> &attrs) {
      std::string output;
      for (auto a: attrs)
       output += a.first+"=\""+a.second+"\" ";
      return output;
    };
  // FIN DE LA LAIDEUR
};

#endif
