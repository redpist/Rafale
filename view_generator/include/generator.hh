#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <fstream>
#include <iostream>

class Generator
{
public:
  Generator()
  {
  }

  void  Generate(std::string fileName)
  {
    std::ifstream viewFile(fileName);

    if (!viewFile.is_open())
      throw ("no view file");

    std::cout << "void View::Print()\n{\n";

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

  ~Generator()
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

#endif /* _GENERATOR_H_ */
