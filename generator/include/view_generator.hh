#ifndef _RAFALE_VIEW_GENERATOR_H_
#define _RAFALE_VIEW_GENERATOR_H_

#include <fstream>
#include <iostream>
#include "cchtml_generator.hh"

class ViewGenerator : public CCHTMLGenerator
{
public:
  ViewGenerator()
  {
  }

  void  Generate(const std::string &controllerName, const std::string &viewName, const std::string &fileName, const std::string &outputFileName)
  {
    std::ifstream viewFile(fileName);

   std::ofstream outputFile(outputFileName);
   outputFile_ = &outputFile;

    if (!viewFile.is_open())
      throw ("no view file : " + fileName);
    if (!outputFile.is_open())
      throw ("problem while opening : " + outputFileName);

    outputFile << "#include \"._controller_" + controllerName + ".hh\"\n";

    outputFile << ("void _Controller_" + controllerName + "::_View_" + viewName + "::Print()\n{\n");

    CCHTMLGenerator::Generate(viewFile);

    outputFile << "std::cout << std::flush;\n}" << std::endl;
  }

  ~ViewGenerator()
  {
  }
};

#endif /* _RAFALE_VIEW_GENERATOR_H_ */
