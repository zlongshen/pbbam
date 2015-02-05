// Copyright (c) 2014, Pacific Biosciences of California, Inc.
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

// Author: Derek Barnett

#include <gtest/gtest.h>
#include <htslib/sam.h>
#include <pbbam/BamReader.h>
#include <pbbam/BamWriter.h>
#include <pbbam/SamHeader.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace PacBio;
using namespace PacBio::BAM;
using namespace std;

// put any BamWriter-only API tests here (error handling, etc.)
//
// plain ol' read & dump is in test_EndToEnd.cpp

struct ResultPacket
{
    std::string name;
    char*       bases;
    char*       overallQv;
    size_t      length;
    int         zmwNum;
    int         startPos;
    BamRecord   bamRecord;

    ResultPacket() = default;

    ResultPacket(ResultPacket&& src)
    {
        name            = std::move(src.name);

        bases           = src.bases;
        overallQv       = src.overallQv;
        length          = src.length;

        zmwNum          = src.zmwNum;
        startPos        = src.startPos;

        src.bases     = 0;
        src.overallQv = 0;

        bamRecord = std::move(src.bamRecord);
    }
    // Copy constructor
    ResultPacket(const ResultPacket& src) = delete;
    // Move assignment constructor
    ResultPacket& operator=(ResultPacket&& rhs) = delete;
    // Copy assignment constructor
    ResultPacket& operator=(const ResultPacket& rhs) = delete;
    // Destructor
    ~ResultPacket()
    {
        // delete [] basesBam;
        if (bases != 0) delete [] bases;
        if (overallQv != 0) delete [] overallQv;
    }
};

TEST(BamWriterTest, SingleWrite_UserRecord)
{
    //Writing a ResultPacket in Workflow.h:
    ResultPacket result;
    result.zmwNum = 42;
    result.name = "ZMW\\"+std::to_string(42);
    auto length = 5;

    result.bases     = (char*) calloc(length,1);
    result.overallQv = (char*) calloc(length,1);
    // FILL WITH CONTENT
    result.bases[0] = 'A';
    result.bases[1] = 'C';
    result.bases[2] = 'G';
    result.bases[3] = 'T';
    result.bases[4] = 'C';
    result.overallQv[0] = ']';
    result.overallQv[1] = '6';
    result.overallQv[2] = '4';
    result.overallQv[3] = '@';
    result.overallQv[4] = '<';

    // Encode data to BamAlignment
    result.bamRecord.Name(result.name);
    result.bamRecord.SetSequenceAndQualities(result.bases, length);
    result.bamRecord.CigarData("");
    result.bamRecord.Bin(0);
    result.bamRecord.Flag(0);
    result.bamRecord.InsertSize(0);
    result.bamRecord.MapQuality(0);
    result.bamRecord.MatePosition(-1);
    result.bamRecord.MateReferenceId(-1);
    result.bamRecord.Position(-1);
    result.bamRecord.ReferenceId(-1);

    std::vector<uint8_t> subQv = std::vector<uint8_t>({34, 5, 125});
//    std::vector<uint16_t> subQv = std::vector<uint16_t>({34, 5, 125});

    TagCollection tags;
    tags["SQ"] = subQv;
    result.bamRecord.Tags(tags);

    BamWriter bamSubreads_;
    SamHeader headerSubreads;
    headerSubreads.version = "1.1";
    headerSubreads.sortOrder = "coordinate";
    bamSubreads_.Open("42.subreads.bam", headerSubreads);
    EXPECT_TRUE(bamSubreads_.Write(result.bamRecord));
    bamSubreads_.Close();

    BamReader reader;
    EXPECT_TRUE(reader.Open("42.subreads.bam"));
    EXPECT_EQ(std::string("1.1"), reader.Header().version);
    EXPECT_EQ(std::string("coordinate"), reader.Header().sortOrder);

    BamRecord inputRecord;
    EXPECT_TRUE(reader.GetNext(&inputRecord));
    EXPECT_EQ(std::string("ACGTC"),   inputRecord.Sequence());
    EXPECT_EQ(std::string("ZMW\\42"), inputRecord.Name());
    EXPECT_EQ(std::vector<uint8_t>({34, 5, 125}), inputRecord.Tags().at("SQ").ToUInt8Array());
//    EXPECT_EQ(std::vector<uint16_t>({34, 5, 125}), inputRecord.Tags().at("SQ").ToUInt16Array());

    reader.Close();
    remove("42.subreads.bam");
}

