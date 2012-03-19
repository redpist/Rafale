#ifndef _FORMS_H_
#define _FORMS_H_

#include <iostream>
#include <string>
#include <list>
#include "models/Form.hh"
#include "models/FormElement.hh"

controller Forms
{
public:
  view  Main
  {
    Forms::Form    form;
    std::string    formErrors;
  };

  action Main() 
  {
    view.form.setData("id", "caca");
    view.form.addElement(new Forms::Textfield("textfield", {
      {"title", "Mail"},
      {"description", "Ex: nom.prenom@domaine.fr"},
    }));
    view.form.addElement(new Forms::Password("password", {
      {"title", "Secret"},
      {"description", "This is super secret !"}
    }));
    view.form.addElement(new Forms::Select("wish", {
      {"title", "Plus tard je veux devenir :"},
      {"description", "On ne triche pas !"},
      {"multiple", "true"},
      {"default", "Une Poule"},
    }, {
      {"Une Poule", "1"}, 
      {"Un Canard", "2"},
      {"Une Oie", "3"}, 
      {"Un Faisan", "4"}
    }));
    view.form.addElement(new Forms::Radios("random", {
      {"title", "Choisis un nombre entre 10 et 50"},
      {"description", "Question anti-robot"},
      {"default", "B"}
    }, {
      {"B", "B"},
      {"5", "5"},
      {"Over 9000", "9000"}
    }));
    view.form.addElement(new Forms::Textarea("random", {
      {"title", "Dessine moi un mouton"},
      {"description", "ASCII art FTW !"},
      {"value", "            __  _         \n        .-:'  `; `-._     \n       (_,           )    \n     ,'o\"(            )> \n    (__,-'            )   \n       (             )    \n        `-'._.--._.-'     \n           |||  |||     "}
    }));
    view.form.addElement(new Forms::File("file", {
      {"title", "Upload moi la gueule !"},
      {"description", "Vas-y met moi tes gros boundaries"},
    }));
    view.form.addElement(new Forms::Submit("action", {
      {"value", "Envoie la purée Jeanine"}
    }));
    if (!Rafale::postDatas.empty()) {
      Forms::ErrorList list = view.form.validate();
      if (!list.empty())
        view.formErrors = Forms::Form::errorListToJSON(list);
    }
  }
};

#endif /* _FORMS_H_ */
