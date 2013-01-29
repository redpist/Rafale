#include <rafale/controller.hh>
#include <rafale/controllers/404.hh>

Rafale::Controller_::ControllersMap_ Rafale::Controller_::controllers_;

std::string Rafale::Controller_::contentType = "text/html";

std::function<Rafale::Controller_* (void)> Rafale::Controller_::Default::error404_ = Rafale::Controller_::Default::Maker404<Rafale::DefaultControllers::Error404>();

std::list<std::string> Rafale::Controller_::RAIIscopes_;
