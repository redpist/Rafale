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

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdint>

class Options
{
public:
  Options(int argc, const char *argv[]) : verbose_(false)
  {
    for (int i = 0; i < argc; ++i)
      argv_.push_back(argv[i]);
  }

  ~Options() { }

  static const std::string  &Usage()
  {
    return Options::usage_;
  }

  static const std::string  &ErrorString()
  {
    return errorString_;
  }

  void  Parse()
  {
    FindHelp();
    FindVersion();
    if (argv_.size() < 4)
      ParsingError();
    FindOutputFile();
    FindVerbose();
    FindInputFile();
  }


  const std::string     &InputFile() const
  {
    return inputFile_;
  }

  const std::string     &OutputFile() const
  {
    return outputFile_;
  }

  bool                  IsVerbose() const
  {
    return verbose_;
  }

private:

  void  FindHelp()
  {
    if ((find(begin(argv_), end(argv_), "--help") != end(argv_)) || (find(begin(argv_), end(argv_), "-h") != end(argv_)))
      ParsingError();
  }

  void  FindVersion()
  {
    if (find(begin(argv_), end(argv_), "--version") != end(argv_))
      throw("rafale++ 0.3\n"
            "Copyright (C) 2012 Jeremy Lecerf\n"
            "This is free software; see the source for copying conditions.  There is NO\n"
            "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
  }

  void  FindVerbose()
  {
    auto verboseIt = find(begin(argv_), end(argv_), "-v");
    if (verboseIt == end(argv_))
      {
        verboseIt = find(begin(argv_), end(argv_), "--verbose");
        if (verboseIt == end(argv_))
          return ;
      }
    verbose_ = true;
    argv_.erase(verboseIt);
  }

  void  FindOutputFile()
  {
    auto outputIt = find(begin(argv_), end(argv_), "-o");
    if (outputIt == end(argv_))
      {
        outputIt = find(begin(argv_), end(argv_), "--output");
        if (outputIt == end(argv_))
          throw (ErrorString() + "you have to specify a output file\n" + Usage());
      }
    if (++outputIt == end(argv_))
      throw (ErrorString() + "no output file specify after \"-o\"\n" + Usage());
    outputFile_ = *outputIt;
    argv_.erase(outputIt);
  }

  void  FindInputFile()
  {
    for (int i = 1; static_cast<std::size_t>(i) < argv_.size(); ++i)
      {
        if (find(begin(alreadyParsed_), end(alreadyParsed_), i) != end(alreadyParsed_))
          continue;
        if (argv_[i][0] != '-')
          inputFile_ = argv_[i];
      }
  }

  void  ParsingError()
  {
    throw (Usage());
  }

  static        std::string     usage_;
  static        std::string     errorString_;
  std::vector<std::string>      argv_;
  bool                          verbose_;
  std::string                   inputFile_;
  std::string                   outputFile_;
  std::list<int>                alreadyParsed_;
};

#endif /* _OPTIONS_H_ */
