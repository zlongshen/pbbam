// Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//  * Neither the name of Pacific Biosciences nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
// BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//
// Author: Derek Barnett

#ifndef PBIFILE_H
#define PBIFILE_H

#include "pbbam/Config.h"
#include <string>

namespace PacBio {
namespace BAM {

class BamFile;

// class-like namespace
namespace PbiFile
{

    /// PBI File Sections
    ///
    /// See (spec/doc links) for more details.
    ///
    enum Section
    {
         SUBREAD   = 0x0000  ///< SubreadData   (required)
       , MAPPED    = 0x0001  ///< MappedData    (always optional)
       , REFERENCE = 0x0002  ///< ReferenceData (always optional)
       , BARCODE   = 0x0004  ///< BarcodeData   (always optional)

       , ALL  = SUBREAD | MAPPED | REFERENCE | BARCODE  ///< synonym for building
    };
    typedef uint16_t Sections;

    /// PBI File Version
    enum VersionEnum
    {
        Version_3_0_0 = 0x030000

      , CurrentVersion = Version_3_0_0
    };

    /// Builds PBI index data from the supplied ".bam" file and writes a ".pbi" file.
    ///
    /// \param[in] bamFile The source BamFile.
    ///
    /// \throws std::exception if index file could not be created
    ///
    PBBAM_EXPORT void CreateFrom(const BamFile& bamFile);

} // namespace PbiFile
} // namespace BAM
} // namespace PacBio

#endif // PBIFILE_H
