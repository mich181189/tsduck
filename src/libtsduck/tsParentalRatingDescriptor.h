//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2019, Thierry Lelegard
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
//!  Representation of an parental_rating_descriptor
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsAbstractDescriptor.h"

namespace ts {
    //!
    //! Representation of an parental_rating_descriptor
    //! @see ETSI 300 468, 6.2.28.
    //! @ingroup descriptor
    //!
    class TSDUCKDLL ParentalRatingDescriptor : public AbstractDescriptor
    {
    public:
        //!
        //! Item entry.
        //!
        struct TSDUCKDLL Entry
        {
            // Public members
            UString country_code;  //!< ISO-3166 country code, 3 characters.
            uint8_t rating;        //!< Parental rating.

            //!
            //! Constructor.
            //! @param [in] code ISO-3166 country code, 3 characters, as a C-string. Can be null.
            //! @param [in] rate Parental rating.
            //!
            Entry(const UChar* code = nullptr, uint8_t rate = 0);

            //!
            //! Constructor.
            //! @param [in] code ISO-3166 country code, 3 characters.
            //! @param [in] rate Parental rating.
            //!
            Entry(const UString& code, uint8_t rate);
        };

        //!
        //! A list of item entries.
        //!
        typedef std::list<Entry> EntryList;

        //!
        //! Maximum number of services entries to fit in 255 bytes.
        //!
        static const size_t MAX_ENTRIES = 63;

        // Public members
        EntryList entries;  //!< The list of item entries.

        //!
        //! Default constructor.
        //!
        ParentalRatingDescriptor();

        //!
        //! Constructor from a binary descriptor
        //! @param [in] bin A binary descriptor to deserialize.
        //! @param [in] charset If not zero, character set to use without explicit table code.
        //!
        ParentalRatingDescriptor(DuckContext& duck, const Descriptor& bin);

        //!
        //! Constructor with one entry.
        //! @param [in] code ISO-3166 country code, 3 characters.
        //! @param [in] rate Parental rating.
        //!
        ParentalRatingDescriptor(const UString& code, uint8_t rate);

        // Inherited methods
        virtual void serialize(DuckContext&, Descriptor&) const override;
        virtual void deserialize(DuckContext&, const Descriptor&) override;
        virtual void buildXML(DuckContext&, xml::Element*) const override;
        virtual void fromXML(DuckContext&, const xml::Element*) override;
        DeclareDisplayDescriptor();
    };
}
