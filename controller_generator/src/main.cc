#include "generator.hh"

int main(int argc, char *argv[])
{
  Generator     generator;

  (void)argc;
  try
    {
  generator.Generate(argv[1]);
    }
  catch (std::string s)
    {
      std::cerr << s << std::endl;
      return 2;
    }
  return 0;
}
