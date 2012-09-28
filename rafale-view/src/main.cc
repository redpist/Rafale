#include <iostream>
#include "options.h"
#include "view.h"

int main(int argc, const char *argv[])
{
  try
    {
      Options   options(argc, argv);
      options.Parse();
      if (options.IsVerbose())
        {
          std::cout << "input file\t: " << options.InputFile() << std::endl;
          std::cout << "output file\t: " << options.OutputFile() << std::endl;
        }
      Rafale::View      view(options.InputFile());
      view.Export(options.OutputFile());
    }
  catch (std::string s)
    {
      std::cerr << s << std::flush;
      return 2;
    }
  catch (const char *s)
    {
      std::cerr << s << std::flush;
      return 2;
    }
  return 0;
}
