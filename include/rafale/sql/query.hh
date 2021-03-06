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

#ifndef _RAFALE_SQL_QUERY_H_
#define _RAFALE_SQL_QUERY_H_

#include <list>
#include <fstream>
#include <string>
#include <rafale/config.hh>
#include <rafale/sql/tools.hh>

namespace Rafale
{
  namespace SQL
  {
    namespace Internal_
    {
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
              case '\'':
                result += "\\\'";
                break;
              default:
                result += *it;
                break;
              }
          }
        return result;
      }
    }

    inline std::string Password(const std::string &str)
    {
      sql::Driver                         *driver =sql::mysql::get_driver_instance() ;
      std::unique_ptr<sql::Connection>    connection;
      std::unique_ptr<sql::Statement>     statement;
      std::string dataBase;
      std::string host;
      std::string user;


      // std::ofstream outputFile("/var/log/rafale/query.log"); // LOGS

      host = Rafale::config["db.host"];
      if (!host.size())
        {
          //todo erreur no db.host section in config.cc
        }

      user = Rafale::config["db.user"];
      if (!user.size())
        {
          //todo erreur no db.user section in config.cc
        }

      std::string password = Rafale::config["db.password"];
      if (!password.size())
        {
          //todo erreur no db.password section in config.cc
        }

      connection = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
      statement = std::unique_ptr<sql::Statement>(connection->createStatement());

      dataBase = Rafale::config["db.database"];
      if (!dataBase.size())
        {
          //todo erreur no db.database section in config.cc
        }
      std::string       sql;
      sql = "SELECT PASSWORD('" + Internal_::EscapeSpecialChar(str) + "')";
      std::unique_ptr<sql::ResultSet>  res(statement->executeQuery(sql));
      if (res->next())
        return res->getString(1);
      return "";
    }

    template <class CurrentModel>
    std::shared_ptr<std::list<CurrentModel>>    Query(const std::map<std::string, Rafale::SQL::Data<Rafale::SQL::Raw> > &where)
    {
      sql::Driver                         *driver =sql::mysql::get_driver_instance() ;
      std::unique_ptr<sql::Connection>    connection;
      std::unique_ptr<sql::Statement>     statement;
      std::string dataBase;
      std::string host;
      std::string user;


      // std::ofstream outputFile("/var/log/rafale/query.log"); // LOGS

      host = Rafale::config["db.host"];
      if (!host.size())
        {
          //todo erreur no db.host section in config.cc
        }

      user = Rafale::config["db.user"];
      if (!user.size())
        {
          //todo erreur no db.user section in config.cc
        }

      std::string password = Rafale::config["db.password"];
      if (!password.size())
        {
          //todo erreur no db.password section in config.cc
        }

      connection = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
      statement = std::unique_ptr<sql::Statement>(connection->createStatement());

      dataBase = Rafale::config["db.database"];
      if (!dataBase.size())
        {
          //todo erreur no db.database section in config.cc
        }
      statement->execute("USE `" + dataBase + "`");
      std::string       sql;
      sql = "SELECT ";
      {
        int nbCommas = 0;
        for (auto member : CurrentModel::datas_)
          {
            sql += member.first + ", ";
            ++nbCommas;
          }
        if (nbCommas > 0)
          sql.erase(sql.end() - 2, sql.end()); // clean last comma
        sql += ' ';
      }
      sql += "FROM " + CurrentModel::tableName;
      if (where.size())
        {
          sql += " WHERE ";
          int i  = 0;
          for (auto criteria : where)
            {
              if (i++)
                sql += " AND ";
              sql += '`' + CurrentModel::tableName  + "`.`" + criteria.first + "`="  + criteria.second.Serialize();
            }
        }

      // if (outputFile.is_open()) // LOGS
      //   {
      //     outputFile << "Query: " << sql << std::endl;
      //   }
      std::shared_ptr<std::list<CurrentModel>> result(new std::list<CurrentModel>());
      std::unique_ptr<sql::ResultSet>  res(statement->executeQuery(sql));
      while (res->next())
        {
          CurrentModel model;
          for (auto member : CurrentModel::datas_)
          {
            switch (member.second.Type())
              {
              case SQL::integer:
                member.second.Set(model, res->getInt(member.first));
                break;

              case SQL::boolean:
                member.second.Set(model, static_cast<bool>(res->getInt(member.first)));
                break;

              case SQL::floating:
                member.second.Set(model, SQL::Tools::GetFloat<decltype(res), decltype(member.first)>(res, member.first));
                break;

              case SQL::string:
                member.second.Set(model, res->getString(member.first));
                break;

              case SQL::dateTime:
                member.second.Set(model, Rafale::DateTime(res->getString(member.first)));
                break;

              default:
                break;
              }
          }
          result->push_back(model);
        }
      return result;
    }


    template <class CurrentModel>
    std::shared_ptr<std::list<CurrentModel>>    Query(const std::map<std::string, Rafale::SQL::Data<Rafale::SQL::Raw> > &where, std::string order)
    {
      sql::Driver                         *driver =sql::mysql::get_driver_instance() ;
      std::unique_ptr<sql::Connection>    connection;
      std::unique_ptr<sql::Statement>     statement;
      std::string dataBase;
      std::string host;
      std::string user;


      // std::ofstream outputFile("/var/log/rafale/query.log"); // LOGS

      host = Rafale::config["db.host"];
      if (!host.size())
        {
          //todo erreur no db.host section in config.cc
        }

      user = Rafale::config["db.user"];
      if (!user.size())
        {
          //todo erreur no db.user section in config.cc
        }

      std::string password = Rafale::config["db.password"];
      if (!password.size())
        {
          //todo erreur no db.password section in config.cc
        }

      connection = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
      statement = std::unique_ptr<sql::Statement>(connection->createStatement());

      dataBase = Rafale::config["db.database"];
      if (!dataBase.size())
        {
          //todo erreur no db.database section in config.cc
        }
      statement->execute("USE `" + dataBase + "`");
      std::string       sql;
      sql = "SELECT ";
      {
        int nbCommas = 0;
        for (auto member : CurrentModel::datas_)
          {
            sql += member.first + ", ";
            ++nbCommas;
          }
        if (nbCommas > 0)
          sql.erase(sql.end() - 2, sql.end()); // clean last comma
        sql += ' ';
      }
      sql += "FROM " + CurrentModel::tableName;
      if (where.size())
        {
          sql += " WHERE ";
          int i  = 0;
          for (auto criteria : where)
            {
              if (i++)
                sql += " AND ";
              sql += '`' + CurrentModel::tableName  + "`.`" + criteria.first + "`="  + criteria.second.Serialize();
            }
        }
      if (order.size())
        {
          sql += "ORDER BY `" + order + "`"; // !!!!!! INJECTION HERE !! GO MOVE MY F***ING ASS TO CREATE TRUE QUERIES
        }

      // if (outputFile.is_open()) // LOGS
      //   {
      //     outputFile << "Query: " << sql << std::endl;
      //   }
      std::shared_ptr<std::list<CurrentModel>> result(new std::list<CurrentModel>());
      std::unique_ptr<sql::ResultSet>  res(statement->executeQuery(sql));
      while (res->next())
        {
          CurrentModel model;
          for (auto member : CurrentModel::datas_)
          {
            switch (member.second.Type())
              {
              case SQL::integer:
                member.second.Set(model, res->getInt(member.first));
                break;

              case SQL::boolean:
                member.second.Set(model, static_cast<bool>(res->getInt(member.first)));
                break;

              case SQL::floating:
                member.second.Set(model, SQL::Tools::GetFloat<decltype(res), decltype(member.first)>(res, member.first));
                break;

              case SQL::string:
                member.second.Set(model, res->getString(member.first));
                break;

              case SQL::dateTime:
                member.second.Set(model, Rafale::DateTime(res->getString(member.first)));
                break;

              default:
                break;
              }
          }
          result->push_back(model);
        }
      return result;
    }

  }
}

#endif /* _RAFALE_SQL_QUERY_H_ */
