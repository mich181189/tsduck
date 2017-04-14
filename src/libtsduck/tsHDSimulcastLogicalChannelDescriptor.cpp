//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2017, Thierry Lelegard
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
//
//  Representation of a HD_simulcast_logical_channel_descriptor.
//  Private descriptor, must be preceeded by the EACEM/EICTA PDS.
//
//----------------------------------------------------------------------------

#include "tsHDSimulcastLogicalChannelDescriptor.h"



//----------------------------------------------------------------------------
// Default constructor:
//----------------------------------------------------------------------------

ts::HDSimulcastLogicalChannelDescriptor::HDSimulcastLogicalChannelDescriptor () :
    AbstractDescriptor (DID_HD_SIMULCAST_LCN),
    entries ()
{
    _is_valid = true;
}


//----------------------------------------------------------------------------
// Constructor from a binary descriptor
//----------------------------------------------------------------------------

ts::HDSimulcastLogicalChannelDescriptor::HDSimulcastLogicalChannelDescriptor (const Descriptor& desc) :
    AbstractDescriptor (DID_HD_SIMULCAST_LCN),
    entries ()
{
    deserialize (desc);
}


//----------------------------------------------------------------------------
// Constructor using a variable-length argument list.
// Each entry is described by 2 arguments: service_id and lcn.
// All services are marked as visible by default.
// All arguments are int, not uint16_t, since integer literals are int
// by default. The end of the argument list must be marked by -1.
//----------------------------------------------------------------------------

ts::HDSimulcastLogicalChannelDescriptor::HDSimulcastLogicalChannelDescriptor (int service_id, int lcn, ...) :
    AbstractDescriptor (DID_HD_SIMULCAST_LCN),
    entries ()
{
    _is_valid = true;
    entries.push_back (Entry (uint16_t (service_id), true, uint16_t (lcn)));

    va_list ap;
    va_start (ap, lcn);
    int id, n;
    while ((id = va_arg (ap, int)) >= 0 && (n = va_arg (ap, int)) >= 0) {
        entries.push_back (Entry (uint16_t (id), true, uint16_t (n)));
    }
    va_end (ap);
}


//----------------------------------------------------------------------------
// Serialization
//----------------------------------------------------------------------------

void ts::HDSimulcastLogicalChannelDescriptor::serialize (Descriptor& desc) const
{
    ByteBlockPtr bbp (new ByteBlock (2));
    CheckNonNull (bbp.pointer());

    for (EntryList::const_iterator it = entries.begin(); it != entries.end(); ++it) {
        bbp->appendUInt16 (it->service_id);
        bbp->appendUInt16 ((it->visible ? 0xFC00 : 0x7C00) | (it->lcn & 0x03FF));
    }

    (*bbp)[0] = _tag;
    (*bbp)[1] = uint8_t(bbp->size() - 2);
    Descriptor d (bbp, SHARE);
    desc = d;
}


//----------------------------------------------------------------------------
// Deserialization
//----------------------------------------------------------------------------

void ts::HDSimulcastLogicalChannelDescriptor::deserialize (const Descriptor& desc)
{
    _is_valid = desc.isValid() && desc.tag() == _tag && desc.payloadSize() % 4 == 0;
    entries.clear();

    if (_is_valid) {
        const uint8_t* data = desc.payload();
        size_t size = desc.payloadSize();
        while (size >= 4) {
            entries.push_back (Entry (GetUInt16 (data), (data[2] & 0x80) != 0, GetUInt16 (data + 2) & 0x03FF));
            data += 4;
            size -= 4;
        }
    }
}
