#include "User.hh"

decltype(User::datas_)          User::datas_ = {
  {"id", &User::id},
  {"mail", &User::mail},
  {"login", &User::login},
  {"password", &User::password},
  {"last_login", &User::last_login},
  {"registration", &User::registration}
};

decltype(User::primaries)       User::primaries         = { "id" };
decltype(User::autoIncrements)  User::autoIncrements    = { "id" };
decltype(User::tableName)       User::tableName         = "users";


