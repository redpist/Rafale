#include <rafale.h>

#include "controllers/Default.hh"

Rafale::Config Rafale::config = {
  {"db.database", "testrafale"},
  {"db.host", "tcp://127.0.0.1:3306"},
  {"db.user", "root"},
  {"db.password", ""}
};

int main()
{
  Rafale::Controller::Register<Default>("/");
  // Rafale::cookiesMaxAge = 3600 * 10;

  Rafale::Server server;
  server.Run();
  return 0;
}
