#include "FormElement.hh"

namespace Forms {

/*****************************************************
 * Abstract FormElement Class                        *
 *****************************************************/
  FormElement::FormElement(const std::string& id, const AttrMap &data) {
    this->setDatas({{"id", id}, {"type", "undefined"}, {"name", id}});
    this->setDatas(data); 
  }
  FormElement::~FormElement() {}

  const std::string& FormElement::getData(const std::string &key) { return this->_data[key]; }  
  void               FormElement::setData(const std::string &key, const std::string &value) { this->_data[key] = value; }
  void               FormElement::setDatas(const Forms::AttrMap &data) { for (auto p: data) { this->setData(p.first, p.second); } }
  void               FormElement::addValidator(FormValidator v) { this->_validators.push_back(v); }
  void               FormElement::addValidators(std::list<FormValidator> v) { for (auto p: v) { this->addValidator(p); } }
  Forms::ErrorList   FormElement::validate(const std::map<std::string, std::string> &values) { 
    std::string msg;
    Forms::ErrorList e;
    for (auto v: this->_validators) {
      if ((msg = v(this, values)) != "") 
        e.push_back({this->_data["id"], msg});
    }
    return e;
  }

/*****************************************************
 * Input FormElements                                *
 *****************************************************/
  Input::Input(const std::string &id, const AttrMap &data): FormElement(id, data) {}
  std::string Input::render() { 
    Forms::AttrMap data = this->_data;
    std::string title = Tools::extract(data, "title");
    std::string desc = Tools::extract(data, "description");
    std::string output = "<input type=\""+this->type+"\" "+Tools::buidlAttributes(data)+"/>";
    return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
  }

/*****************************************************
 * MultipleInput FormElements                        *
 *****************************************************/
  MultipleInput::MultipleInput(const std::string &id, const AttrMap &data): Input(id, data) {}
  MultipleInput::MultipleInput(const std::string &id, const AttrMap &data, const AttrMap &opts): Input(id, data) { this->setOptions(opts); }
  void  MultipleInput::addOption(const std::string& key, const std::string &value) { this->_opts[key] = value; }
  void  MultipleInput::setOptions(const Forms::AttrMap &opts) { for (auto p: opts) { this->_opts[p.first] = p.second; } }
  void  MultipleInput::removeOption(const std::string& key) { this->_opts.erase(key); }

/*****************************************************
 * LabeledInput FormElements                         *
 *****************************************************/
  LabeledInput::LabeledInput(const std::string &id, const Forms::AttrMap &data): FormElement(id, data) {};
  std::string LabeledInput::render() {
    std::string output = "<input type=\""+this->type+"\" "+Tools::buidlAttributes(_data)+"/>";
    return output+"\n<label for=\""+this->_data["id"]+"\">"+this->_data["title"]+"</label>"; 
  }

/*****************************************************
 * Textfield FormElements                            *
 *****************************************************/
  Textfield::Textfield(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "textfield"; }

/*****************************************************
 * Hidden FormElements                               *
 *****************************************************/
  Hidden::Hidden(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "hidden"; }

/*****************************************************
 * Password FormElements                             *
 *****************************************************/
  Password::Password(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "password"; }

/*****************************************************
 * Submit FormElements                               *
 *****************************************************/
  Submit::Submit(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "submit"; };

/*****************************************************
 * Button FormElements                               *
 *****************************************************/
  Button::Button(const std::string &id, const Forms::AttrMap &data): Forms::Input(id, data) { this->type = "button"; }

/*****************************************************
 * Radio FormElements                                *
 *****************************************************/
  Radio::Radio(const std::string &id, const Forms::AttrMap &data): LabeledInput(id, data) { this->type = "radio"; }

/*****************************************************
 * Checkbox FormElements                             *
 *****************************************************/
  Checkbox::Checkbox(const std::string &id, const Forms::AttrMap &data): LabeledInput(id, data) { this->type = "checkbox"; }

/*****************************************************
 * File FormElements                                 *
 *****************************************************/
  File::File(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "file"; }

/*****************************************************
 * Textarea FormElements                             *
 *****************************************************/
  Textarea::Textarea(const std::string &id, const Forms::AttrMap &data): FormElement(id, data) {};
  std::string Textarea::render() {
    Forms::AttrMap data = this->_data;
    std::string value = Tools::extract(data, "value");
    std::string title = Tools::extract(data, "title");
    std::string desc = Tools::extract(data, "description");
    std::string output = "<textarea "+Tools::buidlAttributes(data)+">"+value+"</textarea>";
    return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>";
  }

/*****************************************************
 * Select FormElements                               *
 *****************************************************/
  Select::Select(const std::string &id, const Forms::AttrMap &data): MultipleInput(id, data) {};
  Select::Select(const std::string &id, const Forms::AttrMap &data, const Forms::AttrMap &opts): MultipleInput(id, data, opts) {};
  std::string Select::render() {
    std::string empty = "";
    std::string flag = "selected";
    Forms::AttrMap data = this->_data;
    std::string def = Tools::extract(data, "value");
    std::string title = Tools::extract(data, "title");
    std::string desc = Tools::extract(data, "description");
    
    std::string output = "<select "+Tools::buidlAttributes(data)+">\n";
    for (auto o: this->_opts)
      output += "<option "+(def==o.first?flag:empty)+" value=\""+o.second+"\">"+o.first+"</option>";
    output += "</select>";
    return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
  }

/*****************************************************
 * FormElements Composed Classes                     *
 *****************************************************/
  template <class T>
  ComposedInput<T>::ComposedInput(const std::string &id, const Forms::AttrMap &data): MultipleInput(id, data) {}
  template <class T>
  ComposedInput<T>::ComposedInput(const std::string &id, const Forms::AttrMap &data, const Forms::AttrMap &opts): MultipleInput(id, data, opts) {}
  template <class T>
  std::string ComposedInput<T>::render() {
    Forms::AttrMap data = this->_data;
    std::string def = Tools::extract(data, "value");
    std::string title = Tools::extract(data, "title");
    std::string desc = Tools::extract(data, "description");
    std::string output = "";
    std::stringstream tmp;

    int i = 0;
    for (auto o: this->_opts) {
      tmp.str("");
      tmp << data["id"] << '-' << i++;
      T element(tmp.str(), data);
      element.setDatas({
        {"title", o.first},
        {"name", data["id"]},
        {"value", o.second},
        {o.first==def?"checked":"unchecked","true"}
      });
      output += "<div id=\""+data["id"]+"\" class=\"form-subitem form-"+element.type+"\">"+element.render()+"</div>";
    }
    return "<label for=\""+data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
  }
template class ComposedInput<Forms::Checkbox>;
template class ComposedInput<Forms::Radio>;
};
