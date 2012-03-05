#include "tools.hh"
#include <cstdint>
#include <sstream>
#include <string>
#include <fstream>

bool    Rafale::Exist(const std::string &fileName)
{
  std::ifstream f(fileName);
  return f.is_open();
}

std::string Rafale::Replace(const std::string &str, const std::string &token, const std::string &newToken)
{
  std::string   result;

  std::size_t   prevOffset = 0;
  std::size_t   offset = 0;
  while ((offset = str.find(token, offset)) != std::string::npos)
    {
      result.append(str, prevOffset, offset - prevOffset);
      result += newToken;
      offset += token.size();
      prevOffset = offset;
    }
  result.append(str, prevOffset, str.size() - prevOffset);
  return result;
}

std::string Rafale::DeleteEOS(const std::string &str)
{
  std::string newContent;
  for (char c: str)
    {
      if (c)
        newContent += c;
    }
  return newContent;
}

std::string   Rafale::ToLower(const std::string &value)
{
  for (char c: value)
    {
      if (c >= 'A' && c <= 'Z')
        c += ('a' - 'A');
    }
  return value;
}


// almost all the above code found at http://www.codeguru.com/cpp/cpp/string/conversions/article.php/c12759

const char HEX2DEC[256] =
  {
      /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
      /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

      /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

      /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

      /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
      /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
    };

std::string Rafale::UriDecode(const std::string & sSrc)
{
  // Note from RFC1630:  "Sequences which start with a percent sign
  // but are not followed by two hexadecimal characters (0-9, A-F) are reserved
  // for future extension"
  std::string   nNew;
  int i;
  std::size_t offset;
  for (i = 0; (offset = sSrc.find('+', i)) != std::string::npos;)
    {
      std::stringstream ss;
      ss << offset;
      nNew += sSrc.substr(i, offset - i) + ' ';
      i += (offset - i) + 1;
    }
  nNew += sSrc.substr(i, sSrc.size() - i);
  const unsigned char * pSrc = (const unsigned char *)nNew.c_str();
  const int SRC_LEN = nNew.length();
  const unsigned char * const SRC_END = pSrc + SRC_LEN;
  const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%'

  char * const pStart = new char[SRC_LEN];
  char * pEnd = pStart;

  while (pSrc < SRC_LAST_DEC)
    {
      if (*pSrc == '%')
        {
          char dec1, dec2;
          if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
              && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
            {
              *pEnd++ = (dec1 << 4) + dec2;
              pSrc += 3;
              continue;
            }
        }

      *pEnd++ = *pSrc++;
    }

  // the last 2- chars
  while (pSrc < SRC_END)
    *pEnd++ = *pSrc++;

  std::string sResult(pStart, pEnd);
  delete [] pStart;
  return sResult;
}

  // Only alphanum is safe.
const char SAFE[256] =
  {
    /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
    /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

    /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
    /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

    /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

    /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
  };

std::string Rafale::UriEncode(const std::string & sSrc)
{
  const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
  const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
  const int SRC_LEN = sSrc.length();
  unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
  unsigned char * pEnd = pStart;
  const unsigned char * const SRC_END = pSrc + SRC_LEN;

  for (; pSrc < SRC_END; ++pSrc)
    {
      if (SAFE[*pSrc])
        *pEnd++ = *pSrc;
      else
        {
          // escape this char
          *pEnd++ = '%';
          *pEnd++ = DEC2HEX[*pSrc >> 4];
          *pEnd++ = DEC2HEX[*pSrc & 0x0F];
        }
    }

  std::string sResult((char *)pStart, (char *)pEnd);
  delete [] pStart;

  std::string   nNew;
  int i;
  std::size_t offset;
  for (i = 0; (offset = sResult.find(' ', i)) != std::string::npos;)
    {
      std::stringstream ss;
      ss << offset;
      nNew += sResult.substr(i, offset - i) + '+';
      i += (offset - i) + 1;
    }
  nNew += sResult.substr(i, sResult.size() - i);
  return nNew;
}
