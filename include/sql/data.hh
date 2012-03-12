//////////////////
// Copyright (c) 2011, Jeremy Lecerf
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of R0x nor the
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

#ifndef _RAFALE_SQL_DATA_H_
#define _RAFALE_SQL_DATA_H_

#include <sstream>
#include "abstract.h"

namespace Rafale
{
  namespace SQL
  {
    struct Unknown { };
    enum Type
      {
        unknown = 0,
        integer,
        floating,
        boolean,
        dateTime,
        string,
      };

    struct Raw { };

    namespace Internal_
    {
      template <class ChildModel, typename T>
      struct TypeToEnum;

      // Unknown type
      template <class ChildModel>
      struct TypeToEnum<ChildModel, Unknown>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::unknown;
        }
      };

      // With ChilModel (pointer to member)
      template <class ChildModel>
      struct TypeToEnum<ChildModel, String ChildModel::*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::string;
        }
      };

      template <class ChildModel>
      struct TypeToEnum<ChildModel, int ChildModel::*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::integer;
        }
      };

      template <class ChildModel>
      struct TypeToEnum<ChildModel, bool ChildModel::*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::boolean;
        }
      };

      template <class ChildModel>
      struct TypeToEnum<ChildModel, float ChildModel::*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::floating;
        }
      };

      template <class ChildModel>
      struct TypeToEnum<ChildModel, Rafale::DateTime ChildModel::*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::dateTime;
        }
      };


      // With Raw Data (pointer to none pod (no enought with c++0x) and data for pod)
      template <>
      struct TypeToEnum<Raw, const String &>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::string;
        }
      };

      template <>
      struct TypeToEnum<Raw, String>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::string;
        }
      };

      template <>
      struct TypeToEnum<Raw, const char*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::string;
        }
      };

      template <>
      struct TypeToEnum<Raw, int>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::integer;
        }
      };

      template <>
      struct TypeToEnum<Raw, bool>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::boolean;
        }
      };

      template <>
      struct TypeToEnum<Raw, float>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::floating;
        }
      };

      template <>
      struct TypeToEnum<Raw, Rafale::DateTime>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::dateTime;
        }
      };


      // --------------------------

      template <class ChildModel>
      union BasicData
      {
        String          (ChildModel::*string);
        int             (ChildModel::*integer);
        bool            (ChildModel::*boolean);
        float           (ChildModel::*floating);
        Rafale::DateTime          (ChildModel::*dateTime);
      };

      template <>
      union BasicData<Raw>
      {
        String         *string;
        int             integer;
        bool            boolean;
        float           floating;
        Rafale::DateTime        *dateTime;
      };

      // --------------------------

      template <class ChildModel, typename T>
      struct Assign;

      // Unknown assign
      template <class ChildModel>
      struct Assign<ChildModel, Unknown>
      {
        Assign(BasicData<ChildModel> &, Unknown)
        {
        }
      };


      // With ChilModel (pointer to member)
      template <class ChildModel>
      struct Assign<ChildModel, String ChildModel::*>
      {
        Assign(BasicData<ChildModel> &data_, String ChildModel::*string)
        {
          data_.string = string;
        }
      };

      template <class ChildModel>
      struct Assign<ChildModel, int (ChildModel::*)>
      {
        Assign(BasicData<ChildModel> &data_, int ChildModel::*integer)
        {
          data_.integer = integer;
        }
      };

      template <class ChildModel>
      struct Assign<ChildModel, bool (ChildModel::*)>
      {
        Assign(BasicData<ChildModel> &data_, bool ChildModel::*boolean)
        {
          data_.boolean = boolean;
        }
      };


      template <class ChildModel>
      struct Assign<ChildModel, float (ChildModel::*)>
      {
        Assign(BasicData<ChildModel> &data_, float ChildModel::*floating)
        {
          data_.floating = floating;
        }
      };


      template <class ChildModel>
      struct Assign<ChildModel, Rafale::DateTime (ChildModel::*)>
      {
        Assign(BasicData<ChildModel> &data_, Rafale::DateTime ChildModel::*dateTime)
        {
          data_.dateTime = dateTime;
        }
      };


      // With Raw Data (pointer to none pod (no enought with c++0x) and data for pod)
      template <>
      struct Assign<Raw, const String &>
      {
        Assign(BasicData<Raw> &data_, const String &string)
        {
          data_.string = new String(string);
        }
      };

      template <>
      struct Assign<Raw, String>
      {
        Assign(BasicData<Raw> &data_, const String &string)
        {
          data_.string = new String(string);
        }
      };

      template <>
      struct Assign<Raw, const char *>
      {
        Assign(BasicData<Raw> &data_, const char *string)
        {
          data_.string = new String(string);
        }
      };

      template <>
      struct Assign<Raw, int>
      {
        Assign(BasicData<Raw> &data_, int integer)
        {
          data_.integer = integer;
        }
      };

      template <>
      struct Assign<Raw, bool>
      {
        Assign(BasicData<Raw> &data_, bool boolean)
        {
          data_.boolean = boolean;
        }
      };

      template <>
      struct Assign<Raw, float>
      {
        Assign(BasicData<Raw> &data_, float floating)
        {
          data_.floating = floating;
        }
      };

      template <>
      struct Assign<Raw, const DateTime &>
      {
        Assign(BasicData<Raw> &data_, const DateTime &dateTime)
        {
          data_.dateTime = new DateTime(dateTime);
        }
      };

      template <>
      struct Assign<Raw, DateTime>
      {
        Assign(BasicData<Raw> &data_, const DateTime &dateTime)
        {
          data_.dateTime = new DateTime(dateTime);
        }
      };


    }

    // ------------------------------------------########################################################################

    template <class ChildModel>
    class Data
    {
    public:
      template <typename T = Unknown>
      Data(T data = T()) : type_(Internal_::TypeToEnum<ChildModel, T>::Type())
      {
        SetData_(data);
      }

      ~Data()
      {
      }
    private:
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
    public:
      std::string       Serialize(ChildModel *ptr)
      {
        std::stringstream tmp;
        switch (Type())
          {
          case SQL::integer:
            tmp << ptr->*(data_.integer);
            return tmp.str();
            break;

          case SQL::boolean:
            tmp << ptr->*(data_.boolean);
            return tmp.str();
            break;

          case SQL::floating:
            tmp << ptr->*(data_.floating);
            return tmp.str();
            break;

          case SQL::string:
            return '\'' + EscapeSpecialChar(ptr->*(data_.string)) + '\'';
            break;

          case SQL::dateTime:
            return '\'' + EscapeSpecialChar((ptr->*(data_.dateTime)).ToString()) + '\'';
            break;

          default:
            return "";
            break;
          }
        return "";
      }

      SQL::Type   Type() const
      {
        return type_;
      }

      bool      Set(ChildModel &ptr, int value)
      {
        if (Type() == SQL::integer)
          {
            ptr.*(data_.integer) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel &ptr, bool value)
      {
        if (Type() == SQL::boolean)
          {
            ptr.*(data_.boolean) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel &ptr, float value)
      {
        if (Type() == SQL::floating)
          {
            ptr.*(data_.floating) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel &ptr, const std::string &value)
      {
        if (Type() == SQL::string)
          {
            ptr.*(data_.string) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel &ptr, const Rafale::DateTime &value)
      {
        if (Type() == SQL::dateTime)
          {
            ptr.*(data_.dateTime) = value;
            return true;
          }
        return false;
      }


      bool      Set(ChildModel &ptr, const char *value)
      {
        if (Type() == SQL::string)
          {
            ptr.*(data_.string) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel *ptr, int value)
      {
        if (Type() == SQL::integer)
          {
            ptr->*(data_.integer) = value;
            return true;
          }
        return false;
      }


      bool      Set(ChildModel *ptr, bool value)
      {
        if (Type() == SQL::boolean)
          {
            ptr->*(data_.boolean) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel *ptr, float value)
      {
        if (Type() == SQL::floating)
          {
            ptr->*(data_.floating) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel *ptr, const std::string &value)
      {
        if (Type() == SQL::string)
          {
            ptr->*(data_.string) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel *ptr, const char *value)
      {
        if (Type() == SQL::string)
          {
            ptr->*(data_.string) = value;
            return true;
          }
        return false;
      }

      bool      Set(ChildModel *ptr, const DateTime &value)
      {
        if (Type() == SQL::dateTime)
          {
            ptr->*(data_.dateTime) = value;
            return true;
          }
        return false;
      }

    private:
      template <typename T>
      void      SetData_(T data)
      {
        Internal_::Assign<ChildModel, T>(data_, data);
      }

    private:
      const SQL::Type           type_;
      Internal_::BasicData<ChildModel>      data_;
    };


    // RAW SPECIALIZATION

    template <>
    class Data<Raw>
    {
    public:
      template <typename T = Unknown>
      Data(T data = T()) : type_(Internal_::TypeToEnum<Raw, T>::Type())
      {
        SetData_(data);
      }

      Data(const Data<Raw> &other) : type_(other.type_), data_(other.data_)
      {
        switch (type_)
          {
          case SQL::string:
            data_.string = new String(*other.data_.string);
            break;

          case SQL::dateTime:
            data_.dateTime = new Rafale::DateTime(*other.data_.dateTime);
            break;

          default:
            break;
          }

      }

      ~Data()
      {
        switch (Type())
          {
          case SQL::string:
            delete data_.string;
            break;

          case SQL::dateTime:
            delete data_.dateTime;
            break;

          default:
            break;
          }
      }
    private:
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
    public:
      std::string       Serialize()
      {
        std::stringstream tmp;
        switch (Type())
          {
          case SQL::integer:
            tmp << data_.integer;
            return tmp.str();
            break;

          case SQL::boolean:
            tmp << data_.boolean;
            return tmp.str();
            break;

          case SQL::floating:
            tmp << data_.floating;
            return tmp.str();
            break;

          case SQL::string:
            return '\'' + EscapeSpecialChar(*(data_.string)) + '\'';
            break;

          case SQL::dateTime:
            return '\'' + EscapeSpecialChar(data_.dateTime->ToString()) + '\'';
            break;

          default:
            return "";
            break;
          }
            return "";
      }

      SQL::Type   Type() const
      {
        return type_;
      }

    private:
      template <typename T>
      void      SetData_(T data)
      {
        Internal_::Assign<Raw, T>(data_, data);
      }

    private:
      const SQL::Type           type_;
      Internal_::BasicData<Raw>      data_;
    };
  }
}


#endif /* _RAFALE_SQL_DATA_H_ */
