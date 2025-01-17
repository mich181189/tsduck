//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2022, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------
//!
//!  @file
//!  Command line arguments for the class WebRequest.
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsArgsSupplierInterface.h"
#include "tsUString.h"

namespace ts {
    //!
    //! Command line arguments for the class WebRequest.
    //! @ingroup net
    //!
    class TSDUCKDLL WebRequestArgs : public ArgsSupplierInterface
    {
    public:
        //!
        //! Constructor.
        //!
        WebRequestArgs();

        //!
        //! Destructor.
        //!
        virtual ~WebRequestArgs() override;

        // Public fields, by options.
        MilliSecond   connectionTimeout;    //!< -\-connection-timeout
        MilliSecond   receiveTimeout;       //!< -\-receive-timeout
        uint16_t      proxyPort;            //!< -\-proxy-port
        UString       proxyHost;            //!< -\-proxy-host
        UString       proxyUser;            //!< -\-proxy-user
        UString       proxyPassword;        //!< -\-proxy-password
        UString       userAgent;            //!< -\-user-agent
        bool          useCookies;           //!< Use cookies, no command line options, true by default
        UString       cookiesFile;          //!< Cookies files (Linux only), no command line options

        // Implementation of ArgsSupplierInterface.
        virtual void defineArgs(Args& args) override;
        virtual bool loadArgs(DuckContext& duck, Args& args) override;
    };
}
