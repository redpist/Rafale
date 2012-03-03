
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

void     SetVariables(const std::string &rawData, std::map<std::string, std::string> &container)
{
  std::string   queryString = rawData;
  while (queryString.find("&") != std::string::npos)
    {
      container[queryString.substr(0, queryString.find("="))] = Rafale::UriDecode(queryString.substr(queryString.find("=") + 1, queryString.find("&") - queryString.find("=") - 1));
      queryString = queryString.substr(queryString.find("&") + 1, queryString.size() - queryString.find("&") - 1);
    }
  if (queryString.size()) {
    container[queryString.substr(0, queryString.find("="))] = Rafale::UriDecode(queryString.substr(queryString.find("=") + 1, queryString.size()));
  }
}

void    GetGetData()
{
  Rafale::getDatas.clear();
  SetVariables(Rafale::serverDatas["QUERY_STRING"], Rafale::getDatas);
};

#define NO_FCGI_DEFINES
#include "fcgi_stdio.h"

void    SetContent()
{
  int   length = Rafale::ToInt(Rafale::serverDatas["CONTENT_LENGTH"]);
  if (length)
    {
      char  *rawBuffer = new char [length];
      FCGI_fread(rawBuffer, length, 1, FCGI_stdin);
      Rafale::serverDatas["CONTENT"].append(rawBuffer, length);
    }
  else
    Rafale::serverDatas["CONTENT"];
}

void    GetPostData()
{
  Rafale::postDatas.clear();
  SetVariables(Rafale::serverDatas["CONTENT"], Rafale::postDatas);
};

int main(void)
{
  while(FCGI_Accept() >= 0)
    {
      try {
        FCGI_printf("Content-type: text/html\r\n\r\n");
        Dispatcher dispatcher(getenv("SCRIPT_FILENAME"));
        Rafale::Controller    *p = Caller::Make(dispatcher.Controller());
        GetServerData();
        SetContent();
        GetGetData();
        GetPostData();
        FCGI_printf("%s", p->Action(dispatcher.Action()).c_str());
        delete p;
      }
      catch(const char*s) {
        std::cerr << s << std::endl;
      }
    }
}
