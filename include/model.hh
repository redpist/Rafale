#ifndef _MODEL_H_
#define _MODEL_H_

#include "abstract.h"

namespace Rafale
{
  class Model
  {
  public:
    Model()
    {
    }

    virtual ~Model()
    {
    }

    void        Insert(const  &t)
    {
      
    }
  private:
    sql::Driver                           *driver_;
    sql::Connection                       con_;
  };
}

#endif /* _MODEL_H_ */
