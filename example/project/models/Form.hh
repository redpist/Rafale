#ifndef _FORM_HH_
# define _FORM_HH_

# include <map>
# include "FormElement.hh"

namespace Forms {
  typedef std::list<std::pair<std::string, Forms::FormElement *> > FormElementMap;
  class Form : public Forms::FormElement {
  public:
    AttrMap     datas;

    Form(const std::string& id = ""): FormElement(id, {{"action", ""}, {"method", "POST"}}) {};
    Form(const std::string& id, const InitMap &data): FormElement(id, data) {};
    Form(const std::string& id, const AttrMap &data): FormElement(id, data) {};
    ~Form() {};
    void addElement(FormElement *e) { this->_elements.push_back({e->getData("id"), e}); };

    FormElement *operator[](const std::string &id) {
      for (auto e: this->_elements)
        if (e.first == id) { return e.second; }
      return NULL;
    }

    std::string render() {
      std::string output;
      output += "<form "+Tools::buidlAttributes(this->_data)>+"\n";
      for (auto e: this->_elements)
        output += "<div id=\"form-"+e.first+"\" class=\"form-item\">"+e.second->render()+"</div>\n";
      output += "</form>\n";
      return output;
    }

    ErrorList validate() {
      ErrorList errors;
      ErrorList _errors;
      for (auto e: this->_elements) {
        _errors = e.second->validate();
        errors.merge(_errors);
      }
      return errors;
    }

  // HHAA C'EST MOCHE, MET MOI AILLEURS, JE SOUFFRE
    static std::string errorListToJSON(Forms::ErrorList list) {
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
  // FIN DE LA LAIDEUR
  
  protected:
    FormElementMap _elements;
    
  private:
    Form(const Form&);
    Form operator =(const Form &);
  };
};
#endif /* _FORM_HH_ */