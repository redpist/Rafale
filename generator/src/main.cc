#include "generator.h"

int main(int argc, char *argv[])
{
  Generator     generator;

  try
    {
      if (argc != 2)
        throw ("Usage: RafaleGenerator path\n");
      generator.Generate(argv[1]);
    }
  catch (std::string s)
    {
      std::cerr << s << std::endl;
      return 2;
    }
  catch (const char *s)
    {
      std::cerr << s << std::endl;
      return 2;
    }
  return 0;
}
