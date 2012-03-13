#ifndef _FORM_HH_
#define _FORM_HH_

#include <map>
/*
class FormElement {
public:
  std::string id;
  std::string name;
  std::string type;
  std::string title;
  
  FormElement(const std::string &id, const std::string &name, const std::string &title);
  virtual ~FormElement() = 0;
  virtual print() = 0;
}
*/
class Form
{
public:
  typedef std::map<std::string, std::string> FormElement;
  typedef std::string (Form::*FormViewCallback)(const std::string &, FormElement &) const;
  std::string action;
  std::string method;
  std::string enctype;

  Form(const std::string &action = "", const std::string &method = "POST", const std::string &enctype="") 
    :action(action), method(method), enctype(enctype) {
    this->views = {
      {"radio", &Form::_checkableView},
      {"checkbox", &Form::_checkableView},
      {"input", &Form::_inputableView},
      {"password", &Form::_inputableView},
      {"button", &Form::_inputableView},
      {"submit", &Form::_inputableView},
      {"textarea", &Form::_textareaView},
    };
  };
  ~Form() { }

protected:
  std::map<std::string, FormElement>      elements;
  std::map<std::string, FormViewCallback> views;

public:
  void              addElement(const std::string &id, FormElement e) {
    if (e.find("type") != e.end() && this->views.find(e["type"]) != this->views.end()) {
      this->elements[id] = e;
      this->elements[id]["id"] = id;
    }      
  }

  void              removeElement(const std::string &id) {
    this->elements.erase(id);
  }

  std::string       print() {
    std::string result;
    
    result = "<form action=\""+this->action+"\" method=\""+this->method+"\" enctype=\""+this->enctype+"\">";
    for (auto p: this->elements)
      result += this->print(p.first);
    result += "</form>";
    return result;
  }

  std::string      print(const std::string &id) {
    FormElement e = this->elements[id];
    return (this->*this->views[e["type"]])(e["id"], e);
  }
  
protected:
  std::string _checkableView(const std::string &id, FormElement &e) const {
    return "<input type=\""+e["type"]+"\" id=\""+id+"\" name=\""+id+"\" value=\""+e["value"]+"\">"
          +"<label for=\""+id+"\">"+e["title"]+"</label>";
  }

  std::string _inputableView(const std::string &id, FormElement &e) const {
    return "<label for=\""+id+"\">"+e["title"]+"</label>"
          +"<input type=\""+e["type"]+"\" id=\""+id+"\" name=\""+id+"\" value=\""+e["value"]+"\">";
  }

  std::string _textareaView(const std::string &id, FormElement &e) const {
    return "<label for=\""+id+"\">"+e["title"]+"</label>"
          +"<textarea id=\""+id+"\" name=\""+id+"\" value=\""+e["value"]+"\">";
  }

};

#endif /* _FORM_HH_ */