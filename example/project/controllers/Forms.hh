#ifndef _FORMS_H_
#define _FORMS_H_

#include <iostream>
#include <string>
#include <list>
#include "models/Form.hh"
#include "models/FormElement.hh"
#include "models/FormValidators.hh"

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
    view.form.addElement(new Forms::Textfield("mail", {
      {"title", "Mail"},
      {"description", "Ex: nom.prenom@domaine.fr"},
      {"value", Rafale::postDatas["mail"]}
    }))->addValidator(Forms::Validators::isNumeric);

    view.form.addElement(new Forms::Password("password", {
      {"title", "Secret"},
      {"description", "This is super secret !"},
      {"value", Rafale::postDatas["password"]}
    }))->addValidator(Forms::Validators::isRequired);

    view.form.addElement(new Forms::Select("wish", {
      {"title", "Plus tard je veux devenir :"},
      {"description", "On ne triche pas !"},
      {"multiple", "true"},
      {"value", "Une Poule"},
    }, {
      {"Une Poule", "1"}, 
      {"Un Canard", "2"},
      {"Une Oie", "3"}, 
      {"Un Faisan", "4"}
    }));
    view.form.addElement(new Forms::Radios("random", {
      {"title", "Choisis un nombre entre 10 et 50"},
      {"description", "Question anti-robot"},
      {"value", "B"}
    }, {
      {"B", "B"},
      {"5", "5"},
      {"Over 9000", "9000"}
    }));
    view.form.addElement(new Forms::Textarea("random", {
      {"title", "Dessine moi un mouton"},
      {"description", "ASCII art FTW !"},
      {"value", Rafale::postDatas["random"] != "" ? Rafale::postDatas["random"] : "            __  _         \n        .-:'  `; `-._     \n       (_,           )    \n     ,'o\"(            )> \n    (__,-'            )   \n       (             )    \n        `-'._.--._.-'     \n           |||  |||     "}
    }));
    view.form.addElement(new Forms::Submit("action", {
      {"value", "Envoie la purée Jeanine"}
    }));
    if (Rafale::postDatas["action"] != "") {
      Forms::ErrorList list = view.form.validate(Rafale::postDatas);
      if (!list.empty())
        view.formErrors = Forms::Form::errorListToJSON(list);
    }
  }
};

#endif /* _FORMS_H_ */
