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
//!  Representation of a mosaic_descriptor
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsAbstractDescriptor.h"

namespace ts {
    //!
    //! Representation of a mosaic_descriptor
    //! @see ETSI EN 300 468, 6.2.21.
    //! @ingroup descriptor
    //!
    class TSDUCKDLL MosaicDescriptor : public AbstractDescriptor
    {
    public:
        //!
        //! Cell entry.
        //!
        struct TSDUCKDLL Cell
        {
            Cell();                                    //!< Constructor.
            uint8_t   logical_cell_id;                 //!< 6 bits
            uint8_t   logical_cell_presentation_info;  //!< 3 bits
            ByteBlock elementary_cell_ids;             //!< 6 bits per value
            uint8_t   cell_linkage_info;               //!< Cell linkage info.
            uint16_t  bouquet_id;                      //!< When cell_linkage_info == 0x01
            uint16_t  original_network_id;             //!< When cell_linkage_info == 0x02, 0x03, 0x04
            uint16_t  transport_stream_id;             //!< When cell_linkage_info == 0x02, 0x03, 0x04
            uint16_t  service_id;                      //!< When cell_linkage_info == 0x02, 0x03, 0x04
            uint16_t  event_id;                        //!< When cell_linkage_info == 0x04
        };

        //!
        //! List of Cell entries.
        //!
        typedef std::list<Cell> CellList;

        // MosaicDescriptor public members:
        bool     mosaic_entry_point;                     //!< Top-level mosaic.
        uint8_t  number_of_horizontal_elementary_cells;  //!< 3 bits, warning: contains actual number minux 1.
        uint8_t  number_of_vertical_elementary_cells;    //!< 3 bits, warning: contains actual number minux 1.
        CellList cells;                                  //!< The list of cells.

        //!
        //! Default constructor.
        //!
        MosaicDescriptor();

        //!
        //! Constructor from a binary descriptor
        //! @param [in,out] duck TSDuck execution context.
        //! @param [in] bin A binary descriptor to deserialize.
        //!
        MosaicDescriptor(DuckContext& duck, const Descriptor& bin);

        // Inherited methods
        DeclareDisplayDescriptor();

    protected:
        // Inherited methods
        virtual void clearContent() override;
        virtual void serializePayload(PSIBuffer&) const override;
        virtual void deserializePayload(PSIBuffer&) override;
        virtual void buildXML(DuckContext&, xml::Element*) const override;
        virtual bool analyzeXML(DuckContext&, const xml::Element*) override;
    };
}
