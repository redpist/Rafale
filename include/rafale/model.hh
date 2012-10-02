//////////////////
// Copyright (c) 2012, Jeremy Lecerf
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Rafale nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL JEREMY LECERF BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//////////////////

#ifndef _RAFALE_MODEL_H_
#define _RAFALE_MODEL_H_

#include "rafale/abstract.hh"
#include "rafale/config.hh"
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

#include "rafale/sql.hh"

namespace Rafale
{
  class BasicModel
  {
  protected:
    BasicModel()
    {
    }
  public:
    static void        UseDatabase()
    {
      Rafale::BasicModel::con_.reset();
      Rafale::BasicModel::con_ = std::shared_ptr<sql::Connection>(driver_->connect(Rafale::BasicModel::host_, Rafale::BasicModel::user_, Rafale::BasicModel::password_));
      std::shared_ptr<sql::Statement> stmt = std::unique_ptr<sql::Statement>(BasicModel::con_->createStatement());
      stmt->execute("USE `" + BasicModel::dataBase_ + "`");
    }
  protected:
    static sql::Driver                         *driver_;
    static std::shared_ptr<sql::Connection>    con_;
    static std::string dataBase_;
    static std::string host_;
    static std::string user_;
  private:
    static std::string password_;
  };

  template <class ChildModel>
  class Model : public BasicModel
  {
  protected:
    Model()
    {
      stmt_ = std::unique_ptr<sql::Statement>(BasicModel::con_->createStatement());
    }

    ~Model()
    {
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
              ChildModel::datas_[autoIncrement].Set(static_cast<ChildModel*>(this), res->getInt(1));
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


  typedef std::shared_ptr<std::list<ChildModel> >     List;

  protected:
    std::shared_ptr<sql::Statement>     stmt_;
    std::string dataBase_;
  };
}

#endif /* _RAFALE_MODEL_H_ */

