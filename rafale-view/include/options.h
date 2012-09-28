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
