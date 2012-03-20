#ifndef _FORMELEMENT_HH_
# define _FORMELEMENT_HH_ 
# include <map>
# include "Tools.hh"

namespace Forms {
  class FormElement;
  typedef std::list<std::pair<std::string, std::string> >             ErrorList;
  typedef std::map<std::string, std::string>                          AttrMap;
  typedef std::string (*FormValidator)(FormElement *, const std::map<std::string, std::string> &);
  

  /*****************************************************
   * Abstract FormElement Class                        *
   *****************************************************/
  class FormElement {
  public:
    std::string                                                        type;
  
    FormElement(const std::string& id, const AttrMap &data);
    virtual ~FormElement();
    virtual void               setData(const std::string &key, const std::string &value);
    virtual void               setDatas(const Forms::AttrMap &data);
    virtual const std::string& getData(const std::string &key);
    virtual void               addValidator(FormValidator v);
    virtual void               addValidators(std::list<FormValidator> v);
    virtual std::string        render() = 0;
    virtual Forms::ErrorList   validate(const std::map<std::string, std::string> &values);

  protected:
    Forms::AttrMap            _data;
    std::list<FormValidator>  _validators;
  };

  /*****************************************************
   * FormElements Base Classes                         *
   *****************************************************/
  struct Input: public FormElement {
    Input(const std::string &id, const AttrMap &data);
    virtual std::string render();
  };
  
  struct MultipleInput: public Input {
    MultipleInput(const std::string &id, const AttrMap &data);
    MultipleInput(const std::string &id, const AttrMap &data, const AttrMap &opts);
    virtual void      addOption(const std::string& key, const std::string &value);
    virtual void      setOptions(const Forms::AttrMap &opts);
    virtual void      removeOption(const std::string& key);
    virtual ErrorList validate(const std::map<std::string, std::string> &values);
  protected:
    Forms::AttrMap  _opts;
  };

  struct LabeledInput: public FormElement {
    LabeledInput(const std::string &id, const Forms::AttrMap &data);
    virtual std::string render();
  };
  
  
  /*****************************************************
   * FormElements Typed Classes                        *
   *****************************************************/
  struct Textfield: public Input {
    Textfield(const std::string &id, const Forms::AttrMap &data);
  };

  struct Hidden: public Input {
    Hidden(const std::string &id, const Forms::AttrMap &data);
  };

  struct Password: public Input {
    Password(const std::string &id, const Forms::AttrMap &data);
  };

  struct Submit: public Input {
    Submit(const std::string &id, const Forms::AttrMap &data);
  };
  
  struct Button: public Input {
    Button(const std::string &id, const Forms::AttrMap &data);
  };
  
  struct Radio: public LabeledInput {
    Radio(const std::string &id, const Forms::AttrMap &data);
  };

  struct Checkbox: public LabeledInput {
    Checkbox(const std::string &id, const Forms::AttrMap &data);
  };

  struct File: public Input {
    File(const std::string &id, const Forms::AttrMap &data);
  };

  /*****************************************************
   * FormElements Special Classes                      *
   *****************************************************/
  
  struct Textarea: public FormElement {
    Textarea(const std::string &id, const Forms::AttrMap &data);
    virtual std::string render();
  };

  struct Select: public MultipleInput {
    Select(const std::string &id, const Forms::AttrMap &data);
    Select(const std::string &id, const Forms::AttrMap &data, const Forms::AttrMap &opts);
    virtual std::string render();
  };    

  /*****************************************************
   * FormElements Composed Classes                     *
   *****************************************************/
  template <class T>
  struct ComposedInput: public MultipleInput {
    ComposedInput(const std::string &id, const Forms::AttrMap &data);
    ComposedInput(const std::string &id, const Forms::AttrMap &data, const Forms::AttrMap &opts);
    virtual std::string render();
  };
  typedef ComposedInput<Forms::Checkbox> Checkboxes;  
  typedef ComposedInput<Forms::Radio> Radios;    
};
#endif /* _FORMELEMENT_HH_ */