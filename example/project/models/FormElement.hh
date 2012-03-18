#ifndef _FORMELEMENT_HH_
# define _FORMELEMENT_HH_ 
# include <map>

namespace Form {
  /*****************************************************
   * Abstract FormElement Class                        *
   *****************************************************/
  struct FormElement {
    typedef std::initializer_list<std::pair<std::string, std::string>> InitMap;
    typedef std::map<std::string, std::string>                         AttrMap;
    std::string                                                        type;
  
    template <typename T>
    FormElement(const std::string& id, const T &data) {
      this->type = "undefined";
      this->setDatas(data);
      this->setData("id", id);
      if (this->_data["name"] == "")
        this->setData("name", "form-"+id);
    };
    virtual ~FormElement() {};
    virtual void  setData(const std::string &key, const std::string &value) { this->_data[key] = value; } 
    virtual void  setDatas(const FormElement::InitMap &data) { for (auto p: data) { this->_data[p.first] = p.second; } };
    virtual void  setDatas(const FormElement::AttrMap &data) { for (auto p: data) { this->_data[p.first] = p.second; } };
    virtual const std::string& getData(const std::string &key) { return this->_data[key]; };
    virtual std::string render() = 0;

  // HHAA C'EST MOCHE, MET MOI AILLEURS, JE SOUFFRE
    static std::string extract(FormElement::AttrMap &data, const std::string &key) {
      std::string val = data[key];
      data.erase(key);
      return val;
    }    
    static std::string buidlAttributes(const FormElement::AttrMap &attrs) {
      std::string output;
      for (auto a: attrs)
       output += a.first+"=\""+a.second+"\" ";
      return output;
    };
  // FIN DE LA LAIDEUR

  protected:
    FormElement::AttrMap  _data;
  private:
    FormElement();  
    
  };

  /*****************************************************
   * FormElements Base Classes                         *
   *****************************************************/
  struct Input: public FormElement {
    Input(const std::string &id, const InitMap &data): FormElement(id, data) {};
    Input(const std::string &id, const AttrMap &data): FormElement(id, data) {};
    virtual std::string render() { 
      FormElement::AttrMap data = this->_data;
      std::string title = FormElement::extract(data, "title");
      std::string desc = FormElement::extract(data, "description");
      std::string output = "<input type=\""+this->type+"\" "+FormElement::buidlAttributes(data)+"/>";
      return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
    };
  };
  
  struct MultipleInput: public Input {
    template <typename T>
    MultipleInput(const std::string &id, const T &data): Input(id, data) {};
    template <typename T, typename U>
    MultipleInput(const std::string &id, const T &data, const U &opts): Input(id, data) { this->setOptions(opts); };  
    virtual void  addOption(const std::string& key, const std::string &value) { this->_opts[key] = value; };
    virtual void  setOptions(const FormElement::InitMap &opts) { for (auto p: opts) { this->_opts[p.first] = p.second; } };
    virtual void  setOptions(const FormElement::AttrMap &opts) { for (auto p: opts) { this->_opts[p.first] = p.second; } };
    virtual void  removeOption(const std::string& key) { this->_opts.erase(key); };
  protected:
    FormElement::AttrMap  _opts;
  };

  struct LabeledInput: public FormElement {
    LabeledInput(const std::string &id, const FormElement::InitMap &data): FormElement(id, data) {};
    LabeledInput(const std::string &id, const FormElement::AttrMap &data): FormElement(id, data) {};
    virtual std::string render() { 
      std::string output = "<input type=\""+this->type+"\" "+FormElement::buidlAttributes(_data)+"/>";
      return output+"\n<label for=\""+this->_data["id"]+"\">"+this->_data["title"]+"</label>"; 
    };
  };
  
  
  /*****************************************************
   * FormElements Typed Classes                        *
   *****************************************************/
  struct Textfield: public Input {
    Textfield(const std::string &id, const FormElement::InitMap &data): Input(id, data) { this->type = "textfield"; };
    Textfield(const std::string &id, const FormElement::AttrMap &data): Input(id, data) { this->type = "textfield"; };
  };

  struct Hidden: public Input {
    Hidden(const std::string &id, const FormElement::InitMap &data): Input(id, data) { this->type = "hidden"; };
    Hidden(const std::string &id, const FormElement::AttrMap &data): Input(id, data) { this->type = "hidden"; };
  };

  struct Password: public Input {
    Password(const std::string &id, const FormElement::InitMap &data): Input(id, data) { this->type = "password"; };
    Password(const std::string &id, const FormElement::AttrMap &data): Input(id, data) { this->type = "password"; };
  };

  struct Submit: public Input {
    Submit(const std::string &id, const FormElement::InitMap &data): Input(id, data) { this->type = "submit"; };
    Submit(const std::string &id, const FormElement::AttrMap &data): Input(id, data) { this->type = "submit"; };
  };
  
  struct Button: public Input {
    Button(const std::string &id, const FormElement::InitMap &data): Form::Input(id, data) { this->type = "button"; };
    Button(const std::string &id, const FormElement::AttrMap &data): Form::Input(id, data) { this->type = "button"; };
  };
  
  struct Radio: public LabeledInput {
    Radio(const std::string &id, const FormElement::InitMap &data): LabeledInput(id, data) { this->type = "radio"; };
    Radio(const std::string &id, const FormElement::AttrMap &data): LabeledInput(id, data) { this->type = "radio"; };
  };

  struct Checkbox: public LabeledInput {
    Checkbox(const std::string &id, const FormElement::InitMap &data): LabeledInput(id, data) { this->type = "checkbox"; };
    Checkbox(const std::string &id, const FormElement::AttrMap &data): LabeledInput(id, data) { this->type = "checkbox"; };
  };

  struct File: public Input {
    File(const std::string &id, const FormElement::InitMap &data): Input(id, data) { this->type = "file"; };
    File(const std::string &id, const FormElement::AttrMap &data): Input(id, data) { this->type = "file"; };
  };

  /*****************************************************
   * FormElements Special Classes                      *
   *****************************************************/
  
  struct Textarea: public FormElement {
    Textarea(const std::string &id, const FormElement::InitMap &data): FormElement(id, data) {};
    virtual std::string render() {
      FormElement::AttrMap data = this->_data;
      std::string value = FormElement::extract(data, "value");
      std::string title = FormElement::extract(data, "title");
      std::string desc = FormElement::extract(data, "description");
      std::string output = "<textarea "+FormElement::buidlAttributes(data)+">"+value+"</textarea>";
      return "<label for=\""+this->_data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>";
    };
  };

  struct Select: public MultipleInput {
    Select(const std::string &id, const FormElement::InitMap &data): MultipleInput(id, data) {};
    Select(const std::string &id, const FormElement::InitMap &data, const FormElement::InitMap &opts): MultipleInput(id, data, opts) {};
    virtual std::string render() {
      std::string empty = "";
      std::string flag = "selected";
      FormElement::AttrMap data = this->_data;
      std::string def = FormElement::extract(data, "default");
      std::string title = FormElement::extract(data, "title");
      std::string desc = FormElement::extract(data, "description");
      
      std::string output = "<select "+FormElement::buidlAttributes(data)+">\n";
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
    ComposedInput(const std::string &id, const FormElement::InitMap &data): MultipleInput(id, data) {};
    ComposedInput(const std::string &id, const FormElement::InitMap &data, FormElement::InitMap opts): MultipleInput(id, data, opts) {};
    virtual std::string render() {
      std::string empty = "";
      FormElement::AttrMap data = this->_data;
      std::string def = FormElement::extract(data, "default");
      std::string title = FormElement::extract(data, "title");
      std::string desc = FormElement::extract(data, "description");
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
        output += "<div class=\"form-subitem form-"+element.type+"\">"+element.render()+"</div>";
      }
      return "<label for=\""+data["id"]+"\">"+title+"</label>"+output+"<span>"+desc+"</span>"; 
    };
  };
  typedef ComposedInput<Form::Checkbox> Checkboxes;  
  typedef ComposedInput<Form::Radio> Radios;    
};
#endif /* _FORMELEMENT_HH_ */