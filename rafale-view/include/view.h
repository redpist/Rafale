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

#ifndef _VIEW_H_
#define _VIEW_H_

#include <fstream>

#include <r0x/system/directory.h>
#include "data.h"

namespace Rafale
{
  class View
  {
  public:
    View(const std::string &inputFileName) :
      status_(Data::Type::TypeValue(Raw)), inString_(false)
    {
      Parse(inputFileName);
    }

    void  Export(const std::string &outputFileName)
    {
      std::ofstream outputFile(outputFileName);
      if (!outputFile.is_open())
        throw (Options::ErrorString() + "problem while opening : " + outputFileName);
      // outputFile << "#include \"" << controllerFile << "\"\n";
      // outputFile <<  "void " << controller + "::Print()\n"
      //   "{\n"
      //   "(void)(static_cast<Rafale::Controller*>(this));\n";


      outputFile <<  "void Print()\n"
        "{\n"
        "(void)(static_cast<Rafale::BasicController*>(this));\n";
      for (auto &data : datas_)
        outputFile << data;
      outputFile << "}\n";
    }


    void Parse(const std::string &inputFileName)
    {
      std::ifstream inputFile(inputFileName);
      if (!inputFile.is_open())
        throw (Options::ErrorString() + "problem while opening : " + inputFileName);
      while (!inputFile.eof())
        {
          getline(inputFile, input_);
          input_ += '\n';
        consume:
          switch (status_)
            {
            case Data::Type::TypeValue(Raw):
              if ((offset_ = input_.find(Data::CPlusPlus::openingToken)) != std::string::npos)
                {
                  output_ += input_.substr(0, offset_);
                  input_ = input_.substr(offset_ + Data::CPlusPlus::openingToken.size(), input_.size() - offset_);
                  datas_.push_back(Data::Raw(output_));
                  output_.clear();
                  status_= Data::Type::TypeValue(CPlusPlus);
                  goto consume;
                }
              else if ((offset_ = input_.find(Data::Eval::openingToken)) != std::string::npos)
                {
                  output_ += input_.substr(0, offset_);
                  input_ = input_.substr(offset_ + Data::Eval::openingToken.size(), input_.size() - offset_);
                  datas_.push_back(Data::Raw(output_));
                  output_.clear();
                  status_= Data::Type::TypeValue(Eval);
                  goto consume;
                }
              else
                {
                  output_ += input_;
                  input_.clear();
                }
            break;
            case Data::Type::TypeValue(CPlusPlus):
              if ((offset_ = FindClosingToken(Data::CPlusPlus::closingToken)) != std::string::npos)
              {
                output_ += input_.substr(0, offset_);
                input_ = input_.substr(offset_ + Data::CPlusPlus::closingToken.size(), input_.size() - offset_);
                datas_.push_back(Data::CPlusPlus(output_));
                output_.clear();
                status_= Data::Type::TypeValue(Raw);
                inString_ = false;
                goto consume;
              }
            else
              {
                output_ += input_;
                input_.clear();
              }
              break;
            case Data::Type::TypeValue(Eval):
              if ((offset_ = FindClosingToken(Data::Eval::closingToken)) != std::string::npos)
              {
                output_ += input_.substr(0, offset_);
                input_ = input_.substr(offset_ + Data::Eval::closingToken.size(), input_.size() - offset_);
                datas_.push_back(Data::Eval(output_));
                output_.clear();
                status_= Data::Type::TypeValue(Raw);
                inString_ = false;
                goto consume;
              }
            else
              {
                  output_ += input_;
                  input_.clear();
              }
              break;
            }
        }
      switch (status_)
        {
        case Data::Type::TypeValue(Raw):
          datas_.push_back(Data::Raw(output_));
          output_.clear();
          break;
        case Data::Type::TypeValue(CPlusPlus):
          datas_.push_back(Data::CPlusPlus(output_));
          output_.clear();
          break;
        case Data::Type::TypeValue(Eval):
          datas_.push_back(Data::Eval(output_));
          output_.clear();
          break;
        }
      status_ = Data::Type::TypeValue(Raw);
    }

    // Lil' Joke
    // - Did you hear that a baby was fed on elephant's milk and gained twenty pounds in a week.
    // - That's impossible. Whose baby?
    // - An elephant's.

    std::size_t FindClosingToken(const std::string &token)
    {
      std::size_t       offset   = 0;

      for (const char &c: input_)
        {
          if (c == '"')
            {
              int nbBackslash = 0;
              for (int i = offset; i && (input_[--i] == '\\'); ++nbBackslash);
              if (nbBackslash % 2 == 0)
                inString_ = !inString_;
            }
          else if (!inString_
                   && ((input_.size() - offset - token.size()) > 0)
                   && (input_.substr(offset, token.size()) == token)) // token
            {
              return offset;
            }
          ++offset;
        }
      return std::string::npos;
    }

    ~View() { }
  private:
    std::list<Data> datas_;
    Data::Type  status_;
    std::size_t  offset_;
    std::string  input_;
    std::string  output_;
    bool         inString_;
  };
}

#endif /* _VIEW_H_ */
