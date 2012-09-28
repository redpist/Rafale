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

#include <rafale/curl.hh>
#include <iostream>

namespace Curl
{
  std::string Session::userAgent_ = "libcurl-agent/1.0";

  std::size_t Session::writeData(void *ptr, size_t size, size_t nmemb, void *res)
  {
    size_t realsize = size * nmemb;

    static_cast<std::string*>(res)->append(static_cast<char*>(ptr), realsize);
    return realsize;
  }

  Session::Session(const std::string &url,
                   std::string &res)
  {
    auto curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER , true);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST , true);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &Session::writeData);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &res);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, Session::userAgent_.c_str());
    curl_easy_perform(curl_handle);
    curl_easy_cleanup(curl_handle);
  }


}
