#include <rafale.h>

#include "controllers/Default.hh"
Rafale::Config Rafale::config = {
  {"db.database", "name"},
  {"db.host", "tcp://127.0.0.1:3306"},
  {"db.user", "user"},
  {"db.password", "password"}
};

int main()
{
  Rafale::Controller::Register<Default>("/");
  Rafale::Server server;
  server.Run();
  return 0;

  return 0;
}
