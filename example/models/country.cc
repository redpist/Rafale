#include "country.hh"

decltype(Country::datas_)          Country::datas_ = {
  {"id", &Country::id},
  {"name", &Country::name}
};

decltype(Country::primaries)       Country::primaries         = { "id" };
decltype(Country::autoIncrements)  Country::autoIncrements    = { "id" };
decltype(Country::tableName)       Country::tableName         = "Countries";
