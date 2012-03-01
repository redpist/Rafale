#ifndef _SQL_H_
#define _SQL_H_

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
        // dateTime,
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
      struct TypeToEnum<ChildModel, float ChildModel::*>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::floating;
        }
      };


      // With Raw Data (pointer to none pod (no enought with c++0x) and data for pod)
      template <>
      struct TypeToEnum<Raw, String*>
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
      struct TypeToEnum<Raw, float>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::floating;
        }
      };


      // --------------------------

      template <class ChildModel>
      union BasicData
      {
        String          (ChildModel::*string);
        int             (ChildModel::*integer);
        float           (ChildModel::*floating);
        // DateTime        *dateTime;
      };

      template <>
      union BasicData<Raw>
      {
        String          *string;
        int             integer;
        float           floating;
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
      struct Assign<ChildModel, float (ChildModel::*)>
      {
        Assign(BasicData<ChildModel> &data_, float ChildModel::*floating)
        {
          data_.floating = floating;
        }
      };

      // With Raw Data (pointer to none pod (no enought with c++0x) and data for pod)
      template <>
      struct Assign<Raw, String *>
      {
        Assign(BasicData<Raw> &data_, String *string)
        {
          data_.string = string;
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
      struct Assign<Raw, float>
      {
        Assign(BasicData<Raw> &data_, float floating)
        {
          data_.floating = floating;
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
        switch (Type())
          {
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
      std::string       Serialize(ChildModel *ptr)
      {
        std::stringstream tmp;
        switch (Type())
          {
          case SQL::integer:
            tmp << ptr->*(data_.integer);
            return tmp.str();
            break;

          case SQL::floating:
            tmp << ptr->*(data_.floating);
            return tmp.str();
            break;

          case SQL::string:
            return '\'' + EscapeSpecialChar(ptr->*(data_.string)) + '\'';
            break;

          default:
            return "";
            break;
          }
      }

      SQL::Type   Type() const
      {
        return type_;
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

    template <>
    class Data<Raw>
    {
    public:
      template <typename T = Unknown>
      Data(T data = T()) : type_(Internal_::TypeToEnum<Raw, T>::Type())
      {
        SetData_(data);
      }

      ~Data()
      {
        switch (Type())
          {
          case SQL::string:
            delete data_.string;
            break;

          // case SQL::dateTime:
          //   delete data_.dateTime;
          //   break;

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

          case SQL::floating:
            tmp << data_.floating;
            return tmp.str();
            break;

          case SQL::string:
            return '\'' + EscapeSpecialChar(*(data_.string)) + '\'';
            break;

          default:
            return "";
            break;
          }
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

#endif /* _SQL_H_ */
