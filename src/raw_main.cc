
namespace Rafale
{
  std::map<std::string, std::string>       serverDatas;
  std::map<std::string, std::string>       getDatas;
  std::map<std::string, std::string>       postDatas;
}

void    GetServerData()
{
  Rafale::serverDatas.clear();
  extern char **environ;
  char** env;
  for (env = environ; *env != 0; env++)
    {
      std::string var(*env);
      Rafale::serverDatas[var.substr(0, var.find("="))] = var.substr(var.find("=") + 1, var.size());
    }
};

void    GetGetData()
{
  Rafale::getDatas.clear();
  std::string queryString = Rafale::serverDatas["QUERY_STRING"];
  while (queryString.find("&") != std::string::npos)
    {
      Rafale::getDatas[queryString.substr(0, queryString.find("="))] = Rafale::UriDecode(queryString.substr(queryString.find("=") + 1, queryString.find("&") - queryString.find("=") - 1));
      queryString = queryString.substr(queryString.find("&") + 1, queryString.size() - queryString.find("&") - 1);
    }
  if (queryString.size()) {
    Rafale::getDatas[queryString.substr(0, queryString.find("="))] = Rafale::UriDecode(queryString.substr(queryString.find("=") + 1, queryString.size()));
  }
};

#include "fcgi_stdio.h"

int main(void)
{
  while(FCGI_Accept() >= 0)
    {
      try {
        FCGI_printf("Content-type: text/html\r\n\r\n");
        Dispatcher dispatcher(getenv("SCRIPT_FILENAME"));
        Rafale::Controller    *p = Caller::Make(dispatcher.Controller());
        GetServerData();
        GetGetData();
        FCGI_printf("%s", p->Action(dispatcher.Action()).c_str());
        delete p;
      }
      catch(const char*s) {
        std::cerr << s << std::endl;
      }
    }
}
