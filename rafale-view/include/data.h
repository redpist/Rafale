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

#ifndef _DATA_H_
#define _DATA_H_

#include "options.h"

#define TypeValue(DataType)                     \
  c##DataType

#define AdHocEnum(DataType)                     \
  c##DataType

#define AdHocUnion(DataType)                    \
  DataType c##DataType##_

#define AdHocConstructor(DataType)                  \
  Data(const Data::DataType &data) :                \
    type_(Data::Type::c##DataType)                  \
  {                                                 \
    new (&data_.c##DataType##_)                     \
      DataType(data);                               \
  }                                                 \

#define AdHocCopyConstructor(DataType)              \
  case Data::Type::c##DataType:                     \
  new (&data_.c##DataType##_)                       \
    DataType(oth.data_.c##DataType##_);             \
  break;


#define AdHocSerialize(DataType)                \
  case Data::Type::c##DataType:                 \
  return data_.c##DataType##_.Serialize();      \
  break;

#define AdHocDestructor(DataType)               \
  case Data::Type::c##DataType:                 \
  data_.c##DataType##_.~DataType();             \
  break;


class Data
{
public:

/* STEP 0 : include data declarations */
#include "data_/raw.inc"
#include "data_/cplusplus.inc"
#include "data_/eval.inc"


  enum Type : unsigned char
    {
/* STEP 1 : add associated value in enum */
      AdHocEnum(Raw),
      AdHocEnum(CPlusPlus),
      AdHocEnum(Eval)
    };

private:

  union DatasUnion
  {
    DatasUnion() { }
    ~DatasUnion() { }
/* STEP 2 : add data to union */
    AdHocUnion(Raw);
    AdHocUnion(CPlusPlus);
    AdHocUnion(Eval);
  };

public:
  Data() = delete;

  Data(const Data &oth) { *this = oth; }

  Data& operator=(const Data &oth)
  {
    switch (type_ = oth.type_)
      {
/* STEP 3 : declare data associated case to correctly construct union  */
        AdHocCopyConstructor(Raw);
        AdHocCopyConstructor(CPlusPlus);
        AdHocCopyConstructor(Eval);
      }
    return *this;
  }


/* STEP 4 : declare data associated  constructor  */
  AdHocConstructor(Raw)
  AdHocConstructor(CPlusPlus)
  AdHocConstructor(Eval)

  std::string   Serialize() const
  {
    switch (type_)
      {
/* STEP 5 : declare data associated case to seralize data  */
        AdHocSerialize(Raw);
        AdHocSerialize(CPlusPlus);
        AdHocSerialize(Eval);
      }
    throw (Options::ErrorString() + "parsing problem");
  }

  ~Data()
  {
    switch (type_)
      {
/* STEP 6 : declare data associated case to correctly destroy union  */
        AdHocDestructor(Raw);
        AdHocDestructor(CPlusPlus);
        AdHocDestructor(Eval);
      }
  }

private:
  Type type_;
  DatasUnion data_;
};

std::ostream &operator<<(std::ostream &out, const Data &data);

// Clean
#undef AdHocEnum
#undef AdHocUnion
#undef AdHocConstructor
#undef AdHocSerialize
#undef AdHocDestructor


#endif /* _DATA_H_ */
