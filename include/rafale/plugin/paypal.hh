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

#ifndef _RAFALE_PLUGIN_PAYPAL_H_
#define _RAFALE_PLUGIN_PAYPAL_H_

#include "rafale/curl.hh"

#include <string>
#include <map>

class Paypal
{
  std::string seller_;
  std::string pwd_;
  std::string signature_;
  std::string localecode_;
  std::string currencycode_;
public:
  std::string hdrimg_;
	Paypal() = delete;
	Paypal(
        const std::string &seller,
        const std::string &pwd,
        const std::string &signature,
        const std::string &localecode,
        const std::string &currencycode,
        const std::string &hdrimg = "") :
          seller_(seller),
          pwd_(pwd),
          signature_(signature),
          localecode_(localecode),
          currencycode_(currencycode),
          hdrimg_(hdrimg)
          {}
  void  GetToken(
                 const float amount,
                 const std::string &desc,
                 const std::string &returnUrl,
                 const std::string &cancelUrl,
                 const std::string &mail,
                 const std::string &custom = "");
  void  GetExpressCheckoutDetails(const std::string &token);
  void  DoExpressCheckoutPayment(const std::string &token,
                                 const std::string &payerId,
                                 const float amount);
  static std::string version;
  static std::string url;
  std::map<std::string, std::string> data;
};

#endif /* _RAFALE_PLUGIN_PAYPAL_H_ */
