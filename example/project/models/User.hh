#ifndef _MODEL_USER_H_
#define _MODEL_USER_H_

#include <string>
#include "model.hh"

class User : public Rafale::Model<User> 
{
public:
  User() { }
  ~User() { }

  int               id;
  std::string       mail;
  std::string       login;
  std::string       password;
  Rafale::DateTime  last_login;
  Rafale::DateTime  registration;
  
protected:
  friend class Rafale::Model<User>;
  friend std::shared_ptr<std::list<User>> Rafale::SQL::Query<User>(const std::map<std::string, Rafale::SQL::Data<Rafale::SQL::Raw>> &);
  
  static const std::string tableName;    
  static std::vector<std::string> primaries;
  static std::vector<std::string> autoIncrements;
  static std::map<std::string, Rafale::SQL::Data<User>> datas_;
};


#endif /* _MODEL_USER_H_ */
