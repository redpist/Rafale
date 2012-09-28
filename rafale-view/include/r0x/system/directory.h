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

#ifndef _R0X_SYSTEM_DIRECTORY_H_
#define _R0X_SYSTEM_DIRECTORY_H_

#include <vector>
#include <dirent.h>
#include <string>

#include "r0x/system/file.h"

namespace R0x
{
  namespace System
  {
    template <typename StringType>
    class BasicDirectory
    {
    public:
      BasicDirectory(const StringType &path)
      {
        if (!(directory_ = opendir(path.c_str())))
          throw "error while opening directory : " + path;
      }

      // BasicDirectory(const BasicFile<StringType> &path) : path_(path)

      // BasicDirectory &operator+=(const StringType &filename, int mode = 644);
      // BasicDirectory &operator+=(File &file);

      ~BasicDirectory()
      {
        closedir(directory_);
      }

      typedef std::vector<BasicFile<StringType>> FileArray;

      const  FileArray &List()
      {
        struct dirent     *dirp;
        files_.clear();
        while ((dirp = readdir(directory_)))
          files_.push_back(File(dirp->d_name));
        seekdir(directory_, 0);
        return files_;
      }

    private:
      BasicDirectory();
      DIR               *directory_;
      FileArray         files_;
    };

    typedef BasicDirectory<std::string>      Directory;
  }
}

#endif /* _R0X_SYSTEM_DIRECTORY_H_ */
