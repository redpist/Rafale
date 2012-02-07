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

#ifndef _RAFALE_CONTROLLER_GENERATOR_H_
#define _RAFALE_CONTROLLER_GENERATOR_H_

#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <map>

class ControllerGenerator
{
public:
  ControllerGenerator(std::map<std::string, std::list<std::string> > &controllers) : controllers_(controllers)
  {
  }

  void Init()
  {
    isInController_ = false;
    isInAction_ = false;
    isInView_ = false;
    line_ = 1;
    lockController_ = false;
    lockAction_ = false;
    lockView_ = false;
    newAction_ = false;
    newView_ = false;
  }

  void  Generate(const std::string &inputFileName, const std::string &outputFileName)
  {
    std::ifstream controllerFile(inputFileName);
    controllerFile_ = &controllerFile;

    std::ofstream outputFile(outputFileName);
    outputFile_ = &outputFile;

    if (!controllerFile_->is_open())
      throw ("no controller file");
    Init();
    while (Product())
      {
        if (Controller())
          {
            std::size_t end;
            if ((end = ControllerEnd()) != std::string::npos)
              {
                lockController_ = true;
                std::string tmp = Consume(end);
                // Register(\"" + actionName_ + "\", &_Controller_" + controllerName_ + "::" + actionName_ + ");\n"
                altBuffer_ += "_Controller_" + controllerName_ + "()\n{\n";
                for (auto action: controllers_[controllerName_])
                  {
                    altBuffer_ += "Register(\"" + action + "\", &_Controller_" + controllerName_ + "::" + action + ");\n";
                  }
                altBuffer_ += "}\n";
                altBuffer_ += Consume();
                Product(tmp);
                ParseController();
                lockController_ = false;
                isInController_ = false;
              }
            else
              {
                ParseController();
              }
          }
        Print(Consume());
      }
    Flush();
  }

  void  ParseController()
  {
    std::size_t offset;
    if ((offset = GetIdentifier(controllerName_)) != std::string::npos)
      {
        std::string     s0 = Consume(offset);
        PopIdentifier();
        s0 += "_Controller_" + controllerName_;
        Product(s0 + Consume());
      }
    // Print("C.");
    if (Action())
      {
        // Print("A.");
        if (newAction_)
          {
            std::string s0 = Consume(GetNextOpeningBracket() + 1) + "\n_View_"+ actionName_
              + " view;\n";
            std::string s1 = Consume();
            Product(s0 + s1);
            newAction_ = false;
            return ;
          }
        std::size_t end;
        if ((end = ActionEnd()) != std::string::npos)
          {
            lockAction_ = true;
            std::string tmp = Consume(end);
            altBuffer_ += Consume();
            Product(tmp);
            lockAction_ = false;
            isInAction_ = false;
          }
      }
    else if (View())
      {
        if (newView_)
          {
            std::string s0 = Consume(GetNextOpeningBracket() + 1) + "\n~_View_" + controllerName_ + "_" + viewName_ + "()\n{\nPrint();\n}\nvoid Print();\n";
            std::string s1 = Consume();
            Product(s0 + s1);
            newView_ = false;
            return ;
          }
        std::size_t end;
        if ((end = ViewEnd()) != std::string::npos)
          {
            lockView_ = true;
            std::string tmp = Consume(end);
            altBuffer_ += Consume();
            Product(tmp);
            lockView_ = false;
            isInView_ = false;
          }
      }
  }

  ~ControllerGenerator()
  {
  }
private:
  void  Replace()
  {
  }

  void  Flush()
  {
    (*outputFile_) << toFlush_ << std::flush;
    toFlush_.clear();
  }

  void  Print(const std::string &str)
  {
    toFlush_ += str;
  }

  void  Product(const std::string &string)
  {
    buffer_ += string;
  }

  std::size_t   Product()
  {
    if (!buffer_.size())
      return GetLine();
    return buffer_.size();
  }

  std::size_t  GetLine()
  {
    if (lockView_ || lockAction_ || lockController_)
      return 0;
    if (controllerFile_->eof())
      return 0;
    std::string buffer;
    getline(*controllerFile_, buffer);
    ++line_;
    buffer_ += altBuffer_;
    altBuffer_.clear();
    buffer_ += buffer;
    buffer_ += "\n";
    return buffer_.size();
  }

  std::string   Consume()
  {
    std::string buffer = buffer_;
    buffer_.clear();
    return buffer;
  }

  std::string   Consume(std::size_t length)
  {
    std::string buffer = buffer_.substr(0, length);
    buffer_.erase(0, length);
    return buffer;
  }

  std::size_t   ControllerEnd()
  {
    std::size_t i = 0;
    std::size_t found = std::string::npos;
    while (i < buffer_.size())
      {
        // std::cout << "[" << buffer_[i] << "]" << std::endl;
        if (buffer_[i] == '}')
          --controllerBrackets_;
        else if (buffer_[i] == '{')
          ++controllerBrackets_;
        if (found == std::string::npos && !controllerBrackets_)
            found = i;
        ++i;
    }
    // std::cout << "line :" + Line() + " controller bracket (" << controllerBrackets_ << ") : buffer [" << buffer_ << "]" << std::endl;
    return found;
  }

  std::size_t   ActionEnd()
  {
    std::size_t i = 0;
    std::size_t found = std::string::npos;
    while (i < buffer_.size())
      {
        // std::cout << "[" << buffer_[i] << "]" << std::endl;
        if (buffer_[i] == '}')
          --actionBrackets_;
        else if (buffer_[i] == '{')
            ++actionBrackets_;
        if ((found == std::string::npos) && !actionBrackets_)
            found = i;
        ++i;
      }
    return found + 1;
  }

  std::size_t   ViewEnd()
  {
    std::size_t i = 0;
    std::size_t found = std::string::npos;
    while (i < buffer_.size())
      {
        // std::cout << "[" << buffer_[i] << "]" << std::endl;
        if (buffer_[i] == '}')
          --viewBrackets_;
        else if (buffer_[i] == '{')
            ++viewBrackets_;
        if ((found == std::string::npos) && !viewBrackets_)
            found = i;
        ++i;
      }
    return found + 1;
  }


  bool  Controller()
  {
    if (isInController_)
      {
        return true;
      }
    else
      {
        std::size_t offset = GetIdentifier("controller");
        if (offset == std::string::npos)
          return false;

        std::string buffer = Consume();
        Print(buffer.substr(0, offset));
        buffer.erase(0, offset + sizeof("controller") - 1);
        Product(buffer);
        controllerName_ = PopIdentifier();
        // controllers_.push_back(controllerName_);
        std::size_t line = line_;
        std::size_t   nextOpeningBracket = GetNextOpeningBracket();
        if (nextOpeningBracket == std::string::npos)
          throw ("No Opening bracket after declaration of controller named \"" + controllerName_ + "\", line : " + "(TODO : PUT int to string call with line as parameter)" + ".");
        buffer = "class ";
        buffer += "_Controller_" + controllerName_ + " : public Rafale::Controller";
        Print(buffer);
        buffer = Consume(nextOpeningBracket);
        Print(buffer);
        controllerBrackets_ = 0;
        isInController_ = true;
        // std::cout << "Get controller [" << controllerName_ << "] at line : " << line << std::endl;
        return true;
      }
  }

  bool  Action()
  {
    if (isInAction_)
      {
        return true;
      }
    else
      {
        std::size_t offset = GetIdentifier("action");
        if (offset == std::string::npos)
          return false;

        std::string buffer = Consume();
        Print(buffer.substr(0, offset));
        buffer.erase(0, offset + sizeof("action") - 1);
        Product(buffer);
        actionName_ = PopIdentifier();
        controllers_[controllerName_].push_back(actionName_);
        std::string line = Line();
        std::size_t   nextOpeningBracket = GetNextOpeningBracket();
        if (nextOpeningBracket == std::string::npos)
          throw ("No Opening bracket after declaration of action named \"" + actionName_ + "\", line : " + line + ".");

        buffer = "void " + actionName_;
        Print(buffer);
        // buffer = Consume(nextOpeningBracket);
        actionBrackets_ = 1;
        ++controllerBrackets_;
        isInAction_ = true;
        // std::cout << "Get action [" << actionName_ << "] in controller [" << controllerName_<< "] at line : " << line << std::endl;
        newAction_ = true;
        return true;
      }
  }

  bool  View()
  {
    if (isInView_)
      {
        return true;
      }
    else
      {
        std::size_t offset = GetIdentifier("view");
        if (offset == std::string::npos)
          return false;

        std::string buffer = Consume();
        Print(buffer.substr(0, offset));
        buffer.erase(0, offset + sizeof("view") - 1);
        Product(buffer);
        viewName_ = PopIdentifier();
        std::string line = Line();
        std::size_t   nextOpeningBracket = GetNextOpeningBracket();
        if (nextOpeningBracket == std::string::npos)
          throw ("No Opening bracket after declaration of view named \"" + viewName_ + "\", line : " + line + ".");
        buffer = "struct _View_" + viewName_;
        Print(buffer);
        // buffer = Consume(nextOpeningBracket);
        viewBrackets_ = 1;
        ++controllerBrackets_;
        isInView_ = true;
        // std::cout << "Get view [" << viewName_ << "] in controller [" << controllerName_<< "] at line : " << line << std::endl;
        newView_ = true;
        return true;
      }
  }


  std::string   Line()
  {
    std::stringstream        line;
    line << line_;
    return line.str();
  }

  std::size_t   GetIdentifier(const std::string &identifier)
  {
    std::size_t offset;
    if ((offset = buffer_.find(identifier)) != std::string::npos)
      {
        if (CheckIdentifierSeparatorAfter(offset  + identifier.size() - 1)
            && CheckIdentifierSeparatorBefore(offset))
          return offset;
      }
    return std::string::npos;
  }

  std::string   GetNextIdentifier()
  {
    std::size_t startIdentifier = GetNextIdentifierStart();
    std::size_t stopIdentifier = GetIdentifierStop(startIdentifier) ;
    return buffer_.substr(startIdentifier, stopIdentifier);
  }

  std::string   PopIdentifier()
  {
    std::size_t startIdentifier = GetNextIdentifierStart();
    std::size_t stopIdentifier = GetIdentifierStop(startIdentifier) ;
    std::string buffer = buffer_.substr(startIdentifier, stopIdentifier);
    Consume(startIdentifier + stopIdentifier);
    return buffer;
  }

  std::size_t   GetNextIdentifierStart()
  {
    std::size_t i = 0;
    do
      {
        while (i < buffer_.size())
          {
            if (!IsSpace(buffer_[i]))
              return i;
            ++i;
          }
      }
    while (GetLine());
    return std::string::npos;
  }

  std::size_t   GetIdentifierStop(std::size_t offset)
  {
    std::size_t i = 0;
    do
      {
        while ((offset + i) < buffer_.size())
          {
            if (IsOperator(buffer_[offset + i]))
              return i;
            ++i;
          }
      }
    while (GetLine());
    return std::string::npos;
  }

  bool  CheckIdentifierSeparatorBefore(std::size_t offset)
  {
    if (offset)
      return IsOperator(buffer_[offset - 1]);
    else
      return true;
  }

  bool  CheckIdentifierSeparatorAfter(std::size_t offset)
  {
    if (buffer_.size() > offset + 1)
      return IsOperator(buffer_[offset + 1]);
    else
      return true;
  }

  std::size_t   GetNextOpeningBracket()
  {
    std::size_t i = 0;
    do
      {
        while (buffer_[i])
          {
            if (buffer_[i] == '{')
              return i;
            ++i;
          }
      }
    while (GetLine());
    return std::string::npos;
  }

  bool IsSpace(char c)
  {
    return ((c == ' ') ||
            (c == '\t') ||
            (c == '\n') ||
            (c == '\r'));
  }

  bool IsOperator(char c)
  {
    return ((c == ' ') ||
            (c == '\t') ||
            (c == '\n') ||
            (c == '\r') ||
            (c == ':') ||
            (c == ';') ||
            (c == '{') ||
            (c == '}') ||
            (c == '(') ||
            (c == ')') ||
            (c == '+') ||
            (c == '-') ||
            (c == '=') ||
            (c == '*') ||
            (c == '%') ||
            (c == ',') ||
            (c == '"') ||
            (c == '#'));
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
    return result;
  }

  std::string   buffer_;
  std::string   altBuffer_;
  std::string   toFlush_;
  std::string   controllerName_;
  std::string   actionName_;
  std::string   viewName_;
  int           controllerBrackets_;
  int           actionBrackets_;
  int           viewBrackets_;
  bool          isInString_;
  bool          isInAction_;
  bool          isInController_;
  bool          isInView_;

  bool          lockController_;
  bool          lockAction_;
  bool          lockView_;

  std::ifstream *controllerFile_;
  std::ofstream *outputFile_;
  std::size_t   line_;

  std::list<std::string>        actions_;

  std::map<std::string, std::list<std::string> > &controllers_;
  bool  newAction_;
  bool  newView_;
};

#endif /* _RAFALE_CONTROLLER_GENERATOR_H_ */
