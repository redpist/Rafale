#ifndef _MODEL_H_
#define _MODEL_H_

#include "abstract.h"

namespace Rafale
{
  class Model
  {
  protected:
    Model() : status_(deprecated)
    {
    }

    ~Model()
    {
    }

    // void        Insert(const  &t)
    // {
      
    // }
  private:
    // sql::Driver                           *driver_;
    // sql::Connection                       con_;
    enum Status
      {
        upToDate,
        deprecated
      };
    Status        status_;
  };
}

#endif /* _MODEL_H_ */
