#ifndef _FORMVALIDATORS_HH_
# define _FORMVALIDATORS_HH_

# include "FormElement.hh"

namespace Forms {
  namespace Validators {
    std::string isRequired(Forms::FormElement *element, const std::map<std::string, std::string> &values);
    std::string isNumeric(Forms::FormElement *element, const std::map<std::string, std::string> &values);
//    std::string isMail(Forms::FormElement *element, const std::map<std::string, std::string> &values);
  };
};
#endif /* _FORMVALIDATORS_HH_ */