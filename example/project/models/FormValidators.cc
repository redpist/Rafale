# include "FormValidators.hh"

namespace Forms {
  namespace Validators {

    std::string isRequired(Forms::FormElement *element, const std::map<std::string, std::string> &values) {
      std::string value;
      std::string error = "The field "+element->getData("title")+" is required";
      try { value = values.at(element->getData("name")); }
      catch (...) { }
      return (value == "" ? error : "");
    }  

    std::string isNumeric(Forms::FormElement *element, const std::map<std::string, std::string> &values) {
      std::string value;
      std::string error = "The field "+element->getData("title")+" only accept numerals";
      try { value = values.at(element->getData("name")); }
      catch (...) { }
      return value.find_first_not_of("0123456789.") == std::string::npos ? "" : error;
    }  

  };
};
