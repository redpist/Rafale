#ifndef _FORM_HH_
# define _FORM_HH_

# include <map>
# include "FormElement.hh"

namespace Form {
  class Form {
    typedef std::list<FormElement *> FormElementMap;
  public:
    std::string method;
    std::string action;
    std::string enctype;
  
    Form(const std::string& method = "POST", const std::string& action = "", const std::string& enctype = ""):
      method(method), action(action), enctype(enctype) {};
    ~Form() {};
    void addElement(FormElement *e) { this->_elements.push_back(e); };
    std::string render() {
      std::string output;
      output += "<form action=\""+this->action+"\" method=\""+this->method+"\" enctype=\""+this->enctype+"\">\n";
      for (auto e: this->_elements)
        output += "<div id=\"form-"+e->getData("id")+"\" class=\"form-item\">"+e->render()+"</div>\n";
      output += "</form>\n";
      return output;
    }
  private:
    FormElementMap _elements;
    Form(const Form&);
    Form operator =(const Form &);
  };
};
#endif /* _FORM_HH_ */