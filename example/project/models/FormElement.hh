#ifndef _FORMELEMENT_HH_
# define _FORMELEMENT_HH_ 
# include <map>
# include "Tools.hh"

namespace Forms {
  typedef std::list<std::pair<std::string, std::string> >             ErrorList;
  typedef std::initializer_list<std::pair<std::string, std::string> > InitMap;
  typedef std::map<std::string, std::string>                          AttrMap;
  /*****************************************************
   * Abstract FormElement Class                        *
   *****************************************************/
  class FormElement {
  public:
    std::string                                                        type;
  
    FormElement(const std::string& id, const InitMap &data) {
      this->setDatas({{"id", id}, {"type", "undefined"}, {"name", "form-"+id}});
      this->setDatas(data); 
    };
    FormElement(const std::string& id, const AttrMap &data) {
      this->setDatas({{"id", id}, {"type", "undefined"}, {"name", "form-"+id}});
      this->setDatas(data); 
    };
    virtual ~FormElement() {};
    virtual void  setData(const std::string &key, const std::string &value) { this->_data[key] = value; } 
    virtual void  setDatas(const Forms::InitMap &data) { for (auto p: data) { this->_data[p.first] = p.second; } };
    virtual void  setDatas(const Forms::AttrMap &data) { for (auto p: data) { this->_data[p.first] = p.second; } };
    virtual const std::string& getData(const std::string &key) { return this->_data[key]; };
    virtual std::string render() = 0;
    virtual Forms::ErrorList validate() { return {{this->_data["id"], Rafale::postDatas[this->_data["name"]]}}; };

  protected:
    Forms::AttrMap  _data;
  };

  /*****************************************************
   * FormElements Base Classes                         *
   *****************************************************/
  struct Input: public FormElement {
    Input(const std::string &id, const InitMap &data): FormElement(id, data) {};
    Input(const std::string &id, const AttrMap &data): FormElement(id, data) {};
    virtual std::string render() { 
      Forms::AttrMap data = this->_data;
      std::string title = Tools::extract(data, "title");
      std::string desc = Tools::extract(data, "description");
      std::string output = "<input type=\""+this->type+"\" "+Tools::buidlAttributes(data)+"/>";
      return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
    };
  };
  
  struct MultipleInput: public Input {
    template <typename T>
    MultipleInput(const std::string &id, const T &data): Input(id, data) {};
    template <typename T, typename U>
    MultipleInput(const std::string &id, const T &data, const U &opts): Input(id, data) { this->setOptions(opts); };  
    virtual void  addOption(const std::string& key, const std::string &value) { this->_opts[key] = value; };
    virtual void  setOptions(const Forms::InitMap &opts) { for (auto p: opts) { this->_opts[p.first] = p.second; } };
    virtual void  setOptions(const Forms::AttrMap &opts) { for (auto p: opts) { this->_opts[p.first] = p.second; } };
    virtual void  removeOption(const std::string& key) { this->_opts.erase(key); };
  protected:
    Forms::AttrMap  _opts;
  };

  struct LabeledInput: public FormElement {
    LabeledInput(const std::string &id, const Forms::InitMap &data): FormElement(id, data) {};
    LabeledInput(const std::string &id, const Forms::AttrMap &data): FormElement(id, data) {};
    virtual std::string render() { 
      std::string output = "<input type=\""+this->type+"\" "+Tools::buidlAttributes(_data)+"/>";
      return output+"\n<label for=\""+this->_data["id"]+"\">"+this->_data["title"]+"</label>"; 
    };
  };
  
  
  /*****************************************************
   * FormElements Typed Classes                        *
   *****************************************************/
  struct Textfield: public Input {
    Textfield(const std::string &id, const Forms::InitMap &data): Input(id, data) { this->type = "textfield"; };
    Textfield(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "textfield"; };
  };

  struct Hidden: public Input {
    Hidden(const std::string &id, const Forms::InitMap &data): Input(id, data) { this->type = "hidden"; };
    Hidden(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "hidden"; };
  };

  struct Password: public Input {
    Password(const std::string &id, const Forms::InitMap &data): Input(id, data) { this->type = "password"; };
    Password(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "password"; };
  };

  struct Submit: public Input {
    Submit(const std::string &id, const Forms::InitMap &data): Input(id, data) { this->type = "submit"; };
    Submit(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "submit"; };
  };
  
  struct Button: public Input {
    Button(const std::string &id, const Forms::InitMap &data): Forms::Input(id, data) { this->type = "button"; };
    Button(const std::string &id, const Forms::AttrMap &data): Forms::Input(id, data) { this->type = "button"; };
  };
  
  struct Radio: public LabeledInput {
    Radio(const std::string &id, const Forms::InitMap &data): LabeledInput(id, data) { this->type = "radio"; };
    Radio(const std::string &id, const Forms::AttrMap &data): LabeledInput(id, data) { this->type = "radio"; };
  };

  struct Checkbox: public LabeledInput {
    Checkbox(const std::string &id, const Forms::InitMap &data): LabeledInput(id, data) { this->type = "checkbox"; };
    Checkbox(const std::string &id, const Forms::AttrMap &data): LabeledInput(id, data) { this->type = "checkbox"; };
  };

  struct File: public Input {
    File(const std::string &id, const Forms::InitMap &data): Input(id, data) { this->type = "file"; };
    File(const std::string &id, const Forms::AttrMap &data): Input(id, data) { this->type = "file"; };
  };

  /*****************************************************
   * FormElements Special Classes                      *
   *****************************************************/
  
  struct Textarea: public FormElement {
    Textarea(const std::string &id, const Forms::InitMap &data): FormElement(id, data) {};
    virtual std::string render() {
      Forms::AttrMap data = this->_data;
      std::string value = Tools::extract(data, "value");
      std::string title = Tools::extract(data, "title");
      std::string desc = Tools::extract(data, "description");
      std::string output = "<textarea "+Tools::buidlAttributes(data)+">"+value+"</textarea>";
      return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>";
    };
  };

  struct Select: public MultipleInput {
    Select(const std::string &id, const Forms::InitMap &data): MultipleInput(id, data) {};
    Select(const std::string &id, const Forms::InitMap &data, const Forms::InitMap &opts): MultipleInput(id, data, opts) {};
    virtual std::string render() {
      std::string empty = "";
      std::string flag = "selected";
      Forms::AttrMap data = this->_data;
      std::string def = Tools::extract(data, "default");
      std::string title = Tools::extract(data, "title");
      std::string desc = Tools::extract(data, "description");
      
      std::string output = "<select "+Tools::buidlAttributes(data)+">\n";
      for (auto o: this->_opts)
        output += "<option "+(def==o.first?flag:empty)+" value=\""+o.second+"\">"+o.first+"</option>";
      output += "</select>";
      return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
    };   
  };    

  /*****************************************************
   * FormElements Composed Classes                     *
   *****************************************************/
  template <class T>
  struct ComposedInput: public MultipleInput {
    ComposedInput(const std::string &id, const Forms::InitMap &data): MultipleInput(id, data) {};
    ComposedInput(const std::string &id, const Forms::InitMap &data, Forms::InitMap opts): MultipleInput(id, data, opts) {};
    virtual std::string render() {
      std::string empty = "";
      Forms::AttrMap data = this->_data;
      std::string def = Tools::extract(data, "default");
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
          {"name", data["id"]+"[]"},
          {"value", o.second},
          {o.first==def?"checked":"unchecked","true"}
        });
        output += "<div id=\""+data["id"]+"\" class=\"form-subitem form-"+element.type+"\">"+element.render()+"</div>";
      }
      return "<label for=\""+data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
    };
  };
  typedef ComposedInput<Forms::Checkbox> Checkboxes;  
  typedef ComposedInput<Forms::Radio> Radios;    
};
#endif /* _FORMELEMENT_HH_ */