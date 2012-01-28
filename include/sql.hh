#ifndef _SQL_H_
#define _SQL_H_

#include "abstract.h"

namespace Rafale
{
  namespace SQL
  {
    enum Type
      {
        integer = 0,
        floating,
        dateTime,
        string,
      };

    namespace Internal_
    {
      template <typename T>
      struct TypeToEnum
      {
        static constexpr SQL::Type Type()
        {
          return SQL::string;
        }
      };

      template <>
      struct TypeToEnum<int>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::integer;
        }
      };

      template <>
      struct TypeToEnum<float>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::floating;
        }
      };

      template <>
      struct TypeToEnum<DateTime>
      {
        static constexpr SQL::Type Type()
        {
          return SQL::dateTime;
        }
      };

      // --------------------------


      template <int N>
      struct EnumToType
      {
        typedef String Type;
      };

      // --------------------------

      union BasicData
      {
        String          *string;
        int             integer;
        float           floating;
        DateTime        *dateTime;
      };

      // --------------------------

      template <typename T>
      struct Assign
      {
        Assign(BasicData &data_, const T& data)
        {
          data_.string = new String(data);
        }
      };

      template <>
      struct Assign<int>
      {
        Assign(BasicData &data_, int integer)
        {
          data_.integer = integer;
        }
      };

      template <>
      struct Assign<float>
      {
        Assign(BasicData &data_, float floating)
        {
          data_.floating = floating;
        }
      };

      template <>
      struct Assign<DateTime>
      {
        Assign(BasicData &data_, const DateTime &dateTime)
        {
          data_.dateTime = new DateTime(dateTime);
        }
      };
    }

    class Data
    {
    public:
      template <typename T>
      Data(T data) : type_(Internal_::TypeToEnum<T>::Type())
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

          case SQL::dateTime:
            delete data_.dateTime;
            break;

          default:
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
        Internal_::Assign<T>(data_, data);
      }

      const SQL::Type type_;
      Internal_::BasicData      data_;
    };
  }
}

#endif /* _SQL_H_ */
