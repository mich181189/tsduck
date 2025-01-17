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
//!  Parameters and command line arguments for asynchronous log.
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsArgsSupplierInterface.h"

namespace ts {
    //!
    //! Parameters and command line arguments for asynchronous log.
    //! @ingroup cmd
    //!
    class TSDUCKDLL AsyncReportArgs : public ArgsSupplierInterface
    {
    public:
        // Public fields
        bool   sync_log;       //!< Synchronous log.
        bool   timed_log;      //!< Add time stamps in log messages.
        size_t log_msg_count;  //!< Maximum buffered log messages.

        //!
        //! Default maximum number of messages in the queue.
        //! Must be limited since the logging thread has a low priority.
        //! If a high priority thread loops on report, it would exhaust the memory.
        //!
        static const size_t MAX_LOG_MESSAGES = 512;

        //!
        //! Default constructor.
        //!
        AsyncReportArgs();

        // Implementation of ArgsSupplierInterface.
        virtual void defineArgs(Args& args) override;
        virtual bool loadArgs(DuckContext& duck, Args& args) override;
    };
}
