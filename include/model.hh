#ifndef _MODEL_H_
#define _MODEL_H_

#include "abstract.h"
#include "config.hh"
#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>
#include <cppconn/metadata.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <mysql_connection.h>

#define RafaleModel(X) X : public Rafale::Model<X>

namespace Rafale
{
  template <class ChildModel>
  class Model
  {
  protected:
    Model() : driver_(sql::mysql::get_driver_instance())
    {
      host_ = Rafale::config["db.host"];
      if (!host_.size())
        {
         //todo erreur no db.host section in config.cc
        }

      user_ = Rafale::config["db.user"];
      if (!user_.size())
        {
         //todo erreur no db.user section in config.cc
        }

      std::string password = Rafale::config["db.password"];
      if (!password.size())
        {
         //todo erreur no db.password section in config.cc
        }

      con_ = driver_->connect(host_, user_, password);
      stmt_ = con_->createStatement();

      dataBase_ = Rafale::config["db.database"];
      if (!dataBase_.size())
        {
         //todo erreur no db.database section in config.cc
        }
      stmt_->execute("USE `" + dataBase_ + "`");
    }

    ~Model()
    {
      stmt_.reset(NULL);
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
      return result + "\\n";
    }

    // void        Insert(const  &t)
    // {
      
    // }
  private:
    sql::Driver * driver_;
    std::auto_ptr< sql::Connection > con_;
    std::auto_ptr< sql::Statement > stmt_;
  protected:
    std::string dataBase_;
    std::string host_;
    std::string user_;
  };
}

#endif /* _MODEL_H_ */
