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


#include <iostream>
#include <rafale.h>
#include "rafale/plugin/paypal.hh"

std::string Paypal::url = "https://api-3t.paypal.com/nvp?";
std::string Paypal::version= "89.0";

void  Paypal::GetToken(
                        const float amount,
                        const std::string &desc,
                        const std::string &returnUrl,
                        const std::string &cancelUrl,
                        const std::string &mail,
                        const std::string &custom)
{
  url += "VERSION=" + Rafale::UriEncode(version);
  url += "&USER=" + Rafale::UriEncode(seller_);
  url += "&PWD=" + Rafale::UriEncode(pwd_);
  url += "&SIGNATURE=" + Rafale::UriEncode(signature_);
  url += "&METHOD=SetExpressCheckout";
  url += "&RETURNURL=" + Rafale::UriEncode(returnUrl);
  url += "&CANCELURL=" + Rafale::UriEncode(cancelUrl);
  url += "&AMT=" + Rafale::UriEncode(Rafale::ToString(amount));
  url += "&CURRENCYCODE=" + Rafale::UriEncode(currencycode_);
  url += "&DESC=" + Rafale::UriEncode(desc);
  url += "&LOCALECODE=" + Rafale::UriEncode(localecode_);
  url += "&HDRIMG=" + Rafale::UriEncode(hdrimg_);
  url += "&CUSTOM=" + Rafale::UriEncode(custom);
  url += "&EMAIL=" + Rafale::UriEncode(mail);

  std::string res;
  Curl::Session session(url,
                        res);
  Rafale::Server::SetVariables(res, data);
}

void  Paypal::GetExpressCheckoutDetails(const std::string &token)
{
  url += "VERSION=" + Rafale::UriEncode(version);
  url += "&USER=" + Rafale::UriEncode(seller_);
  url += "&PWD=" + Rafale::UriEncode(pwd_);
  url += "&SIGNATURE=" + Rafale::UriEncode(signature_);
  url += "&METHOD=GetExpressCheckoutDetails";
  url += "&TOKEN=" + Rafale::UriEncode(token);
  std::string res;
  Curl::Session session(url, res);
  Rafale::Server::SetVariables(res, data);
}

void  Paypal::DoExpressCheckoutPayment(const std::string &token,
                                       const std::string &payerId,
                                       const float amount)
{
  url += "VERSION=" + Rafale::UriEncode(version);
  url += "&USER=" + Rafale::UriEncode(seller_);
  url += "&PWD=" + Rafale::UriEncode(pwd_);
  url += "&SIGNATURE=" + Rafale::UriEncode(signature_);
  url += "&METHOD=DoExpressCheckoutPayment";
  url += "&TOKEN=" + Rafale::UriEncode(token);
  url += "&PAYERID=" + Rafale::UriEncode(payerId);
  url += "&CURRENCYCODE=" + Rafale::UriEncode(currencycode_);
  url += "&PAYMENTACTION=sale";
  url += "&AMT=" + Rafale::ToString(amount);
  std::string res;
  Curl::Session session(url, res);
  Rafale::Server::SetVariables(res, data);
}

