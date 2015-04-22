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

// Author: Derek Barnett

#ifndef CIGAROPERATION_H
#define CIGAROPERATION_H

#include "pbbam/Config.h"

namespace PacBio {
namespace BAM {

/// Describes a CIGAR operation. Bracketed character is the corresponding SAM/BAM character code.
enum class CigarOperationType
{
    UNKNOWN_OP        = -1 ///< unknown/invalid CIGAR operator
  , ALIGNMENT_MATCH   = 0  ///< alignment match (can be a sequence match or mismatch) [M]
  , INSERTION              ///< insertion to the reference [I]
  , DELETION               ///< deletion from the reference [D]
  , REFERENCE_SKIP         ///< skipped region from the reference [N]
  , SOFT_CLIP              ///< soft clipping (clipped sequences present in SEQ) [S]
  , HARD_CLIP         = 5  ///< hard clipping (clipped sequences NOT present in SEQ) [H]
  , PADDING                ///< padding (silent deletion from padded reference) [P]
  , SEQUENCE_MATCH         ///< sequence match [=]
  , SEQUENCE_MISMATCH      ///< sequence mismatch [X]

    // TODO: looks like there is a new 'B' type in htslib soure...
    //       no reference in htslib docs though yet as to what it means
};

class PBBAM_EXPORT CigarOperation
{
public:

    /// \name Operation Type Conversion Methods
    /// \{

    /// Convert between CigarOperationType enum & SAM/BAM character code.
    ///
    /// \param[in] type CigarOperationType value
    /// \returns SAM/BAM character code
    static char TypeToChar(const CigarOperationType type);

    /// Convert between CigarOperationType enum & SAM/BAM character code.
    ///
    /// \param[in] c SAM/BAM character code
    /// \returns CigarOperationType value
    static CigarOperationType CharToType(const char c);

    /// \}

public:
    /// \name Constructors & Related Methods
    /// \{

    CigarOperation(void);
    CigarOperation(char c, uint32_t length);
    CigarOperation(CigarOperationType op, uint32_t length);
    CigarOperation(const CigarOperation& other);
    CigarOperation(CigarOperation&& other) = default;
    CigarOperation& operator=(const CigarOperation& other) = default;
    CigarOperation& operator=(CigarOperation&& other) = default;
    ~CigarOperation(void);

    /// \}

public:

    /// \returns operation type as SAM/BAM char code
    inline char Char(void) const;

    /// \returns operation length
    inline uint32_t Length(void) const;

    /// \returns operation type as CigarOperationType enum value
    inline CigarOperationType Type(void) const;

    /// \}

public:
    /// \name Attributes
    /// \{

    /// Sets this operation type.
    ///
    /// \param[in] opChar SAM/BAM character code
    /// \returns reference to this operation
    inline CigarOperation& Char(const char opChar);

    /// Sets this operation length.
    ///
    /// \param[in] length
    /// \returns reference to this operation
    inline CigarOperation& Length(const uint32_t length);

    /// Sets this operation type.
    ///
    /// \param[in] opType CigarOperationType value
    /// \returns reference to this operation
    inline CigarOperation& Type(const CigarOperationType opType);

    /// \}

public:
    /// \name Comparison Operators
    /// \{

    /// \returns true if both CIGAR operation type & length match
    inline bool operator==(const CigarOperation& other) const;

    /// \returns true if either CIGAR operation type or length differ
    inline bool operator!=(const CigarOperation& other) const;

    /// \}

private:
    CigarOperationType type_;
    uint32_t length_;
};

inline CigarOperation::CigarOperation(void)
    : type_(CigarOperationType::UNKNOWN_OP)
    , length_(0)
{ }

inline CigarOperation::CigarOperation(char c, uint32_t length)
    : type_(CigarOperation::CharToType(c))
    , length_(length)
{ }

inline CigarOperation::CigarOperation(CigarOperationType op, uint32_t length)
    : type_(op)
    , length_(length)
{ }

inline CigarOperation::CigarOperation(const CigarOperation& other)
    : type_(other.type_)
    , length_(other.length_)
{ }

inline CigarOperation::~CigarOperation(void) { }

inline uint32_t CigarOperation::Length(void) const
{ return length_; }

inline CigarOperation& CigarOperation::Length(const uint32_t length)
{ length_ = length; return *this; }

inline CigarOperationType CigarOperation::Type(void) const
{ return type_; }

inline CigarOperation &CigarOperation::Type(const CigarOperationType opType)
{ type_ = opType; return *this; }

inline char CigarOperation::Char(void) const
{ return CigarOperation::TypeToChar(type_); }

inline CigarOperation &CigarOperation::Char(const char opChar)
{ type_ = CigarOperation::CharToType(opChar);return *this; }

inline bool CigarOperation::operator==(const CigarOperation& other) const
{ return type_ == other.type_ && length_ == other.length_; }

inline bool CigarOperation::operator!=(const CigarOperation& other) const
{ return !(*this == other); }

} // namespace BAM
} // namespace PacBio

#endif // CIGAROPERATION_H