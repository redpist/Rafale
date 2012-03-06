#include <sys/types.h>
#include <dirent.h>

namespace Rafale
{
  std::map<std::string, std::string>       serverDatas;
  std::map<std::string, std::string>       getDatas;
  std::map<std::string, std::string>       postDatas;
  std::map<std::string, Rafale::File>      files;
  std::string                              tmpDirectory;
  std::string                              filesDirectory;
  std::string                              sessionsDirectory;
  std::map<std::string, Session*>              Session::sessionMap_;
  std::map<time_t, Session>                    Session::sessions_;

  // std::map<std::string, std::string>       headers;
  std::map<std::string, Rafale::Cookie>    cookies;
}

void    GetServerData()
{
  extern char **environ;
  char** env;
  for (env = environ; *env != 0; env++)
    {
      std::string var(*env);
      Rafale::serverDatas[var.substr(0, var.find("="))] = var.substr(var.find("=") + 1, var.size() - var.find("="));
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
  SetVariables(Rafale::serverDatas["QUERY_STRING"], Rafale::getDatas);
};


#define NO_FCGI_DEFINES
#include "fcgi_stdio.h"

void    SetContent()
{
  if (Rafale::serverDatas["CONTENT_LENGTH"].size())
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
}

std::string     ParseValue(const std::string &name, const std::string &str, std::size_t offset = 0, char assign = '=', char startDelim = '"', char endDelim = '"')
{
  std::size_t startValueOffset = str.find(name + assign + startDelim, offset) + name.size() + sizeof(assign) + sizeof(startDelim);
  std::size_t endValueOffset = str.find(endDelim, startValueOffset); // TO DO : fix \delim
  return str.substr(startValueOffset, endValueOffset - startValueOffset);
}

inline bool            IsFileUpload(const std::string &part)
{
  return (part.find("filename") < part.find("\r\n\r\n"));
}

inline std::string     GetContent(const std::string &part)
{
  std::size_t start = part.find("\r\n\r\n") + (sizeof("\r\n\r\n") - 1);
  return part.substr(start, part.size() - start);
}

void                   FileUpload(std::string &name, const std::string &content)
{
  Rafale::files[name] = Rafale::File(ParseValue("filename", content), ParseValue("Content-Type", content, 0, ':', ' ', '\r'),
                                     GetContent(content));
}

std::size_t    ParseOnepart(std::size_t offset, std::size_t nextOffset)
{
  std::string currentData = Rafale::serverDatas["CONTENT"].substr(offset, nextOffset - offset);
  std::string name = ParseValue("name", currentData, currentData.find("name"));
  if (IsFileUpload(currentData))
    FileUpload(name, currentData);
  else
    Rafale::postDatas[name] = GetContent(currentData);
  return nextOffset + (sizeof("\r\n--") - 1) + (Rafale::serverDatas["boundary"].size() - 1) + (sizeof("\r\n") - 1);
}


void    ParseMultipart() // FULL OF FAIL GO PATCH QUICKLY BRO
{
  std::size_t offset = (sizeof("--") - 1)+ Rafale::serverDatas["boundary"].size() + (sizeof("\r\n") - 1);
  std::size_t nextOffset = 0;

  // {
  //   std::string newContent;
  //   for (char c: Rafale::serverDatas["CONTENT"])
  //     {
  //       if (c < 32)
  //         {
  //           newContent += '\\';
  //           if (c == 13)
  //             newContent += 'r';
  //           else if (c == 10)
  //             newContent += 'n';
  //           else
  //             newContent += Rafale::ToString(static_cast<int>(c));
  //         }
  //       else
  //         newContent += c;
  //     }
  //   Rafale::serverDatas["CONTENT_ESCAPED"] = newContent;
  // }

  int i = 0;
  while ((nextOffset = Rafale::serverDatas["CONTENT"].find("\r\n--" + Rafale::serverDatas["boundary"] + "\r\n", offset)) != std::string::npos)
    {
      ++i;
      offset = ParseOnepart(offset, nextOffset);
    }
  if ((nextOffset = Rafale::serverDatas["CONTENT"].find("\r\n--" + Rafale::serverDatas["boundary"] + "--\r\n", offset)) != std::string::npos)
    {
      ++i;
      offset = ParseOnepart(offset, nextOffset);
    }
  Rafale::serverDatas["MULTIPART_COUNTER"] = Rafale::ToString(i);
  //         -----------------------------  TO DO !!!
}

void    GetPostData()
{
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

void    GetCookies()
{
  if (Rafale::serverDatas["HTTP_COOKIE"].size())
    {
      std::size_t prevOffset = 0;
      for (std::size_t offset = 2; (offset = Rafale::serverDatas["HTTP_COOKIE"].find("; ", offset)); prevOffset = offset)
        {
          
          if (offset == std::string::npos)
            offset = Rafale::serverDatas["HTTP_COOKIE"].size();
          std::size_t equal = Rafale::serverDatas["HTTP_COOKIE"].find("=", prevOffset);
          Rafale::cookies[Rafale::UriDecode(Rafale::serverDatas["HTTP_COOKIE"].substr(prevOffset, equal - prevOffset))]
            = Rafale::UriDecode(Rafale::serverDatas["HTTP_COOKIE"].substr(equal + 1, offset - equal - 1));
          if (offset == Rafale::serverDatas["HTTP_COOKIE"].size())
            break;
          offset += 2;
        }
    }
}

std::string    SetCookies()
{
  std::string result;
  for (auto cookie: Rafale::cookies)
    {
      if (cookie.second.modified)
        result += "Set-Cookie: " + Rafale::UriEncode(cookie.first) + '=' + Rafale::UriEncode(cookie.second.Value()) + "; expires=Sun, 17-Jan-2038 19:14:01 GMT; path=/; domain=" + Rafale::serverDatas["SERVER_ADDR"] + "\r\n";
    }
  return result;
}

int remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2;
          buf = (char*)malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}

int main(void)
{
  srand(static_cast<unsigned int>(time(0)) * getpid());
  Rafale::tmpDirectory = "/tmp/rafale"; // + Rafale::ToString(rand())
  Rafale::filesDirectory = Rafale::tmpDirectory + "/files";
  Rafale::sessionsDirectory = Rafale::tmpDirectory + "/sessions";

  remove_directory(Rafale::tmpDirectory.c_str());
  mkdir(Rafale::tmpDirectory.c_str(), 0700);
  mkdir(Rafale::sessionsDirectory.c_str(), 0700);
  mkdir(Rafale::filesDirectory.c_str(), 0700);
  while(FCGI_Accept() >= 0)
    {
      try {
        Dispatcher dispatcher(getenv("SCRIPT_FILENAME"));

        Rafale::Controller    *p = Caller::Make(dispatcher.Controller());

        Rafale::Session::Clean();
        GetServerData();
        GetGetData();
        GetCookies();
        SetContent();
        GetPostData();
        std::string data = p->Action(dispatcher.Action());
        FCGI_printf("%s", SetCookies().c_str());
        FCGI_printf("Content-type: text/html\r\n\r\n");
        FCGI_printf("%s", data.c_str());
        delete p;
        Rafale::serverDatas.clear();
        Rafale::getDatas.clear();
        Rafale::postDatas.clear();
        Rafale::cookies.clear();
        Rafale::files.clear();
      }
      catch(const char*s) {
        std::cerr << s << std::endl;
      }
    }
  // unlink(Rafale::tmpDirectory.c_str()); // CATCH SIGNAL TO DO THIS
}
