#include "Form.hh"

namespace Forms {

  Form::Form(const std::string& id): FormElement(id, {{"action", ""}, {"method", "POST"}}) {}
  Form::Form(const std::string& id, const AttrMap &data): FormElement(id, data) {}
  Form::~Form() {}

  FormElement *Form::addElement(FormElement *e) { 
    this->_elements.push_back({e->getData("id"), e}); 
    return e;
  }
  
  FormElement *Form::operator[](const std::string &id) {
    for (auto e: this->_elements)
      if (e.first == id)
         return e.second;
    return NULL;
  }

  std::string Form::render() {
    std::string output;
    output += "<form "+Tools::buidlAttributes(this->_data)+">\n";
    for (auto e: this->_elements)
      output += "<div id=\"form-"+e.first+"\" class=\"form-item\">"+e.second->render()+"</div>\n";
    output += "</form>\n";
    return output;
  }

  ErrorList  Form::validate(const std::map<std::string, std::string> &values) {
    ErrorList errors;
    ErrorList _errors;
    for (auto e: this->_elements) {
      _errors = e.second->validate(values);
      errors.merge(_errors);
    }
    return errors;
  }

  std::string Form::errorListToJSON(Forms::ErrorList list) {
    std::string error; 
    std::string output = "[";      
    for (auto e: list) {
      error = e.second;
      error = Rafale::Replace(error, "\\", "\\\\");
      error = Rafale::Replace(error, "\n", " \\n");
      error = Rafale::Replace(error, "\r", " \\r");
      error = Rafale::Replace(error, "\"", "\\\"");
      output += "{id: \""+e.first+"\", error: \""+error+"\"},";
      }
    output += "]";
    return output;
  }
};