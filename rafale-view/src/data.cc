#include "data.h"

const std::string Data::Eval::openingToken = "<c++(";
const std::string Data::Eval::closingToken = ")/>";

const std::string Data::CPlusPlus::openingToken = "<c++>";
const std::string Data::CPlusPlus::closingToken = "</c++>";

std::ostream &operator<<(std::ostream &out, const Data &data)
{
  return (out << data.Serialize());
}
