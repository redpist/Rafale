#include <rafale.h>

#include "controllers/Default.hh"
#include "controllers/Test.hh"

Rafale::Config Rafale::config = {
  {"db.database", "testrafale"},
  {"db.host", "tcp://127.0.0.1:3306"},
  {"db.user", "root"},
  {"db.password", ""}
};

int main()
{
  Rafale::Controller::Register<Default>("/");
  Rafale::Controller::Register<Test>("/test");
  // Rafale::cookiesMaxAge = 3600 * 10;

  Rafale::Server server;
  server.Run();
  return 0;
}
