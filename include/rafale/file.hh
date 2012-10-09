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

#ifndef _RAFALE_FILE_HH_
#define _RAFALE_FILE_HH_

#include "rafale/tools.hh"
#include "rafale/extern.hh"
#include <cstdlib>
#include <fstream>
#include <string>

namespace Rafale
{
  class File
  {
  public:
    File() : empty_(true)
    {
    }

    File(const std::string &name, const std::string &contentType, const std::string &content) : contentType_(contentType), empty_(false)
    {
      name_ = Rafale::Replace(name, "\\", "");
      name_ = Rafale::Replace(name_, "/", "_"); // MAGIC PATCHs , GO CLEAN
      tmpName_ = "_file_" + Rafale::ToString(rand());
      std::ofstream tmpFile(Rafale::filesDirectory + "/" + tmpName_);
      if (tmpFile.is_open())
        tmpFile.write(content.c_str(), content.size()); // TO DO : VERIF
      else
        {
          name_ = "";
          empty_ = true;
        }
    }

    void        Copy(const std::string &destination)
    {
      if (!empty_)
        {
          std::ifstream tmpFile(Rafale::filesDirectory + "/" + tmpName_);
          std::ofstream destinationFile(destination);

          char      buff[4096];
          while (!tmpFile.eof())
            {
              tmpFile.read(buff, sizeof(buff));
              std::size_t readed = tmpFile.gcount();
              destinationFile.write(buff, readed);
            }
        }
    }

    bool        Empty()
    {
      return empty_;
    }

    const std::string   &Name()
    {
      return name_;
    }

    std::string Extension()
    {
      if ((name_.rfind(".") != std::string::npos) && name_.rfind(".") &&
          ((name_.rfind(".") != (name_.size() - 1))))
        return name_.substr(name_.rfind(".") + 1);
      else
        return "";
    }


    ~File()
    {
      if (!empty_)
        remove(std::string(Rafale::filesDirectory + "/" + tmpName_).c_str());
    }
  private:
    std::string         tmpName_;
    std::string         name_;
    std::string         contentType_;
    bool                empty_;
  };

}

#endif /* _RAFALE_FILE_HH_ */
