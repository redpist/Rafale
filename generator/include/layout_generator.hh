#ifndef _RAFALE_LAYOUT_GENERATOR_H_
#define _RAFALE_LAYOUT_GENERATOR_H_

#include <fstream>
#include <iostream>

class LayoutGenerator
{
public:
  LayoutGenerator()
  {
  }

  void  Generate(const std::string &fileName, const std::string &outputFileName, const std::string &layoutName)
  {
    std::ifstream viewFile(fileName);

   std::ofstream outputFile(outputFileName);
   outputFile_ = &outputFile;

    if (!viewFile.is_open())
      throw ("no view file : " + fileName);
    if (!outputFile.is_open())
      throw ("problem while opening : " + outputFileName);

    outputFile << "#ifndef _LAYOUT_" + layoutName + "_H_\n";
    outputFile << "# define _LAYOUT_" + layoutName + "_H_\n\n";
    outputFile << "#include \"layout.hh\"\n\n";
    outputFile << "struct " + layoutName + " : public Rafale::Layout\n{\n";
    outputFile << ("void Print(Rafale::View &view)\n{\n");

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
                buffer_ = buffer_.substr(offset + sizeof("<c++>") - 1, buffer_.size() - offset);
                goto inCplusplus;
              }
            else
              PrintHtml(buffer_);
          }
        else
          {
inCplusplus:
            if ((offset = buffer_.find("</c++>")) != std::string::npos)
              {
                PrintCPlusPlus(buffer_.substr(0, offset));
                isInCPlusPlus_ = false;
                PrintHtml(buffer_.substr(offset + sizeof("</c++>") - 1, buffer_.size() - offset));
              }
            else
              {
                PrintCPlusPlus(buffer_);
                outputFile << "\n";
              }
          }
      }
    outputFile << "std::cout << std::flush;\n";
    outputFile << "}\n};\n";
    outputFile << "#endif" << std::endl;
  }

  ~LayoutGenerator()
  {
  }
private:
  void  PrintHtml(const std::string& buffer)
  {
    (*outputFile_) << "std::cout << \"" << EscapeSpecialChar(buffer) << "\" << \"\\n\";\n";
  }

  void  PrintCPlusPlus(const std::string& buffer)
  {
    (*outputFile_) << buffer;
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

  std::ofstream *outputFile_;
  std::string   buffer_;
  bool          isInCPlusPlus_;
};

#endif /* _RAFALE_LAYOUT_GENERATOR_H_ */
