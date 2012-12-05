#include <rafale/controller.hh>
#include <rafale/controllers/404.hh>

Rafale::Controller::ControllersMap_ Rafale::Controller::controllers_;

std::string Rafale::Controller::contentType = "text/html";

std::function<Rafale::Controller* (void)> Rafale::Controller::Default::error404_ = &Rafale::Controller::Make_<Rafale::DefaultControllers::Error404>;

std::list<std::string> Rafale::Controller::scopes_;
