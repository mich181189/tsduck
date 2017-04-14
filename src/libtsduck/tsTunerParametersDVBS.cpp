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
//  DVB-S / DVB-S2 (satellite) tuners parameters
//
//----------------------------------------------------------------------------

#include "tsTunerParametersDVBS.h"
#include "tsDektec.h"
#include "tsEnumeration.h"
#include "tsDecimal.h"
#include "tsFormat.h"

#if defined (TS_NEED_STATIC_CONST_DEFINITIONS)
const ts::Polarization ts::TunerParametersDVBS::DEFAULT_POLARITY;
const ts::SpectralInversion ts::TunerParametersDVBS::DEFAULT_INVERSION;
const uint32_t ts::TunerParametersDVBS::DEFAULT_SYMBOL_RATE;
const ts::InnerFEC ts::TunerParametersDVBS::DEFAULT_INNER_FEC;
const size_t ts::TunerParametersDVBS::DEFAULT_SATELLITE_NUMBER;
const ts::DeliverySystem ts::TunerParametersDVBS::DEFAULT_DELIVERY_SYSTEM;
const ts::Modulation ts::TunerParametersDVBS::DEFAULT_MODULATION;
const ts::Pilot ts::TunerParametersDVBS::DEFAULT_PILOTS;
const ts::RollOff ts::TunerParametersDVBS::DEFAULT_ROLL_OFF;
#endif


//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

ts::TunerParametersDVBS::TunerParametersDVBS() :
    TunerParameters (DVB_S),
    frequency (0),
    polarity (DEFAULT_POLARITY),
    lnb (LNB::Universal),
    inversion (DEFAULT_INVERSION),
    symbol_rate (DEFAULT_SYMBOL_RATE),
    inner_fec (DEFAULT_INNER_FEC),
    satellite_number (DEFAULT_SATELLITE_NUMBER),
    delivery_system (DEFAULT_DELIVERY_SYSTEM),
    modulation (DEFAULT_MODULATION),
    pilots (DEFAULT_PILOTS),
    roll_off (DEFAULT_ROLL_OFF)
{
}


//----------------------------------------------------------------------------
// Virtual assignment
//----------------------------------------------------------------------------

void ts::TunerParametersDVBS::copy (const TunerParameters& obj) throw (IncompatibleTunerParametersError)
{
    const TunerParametersDVBS* other = dynamic_cast <const TunerParametersDVBS*> (&obj);
    if (other == 0) {
        throw IncompatibleTunerParametersError ("DVBS != " + TunerTypeEnum.name (obj.tunerType()));
    }
    else {
        this->frequency = other->frequency;
        this->polarity = other->polarity;
        this->lnb = other->lnb;
        this->inversion = other->inversion;
        this->symbol_rate = other->symbol_rate;
        this->inner_fec = other->inner_fec;
        this->satellite_number = other->satellite_number;
        this->delivery_system = other->delivery_system;
        this->modulation = other->modulation;
        this->pilots = other->pilots;
        this->roll_off = other->roll_off;
    }
}


//----------------------------------------------------------------------------
// Polarity as encoded in zap format
//----------------------------------------------------------------------------

namespace {
    const ts::Enumeration ZapPolarizationEnum
        ("h", ts::POL_HORIZONTAL,
         "v", ts::POL_VERTICAL,
         "l", ts::POL_LEFT,
         "r", ts::POL_RIGHT,
         TS_NULL);
}


//----------------------------------------------------------------------------
// Format the tuner parameters according to the Linux DVB "zap" format
// Expected format: "freq:pol:satnum:symrate"
//    With freq = frequency in MHz, pol = polarity (either v or h), satnum =
//    satellite number, symrate = symbol rate in ksym/s.
//----------------------------------------------------------------------------

std::string ts::TunerParametersDVBS::toZapFormat() const
{
    const std::string pol (ZapPolarizationEnum.name (polarity));
    return Format ("%" FMT_INT64 "u:%s:%" FMT_SIZE_T "u:%d",
                   frequency / 1000000,
                   pol.c_str(),
                   satellite_number,
                   int (symbol_rate / 1000));
}


//----------------------------------------------------------------------------
// Decode a Linux DVB "zap" specification and set the corresponding values
//----------------------------------------------------------------------------

bool ts::TunerParametersDVBS::fromZapFormat (const std::string& zap)
{
    StringVector values;
    SplitString (values, zap, ':', true);

    uint64_t freq;
    int pol;
    size_t sat;
    uint32_t symrate;

    if (values.size() != 4 ||
        !ToInteger (freq, values[0]) ||
        ((pol = ZapPolarizationEnum.value (values[1])) != POL_HORIZONTAL && pol != POL_VERTICAL) ||
        !ToInteger (sat, values[2]) ||
        !ToInteger (symrate, values[3])) {
        return false;
    }

    frequency = freq * 1000000;
    polarity = Polarization (pol);
    satellite_number = sat;
    symbol_rate = symrate * 1000;
    lnb = LNB::Universal;
    inversion = DEFAULT_INVERSION;
    inner_fec = DEFAULT_INNER_FEC;

    // DVB-S2 specific values go to default

    delivery_system = DEFAULT_DELIVERY_SYSTEM;
    modulation = DEFAULT_MODULATION;
    pilots = DEFAULT_PILOTS;
    roll_off = DEFAULT_ROLL_OFF;

    return true;
}


//----------------------------------------------------------------------------
// Format the tuner parameters as a list of options for the dvb tsp plugin.
//----------------------------------------------------------------------------

std::string ts::TunerParametersDVBS::toPluginOptions (bool no_local) const
{
    std::string local_options;
    if (!no_local) {
        local_options = " --lnb " + std::string (lnb) +
            Format (" --satellite-number %" FMT_SIZE_T "d", satellite_number);
    }

    return Format ("--frequency %" FMT_INT64 "u --symbol-rate %d", frequency, int (symbol_rate)) +
        " --fec-inner " + InnerFECEnum.name (inner_fec) +
        " --spectral-inversion " + SpectralInversionEnum.name (inversion) +
        " --polarity " + PolarizationEnum.name (polarity) +
        " --delivery-system " + DeliverySystemEnum.name (delivery_system) +
        " --modulation " + ModulationEnum.name (modulation) +
        " --pilots " + PilotEnum.name (pilots) +
        " --roll-off " + RollOffEnum.name (roll_off) +
        local_options;
}


//----------------------------------------------------------------------------
// Extract options from a TunerArgs, applying defaults when necessary.
//----------------------------------------------------------------------------

bool ts::TunerParametersDVBS::fromArgs (const TunerArgs& tuner, ReportInterface& report)
{
    if (!tuner.frequency.set()) {
        report.error ("no frequency specified, use option --frequency");
        return false;
    }

    frequency = tuner.frequency.value();
    symbol_rate = tuner.symbol_rate.set() ? tuner.symbol_rate.value() : DEFAULT_SYMBOL_RATE;
    inner_fec = tuner.inner_fec.set() ? tuner.inner_fec.value() : DEFAULT_INNER_FEC;
    inversion = tuner.inversion.set() ? tuner.inversion.value() : DEFAULT_INVERSION;
    polarity = tuner.polarity.set() ? tuner.polarity.value() : DEFAULT_POLARITY;
    satellite_number = tuner.satellite_number.set() ? tuner.satellite_number.value() : DEFAULT_SATELLITE_NUMBER;
    lnb = tuner.lnb.set() ? tuner.lnb.value() : LNB::Universal;
    delivery_system = tuner.delivery_system.set() ? tuner.delivery_system.value() : DEFAULT_DELIVERY_SYSTEM;
    modulation = tuner.modulation.set() ? tuner.modulation.value() : DEFAULT_MODULATION;
    pilots = tuner.pilots.set() ? tuner.pilots.value() : DEFAULT_PILOTS;
    roll_off = tuner.roll_off.set() ? tuner.roll_off.value() : DEFAULT_ROLL_OFF;

    return true;
}


//----------------------------------------------------------------------------
// This abstract method computes the theoretical useful bitrate of a
// transponder, based on 188-bytes packets, in bits/second.
// If the characteristics of the transponder are not sufficient
// to compute the bitrate, return 0.
//----------------------------------------------------------------------------

ts::BitRate ts::TunerParametersDVBS::theoreticalBitrate() const
{
    // Let the Dektec API compute the TS rate if we have a Dektec library.
#if !defined(TS_NO_DTAPI)
    int bitrate, modulation_type, param0, param1, param2;
    if (convertToDektecModulation(modulation_type, param0, param1, param2) &&
        Dtapi::DtapiModPars2TsRate(bitrate, modulation_type, param0, param1, param2, int(symbol_rate)) == DTAPI_OK) {
        // Successfully found Dektec modulation parameters and computed TS bitrate
        return BitRate(bitrate);
    }
#endif

    // Otherwise, don't know how to compute DVB-S2 bitrate...
    return delivery_system == DS_DVB_S ? TheoreticalBitrateForModulation(modulation, inner_fec, symbol_rate) : 0;
}


//----------------------------------------------------------------------------
// Attempt to convert the tuning parameters in modulation parameters
// Dektec modulator cards. This is an optional method. Return true
// on success, false on error (includes unsupported operation).
//----------------------------------------------------------------------------

bool ts::TunerParametersDVBS::convertToDektecModulation (int& modulation_type, int& param0, int& param1, int& param2) const
{
#if defined(TS_NO_DTAPI)

    // No Dektec library.
    return false;

#else

    // Determine modulation type
    if (delivery_system == DS_DVB_S) {
        modulation_type = DTAPI_MOD_DVBS_QPSK;
    }
    else if (delivery_system == DS_DVB_S2 && modulation == QPSK) {
        modulation_type = DTAPI_MOD_DVBS2_QPSK;
    }
    else if (delivery_system == DS_DVB_S2 && modulation == PSK_8) {
        modulation_type = DTAPI_MOD_DVBS2_8PSK;
    }
    else {
        return false; // unsupported
    }

    // Determine convolution code rate
    switch (inner_fec) {
        case FEC_1_2:  param0 = DTAPI_MOD_1_2; break;
        case FEC_1_3:  param0 = DTAPI_MOD_1_3; break;
        case FEC_1_4:  param0 = DTAPI_MOD_1_4; break;
        case FEC_2_3:  param0 = DTAPI_MOD_2_3; break;
        case FEC_2_5:  param0 = DTAPI_MOD_2_5; break;
        case FEC_3_4:  param0 = DTAPI_MOD_3_4; break;
        case FEC_3_5:  param0 = DTAPI_MOD_3_5; break;
        case FEC_4_5:  param0 = DTAPI_MOD_4_5; break;
        case FEC_5_6:  param0 = DTAPI_MOD_5_6; break;
        case FEC_6_7:  param0 = DTAPI_MOD_6_7; break;
        case FEC_7_8:  param0 = DTAPI_MOD_7_8; break;
        case FEC_8_9:  param0 = DTAPI_MOD_8_9; break;
        case FEC_9_10: param0 = DTAPI_MOD_9_10; break;
        default: return false; // unsupported
    }

    // Additional parameters param1 and param2
    switch (delivery_system) {
        case DS_DVB_S: {
            param1 = param2 = 0;
            break;
        }
        case DS_DVB_S2: {
            switch (pilots) {
                case PILOT_ON:  param1 = DTAPI_MOD_S2_PILOTS; break;
                case PILOT_OFF: param1 = DTAPI_MOD_S2_NOPILOTS; break;
                default: return false; // unsupported
            }
            // Assume long FEC frame for broadcast service (should be updated by caller if necessary).
            param1 |= DTAPI_MOD_S2_LONGFRM;
            // No physical layer scrambling initialization sequence
            param2 = 0;
            break;
        }
        default: {
            return false; // unsupported
        }
    }

    return true;

#endif // TS_NO_DTAPI
}
