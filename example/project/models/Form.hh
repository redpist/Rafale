#ifndef _FORM_HH_
# define _FORM_HH_

# include <map>
# include "FormElement.hh"

namespace Forms {
  typedef std::list<std::pair<std::string, Forms::FormElement *> > FormElementMap;
  class Form : public Forms::FormElement {
  public:
    AttrMap     datas;

    Form(const std::string& id = "");
    Form(const std::string& id, const AttrMap &data);
    ~Form();
    std::string  render();
    FormElement *addElement(FormElement *e);
    FormElement *operator[](const std::string &id);
    ErrorList    validate(const std::map<std::string, std::string> &values);

  // HHAA C'EST MOCHE, MET MOI AILLEURS, JE SOUFFRE
    static       std::string errorListToJSON(Forms::ErrorList list);
  // FIN DE LA LAIDEUR
  
  protected:
    FormElementMap _elements;
    
  private:
    Form(const Form&);
    Form operator =(const Form &);
  };
};
#endif /* _FORM_HH_ */