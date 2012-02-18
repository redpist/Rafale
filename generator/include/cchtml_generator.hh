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

#ifndef _CCHTML_GENERATOR_H_
#define _CCHTML_GENERATOR_H_

class CCHTMLGenerator
{
public:
  CCHTMLGenerator() { }
  ~CCHTMLGenerator() { }
protected:
  void  Generate(std::ifstream &cchtmlFile)
  {
    std::size_t offset;
    isInCPlusPlus_ = false;
    while (!cchtmlFile.eof())
      {
        getline(cchtmlFile, buffer_);
notInCplusplus:
        if (!isInCPlusPlus_)
          {
            if ((offset = buffer_.find("<c++>")) != std::string::npos)
              {
                PrintHtml(buffer_.substr(0, offset));
                isInCPlusPlus_ = true;
                buffer_ = buffer_.substr(offset + sizeof("<c++>") - 1, buffer_.size() - offset);
                goto inCplusplus;
              }
            else
              PrintHtml(buffer_);
          }
        else
          {
inCplusplus:
            if ((offset = buffer_.find("</c++>")) != std::string::npos)
              {
                PrintCPlusPlus(buffer_.substr(0, offset));
                isInCPlusPlus_ = false;
                buffer_ = buffer_.substr(offset + sizeof("</c++>") - 1, buffer_.size() - offset);
                goto notInCplusplus;
              }
            else
              {
                PrintCPlusPlus(buffer_);
              }
          }
      }
  }

  void  PrintHtml(const std::string& buffer)
  {
    (*outputFile_) << "std::cout << \"" << EscapeSpecialChar(buffer) << "\";\n";
  }

  void  PrintCPlusPlus(const std::string& buffer)
  {
    (*outputFile_) << buffer;
  }

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
          case '"':
            result += "\\\"";
            break;
          default:
            result += *it;
            break;
          }
      }
    return result + "\\n";
  }

  std::ofstream *outputFile_;
  std::string   buffer_;
  bool          isInCPlusPlus_;
};


#endif /* _CCHTML_GENERATOR_H_ */
