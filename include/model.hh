#ifndef _MODEL_H_
#define _MODEL_H_

#include "abstract.h"
#include "config.hh"
#include <string>
#include <algorithm>
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

#include "sql.hh"

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

      con_ = std::auto_ptr<sql::Connection>(driver_->connect(host_, user_, password));
      stmt_ = std::auto_ptr<sql::Statement>(con_->createStatement());

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

  public:
    bool        Insert()
    {
      std::string sql;
      sql = "INSERT INTO `" + ChildModel::tableName + "` (";
      {
        int nbCommas = 0;
        for (auto member : ChildModel::datas_)
          {
            if ((find(begin(ChildModel::autoIncrements), end(ChildModel::autoIncrements), member.first) == end(ChildModel::autoIncrements))
                && (member.second.Type() != Rafale::SQL::unknown))
              {
                sql += member.first + ", ";
                ++nbCommas;
              }
          }
        if (nbCommas > 0)
          sql.erase(sql.end() - 2, sql.end()); // clean last comma
      }
      sql += ") VALUES (";
      {
        int nbCommas = 0;
        for (auto member : ChildModel::datas_)
          {
            if ((find(begin(ChildModel::autoIncrements), end(ChildModel::autoIncrements), member.first) == end(ChildModel::autoIncrements))
                && (member.second.Type() != Rafale::SQL::unknown))
              {

                sql += member.second.Serialize(static_cast<ChildModel*>(this)) + ", ";
                ++nbCommas;
              }
          }
        if (nbCommas > 0)
          sql.erase(sql.end() - 2, sql.end()); // clean last comma
      }
      sql += ");";
      stmt_->execute("START TRANSACTION");
      stmt_->execute(sql);
      std::unique_ptr<sql::ResultSet>  res(stmt_->executeQuery("SELECT LAST_INSERT_ID() FROM `" + ChildModel::tableName + "`"));
      stmt_->execute("COMMIT");
      // START TRANSACTION;
      if (res->next())
        {
          for (auto autoIncrement : ChildModel::autoIncrements)
            {
              ChildModel::datas_[autoIncrement].SetInt(static_cast<ChildModel*>(this), res->getInt(1));
            }
          // stmt_->execute(sql);
          return true;  // TODO RETURN SUCCESS OF INSERT
        }
      return false;
    }

    std::size_t        Update(const std::map<std::string, Rafale::SQL::Data<Rafale::SQL::Raw> > &where)
    {
      std::string sql;

      sql = "UPDATE `" + ChildModel::tableName + "`";
      sql += "SET ";
      {
        int nbCommas = 0;
        for (auto member : ChildModel::datas_)
          {
            if ((find(begin(ChildModel::primaries), end(ChildModel::primaries), member.first) == end(ChildModel::primaries)) && (member.second.Type() != Rafale::SQL::unknown) && (where.find(member.first) == end(where)))
              {
                sql += '`' + ChildModel::tableName + "`.`"  + member.first + "`=" + member.second.Serialize(static_cast<ChildModel*>(this)) + ", ";
                ++nbCommas;
              }
          }
        if (nbCommas > 0)
          sql.erase(sql.end() - 2, sql.end()); // clean last comma
      }
      sql += " WHERE ";
      int i  = 0;
      for (auto criteria : where)
        {
          if (i++)
            sql += " AND ";
          sql += '`' + ChildModel::tableName  + "`.`" + criteria.first + "`="  + criteria.second.Serialize();
        }
      stmt_->execute(sql);
      return 1;  // TODO RETURN NB OF UPDATE
    }

    std::size_t        Update()
    {
      std::string sql;

      sql = "UPDATE `" + ChildModel::tableName + "`";
      sql += "SET ";
      {
        int nbCommas = 0;
        for (auto member : ChildModel::datas_)
          {
            if ((find(begin(ChildModel::primaries), end(ChildModel::primaries), member.first) == end(ChildModel::primaries)) && (member.second.Type() != Rafale::SQL::unknown))
              {
                sql += '`' + ChildModel::tableName + "`.`"  + member.first + "`=" + member.second.Serialize(static_cast<ChildModel*>(this)) + ", ";
                ++nbCommas;
              }
          }
        if (nbCommas > 0)
          sql.erase(sql.end() - 2, sql.end()); // clean last comma
      }
      sql += " WHERE ";
      int i  = 0;
      for (auto primary : ChildModel::primaries)
        {
          if (i++)
            sql += " AND ";
          sql += '`' + ChildModel::tableName  + "`.`" + primary + "`=" + ChildModel::datas_[primary].Serialize(static_cast<ChildModel*>(this));
        }
      stmt_->execute(sql);
      return 1;  // TODO RETURN NB OF UPDATE
    }

    std::size_t        Delete(const std::map<std::string, Rafale::SQL::Data<Rafale::SQL::Raw> > &where)
    {
      std::string sql;

      sql = "DELETE FROM `" + ChildModel::tableName + "`";
      sql += " WHERE ";
      int i  = 0;
      for (auto criteria : where)
        {
          if (i++)
            sql += " AND ";
          sql += '`' + ChildModel::tableName  + "`.`" + criteria.first + "`=" + criteria.second.Serialize();
        }
      stmt_->execute(sql);
      return 1; // TODO RETURN NB OF DELETE
    }

    std::size_t        Delete()
    {
      std::string sql;

      sql = "DELETE FROM `" + ChildModel::tableName + "`";
      sql += " WHERE ";
      int i  = 0;
      for (auto primary : ChildModel::primaries)
        {
          if (i++)
            sql += " AND ";
          sql += '`' + ChildModel::tableName  + "`.`" + primary + "`=" + ChildModel::datas_[primary].Serialize(static_cast<ChildModel*>(this));
        }
      stmt_->execute(sql);
      return 1; // TODO RETURN NB OF DELETE
    }

  private:
    sql::Driver                         *driver_;
    std::auto_ptr<sql::Connection>    con_;
    std::auto_ptr<sql::Statement>     stmt_;
  protected:
    std::string dataBase_;
    std::string host_;
    std::string user_;
  };
}

#endif /* _MODEL_H_ */

