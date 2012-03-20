#ifndef _FORMVALIDATORS_HH_
# define _FORMVALIDATORS_HH_

# include "FormElement.hh"

namespace Forms {
  class FormElement;
    struct Validators {

    static std::string Required(Forms::FormElement *element, const std::map<std::string, std::string> &values) {
      std::string value;
      try { value = values.at(element->getData("name")); }
      catch (...) { (void)value; }
      return (value == "" ? "The field "+element->getData("Title")+" is required" : "");
    };
    
  };
};
#endif /* _FORMVALIDATORS_HH_ */