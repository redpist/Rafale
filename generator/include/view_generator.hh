#ifndef _RAFALE_VIEW_GENERATOR_H_
#define _RAFALE_VIEW_GENERATOR_H_

#include <fstream>
#include <iostream>

class ViewGenerator
{
public:
  ViewGenerator()
  {
  }

  void  Generate(const std::string &controllerName, const std::string &viewName, const std::string &fileName)
  {
    std::ifstream viewFile(fileName);

    if (!viewFile.is_open())
      throw ("no view file");

    std::cout << ("void _Controller_" + controllerName + "::_View_" + viewName + "::Print()\n{\n");

    std::size_t offset;
    isInCPlusPlus_ = false;
    while (!viewFile.eof())
      {
        getline(viewFile, buffer_);
        if (!isInCPlusPlus_)
          {
            if ((offset = buffer_.find("<c++>")) != std::string::npos)
              {
                PrintHtml(buffer_.substr(0, offset));
                isInCPlusPlus_ = true;
                PrintCPlusPlus(buffer_.substr(offset + sizeof("<c++>") - 1, buffer_.size() - offset));
                std::cout << "\n";
              }
            else
              PrintHtml(buffer_);
          }
        else
          {
            if ((offset = buffer_.find("</c++>")) != std::string::npos)
              {
                PrintCPlusPlus(buffer_.substr(0, offset));
                isInCPlusPlus_ = false;
                PrintHtml(buffer_.substr(offset + sizeof("</c++>") - 1, buffer_.size() - offset));
              }
            else
              {
                PrintCPlusPlus(buffer_);
                std::cout << "\n";
              }
          }
      }
    std::cout << "std::cout << std::endl;\n}" << std::endl;
  }

  ~ViewGenerator()
  {
  }
private:
  void  PrintHtml(const std::string& buffer)
  {
    std::cout << "std::cout << \"" << EscapeSpecialChar(buffer) << "\" << \"\\n\";\n";
  }

  void  PrintCPlusPlus(const std::string& buffer)
  {
    std::cout << buffer;
  }

  inline std::string EscapeSpecialChar(const std::string& buffer)
  {
    std::string result;
    for (auto it = begin(buffer); it != end(buffer); ++it)
      {
        switch (*it)
          {
          case '\\':
            result += "\\\\";
            break;
          case '"':
            result += "\\\"";
            break;
          default:
            result += *it;
            break;
          }
      }
    return result;
  }

  std::string   buffer_;
  bool          isInCPlusPlus_;
};

#endif /* _RAFALE_VIEW_GENERATOR_H_ */
