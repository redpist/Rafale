#include "generator.hh"

int main(int argc, char *argv[])
{
  Generator     generator;

  (void)argc;
  generator.Generate(argv[1]);
  return 0;
}
