
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

void    ParseMultipart()
{
  std::size_t offset = 0;
  std::size_t nextOffset;

  /*      while (nextOffset = Rafale::serverDatas["CONTENT"].find("--" + Rafale::serverDatas["boundary"], offset))
        {
          offset += Rafale::serverDatas["boundary"].size() + 3;
          std::size_t nameOffset = Rafale::serverDatas["CONTENT"].find("name=\"", offset);
          if (
              offset = nextOffset + 1;                                                                                       //   A FINIR !!!
              }*/


  //                                                                                    TO DO !!!
}

void    GetPostData()
{
  Rafale::postDatas.clear();
  // reminder : something weird with int convertion to string
  if (Rafale::serverDatas["CONTENT_TYPE"].find("multipart/form-data; boundary=") == 0)
    {
      Rafale::serverDatas["boundary"] =
        Rafale::serverDatas["CONTENT_TYPE"].substr(
                                                   sizeof("multipart/form-data; boundary=") - 1,
                                                   Rafale::serverDatas["CONTENT_TYPE"].size()
                                                   - sizeof("multipart/form-data; boundary=") + 1);
      ParseMultipart();
      // Rafale::serverDatas["FILE"] = Rafale::serverDatas["CONTENT"];
    }
  else
    {
      SetVariables(Rafale::serverDatas["CONTENT"], Rafale::postDatas);
    }
};

int main(void)
{
  {
    std::ofstream outputFile("/var/log/rafale/preindex.log"); // LOGS
    if (outputFile.is_open()) // LOGS
      {
        outputFile << "TEST: " << std::endl;
      }
  }

  while(FCGI_Accept() >= 0)
    {
      try {
        {
            std::ofstream outputFile("/var/log/rafale/index.log"); // LOGS
            if (outputFile.is_open()) // LOGS
              {
                outputFile << "TEST: " << std::endl;
              }
        }

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
