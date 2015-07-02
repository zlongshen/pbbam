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

#ifndef TAG_H
#define TAG_H

#include "pbbam/Config.h"
#include <boost/variant.hpp>
#include <string>
#include <vector>

namespace PacBio {
namespace BAM {

/// \brief Provides information on the exact (C++) data type held by a Tag.
enum class TagDataType
{
    INVALID      = 0     ///< boost::blank
  , INT8                 ///< int8_t
  , UINT8                ///< uint8_t
  , INT16                ///< int16_t
  , UINT16               ///< uint16_t
  , INT32        = 5     ///< int32_t
  , UINT32               ///< uint32_t
  , FLOAT                ///< float
  , STRING               ///< std::string
  , INT8_ARRAY           ///< std::vector<int8_t>
  , UINT8_ARRAY  = 10    ///< std::vector<uint8_t>
  , INT16_ARRAY          ///< std::vector<int16_t>
  , UINT16_ARRAY         ///< std::vector<uint16_t>
  , INT32_ARRAY          ///< std::vector<int32_t>
  , UINT32_ARRAY         ///< std::vector<uint32_t>
  , FLOAT_ARRAY  = 15    ///< std::vector<float>
};

/// \brief Provides additional instructions on interpreting the tag's value.
///
/// Some C++ data types (e.g. std::string) may represent more than one BAM tag type
/// ('H' vs 'Z'). These modifiers indicate how to properly interpret those shared
/// data types.
enum class TagModifier
{
    /// \brief This indicates the tag has no modifiers set.
    NONE = 0,

    /// \brief This modifier marks an integer as ASCII.
    ///
    /// SAM/BAM has the concept of an ASCII character that is distinct from an 8-bit
    /// integer. However, there is no such pure separation in C++
    /// (int8_t/uint8_t are likely implemented as typedefs around char/unsigned char).
    /// Thus this modifier can be used to indicate a tag's integer data should be
    /// interpreted as a printable, ASCII character.
    ASCII_CHAR,

    /// \brief This modifier marks std::string data as "hex string", rather than a regular string.
    ///
    /// SAM/BAM has a distinction between regular strings and "Hex format" strings.
    /// However, they are both manipulated in C++ via std::string. Thus this modifier
    /// can be used to indicate that a tag's string data should be interpreted as
    /// "Hex format" rather than a regular, literal string.
    HEX_STRING
};

// Inspired by (but greatly simplified & modified from) the boost::variant
// wrapper approach taken by DynamicCpp (https://code.google.com/p/dynamic-cpp)
class PBBAM_EXPORT Tag
{
public:

    /// \name Constructors & Related Methods
    /// \{

    /// Constructs a null tag.
    /// \sa IsNull()
    Tag(void);

    Tag(int8_t value);
    Tag(uint8_t value);
    Tag(int16_t value);
    Tag(uint16_t value);
    Tag(int32_t value);
    Tag(uint32_t value);
    Tag(float value);
    Tag(const std::string& value);
    Tag(const std::vector<int8_t>& value);
    Tag(const std::vector<uint8_t>& value);
    Tag(const std::vector<int16_t>& value);
    Tag(const std::vector<uint16_t>& value);
    Tag(const std::vector<int32_t>& value);
    Tag(const std::vector<uint32_t>& value);
    Tag(const std::vector<float>& value);
    
    Tag(const Tag& other);
    Tag(Tag&& other);

    ~Tag(void);

    Tag& operator=(boost::blank value);
    Tag& operator=(int8_t value);
    Tag& operator=(uint8_t value);
    Tag& operator=(int16_t value);
    Tag& operator=(uint16_t value);
    Tag& operator=(int32_t value);
    Tag& operator=(uint32_t value);
    Tag& operator=(float value);
    Tag& operator=(const std::string& value);
    Tag& operator=(const std::vector<int8_t>& value);
    Tag& operator=(const std::vector<uint8_t>& value);
    Tag& operator=(const std::vector<int16_t>& value);
    Tag& operator=(const std::vector<uint16_t>& value);
    Tag& operator=(const std::vector<int32_t>& value);
    Tag& operator=(const std::vector<uint32_t>& value);
    Tag& operator=(const std::vector<float>& value);
    Tag& operator=(const Tag& other);
    Tag& operator=(Tag&& other);

    bool operator== (const Tag& other) const;
    bool operator!= (const Tag& other) const;

    /// \}

public:
    /// \name Data Conversion & Validation
    /// \{

    /// Converts the tag value to an ASCII character
    ///
    /// Tag must hold an integer type, within the valid ASCII range [33-127].
    ///
    /// \returns ASCII character if valid
    /// \throws if not ASCII-compatible
    char ToAscii(void) const;

    /// \returns tag data as signed 8-bit (casting if needed)
    /// \throws if not integral data, or out of valid range
    int8_t ToInt8(void) const;

    /// \returns tag data as unsigned 8-bit (casting if needed)
    /// \throws if not integral data, or out of valid range
    uint8_t ToUInt8(void) const;

    /// \returns tag data as signed 16-bit (casting if needed)
    /// \throws if not integral data, or out of valid range
    int16_t ToInt16(void) const;

    /// \returns tag data as unsigned 16-bit (casting if needed)
    /// \throws if not integral data, or out of valid range
    uint16_t ToUInt16(void) const;

    /// \returns tag data as signed 32-bit (casting if needed)
    /// \throws if not integral data, or out of valid range
    int32_t ToInt32(void) const;

    /// \returns tag data as unsigned 32-bit (casting if needed)
    /// \throws if not integral data, or out of valid range
    uint32_t ToUInt32(void) const;

    /// \returns tag data as float
    /// \throws if tag does not contain a value of explicit type: float
    float ToFloat(void) const;

    /// \returns tag data as std::string
    /// \throws if tag does not contain a value of explicit type: std::string
    std::string ToString(void) const;

    /// \returns tag data as std::vector<int8_t>
    /// \throws if tag does not contain a value of explicit type: std::vector<int8_t>
    std::vector<int8_t> ToInt8Array(void) const;

    /// \returns tag data as std::vector<uint8_t>
    /// \throws if tag does not contain a value of explicit type: std::vector<uint8_t>
    std::vector<uint8_t> ToUInt8Array(void) const;

    /// \returns tag data as std::vector<int16_t>
    /// \throws if tag does not contain a value of explicit type: std::vector<int16_t>
    std::vector<int16_t> ToInt16Array(void) const;

    /// \returns tag data as std::vector<uint16_t>
    /// \throws if tag does not contain a value of explicit type: std::vector<uint16_t>
    std::vector<uint16_t> ToUInt16Array(void) const;

    /// \returns tag data as std::vector<int32_t>
    /// \throws if tag does not contain a value of explicit type: std::vector<int32_t>
    std::vector<int32_t> ToInt32Array(void) const;

    /// \returns tag data as std::vector<uint32_t>
    /// \throws if tag does not contain a value of explicit type: std::vector<uint32_t>
    std::vector<uint32_t> ToUInt32Array(void) const;

    /// \returns tag data as std::vector<float>
    /// \throws if tag does not contain a value of explicit type: std::vector<float>
    std::vector<float> ToFloatArray(void) const;

    /// \}

public:

    /// \name Data Conversion & Validation
    ///

    /// \returns true if tag is null (e.g. default-constructed)
    bool IsNull(void) const;

    /// \returns true if tag contains a value of type: int8_t
    bool IsInt8(void) const;

    /// \returns true if tag contains a value of type: uint8_t
    bool IsUInt8(void) const;

    /// \returns true if tag contains a value of type: int16_t
    bool IsInt16(void) const;

    /// \returns true if tag contains a value of type: uint16_t
    bool IsUInt16(void) const;

    /// \returns true if tag contains a value of type: int32_t
    bool IsInt32(void) const;

    /// \returns true if tag contains a value of type: uint32_t
    bool IsUInt32(void) const;

    /// \returns true if tag contains a value of type: float
    bool IsFloat(void) const;

    /// \returns true if tag contains a value of type: std::string
    bool IsString(void) const;

    /// \returns true if tag contains a value of type: std::string \b AND has a TagModifier of HEX_STRING
    bool IsHexString(void) const;

    /// \returns true if tag contains a value of type: std::vector<int8_t>
    bool IsInt8Array(void) const;

    /// \returns true if tag contains a value of type: std::vector<uint8_t>
    bool IsUInt8Array(void) const;

    /// \returns true if tag contains a value of type: std::vector<int16_t>
    bool IsInt16Array(void) const;

    /// \returns true if tag contains a value of type: std::vector<uint16_t>
    bool IsUInt16Array(void) const;

    /// \returns true if tag contains a value of type: std::vector<int32_t>
    bool IsInt32Array(void) const;

    /// \returns true if tag contains a value of type: std::vector<uint32_t>
    bool IsUInt32Array(void) const;

    /// \returns true if tag contains a value of type: std::vector<float>
    bool IsFloatArray(void) const;

    /// \returns true if tag contains a value with any signed integer type
    bool IsSignedInt(void) const;

    /// \returns true if tag contains a value with any unsigned integer type
    bool IsUnsignedInt(void) const;

    /// \returns true if tag contains a value with any integer type
    bool IsIntegral(void) const;

    /// \returns true if tag contains a value with any integer or float type
    bool IsNumeric(void) const;

    /// \returns true if tag contains a vector containing signed integers
    bool IsSignedArray(void) const;

    /// \returns true if tag contains a vector containing unsigned integers
    bool IsUnsignedArray(void) const;

    /// \returns true if tag contains a vector containing integers
    bool IsIntegralArray(void) const;

    /// \returns true if tag contains a vector (integers or floats)
    bool IsArray(void) const;

    /// \}

public:
    /// \name Type & Modifier Attributes
    /// \{

    /// \returns enum value for current tag data
    TagDataType Type(void) const;

    /// \returns printable type name for current tag data
    std::string Typename(void) const;

    /// \returns true if tag data modifier \p m is set
    bool HasModifier(const TagModifier m) const;

    /// \returns current tag data modifier
    TagModifier Modifier(void) const;

    /// Sets tag data modifier
    /// \returns reference to this tag
    Tag& Modifier(const TagModifier m);

    /// \}

private :
    // NOTE - keep this synced with TagDataType enum ordering
    typedef boost::variant<boost::blank, // <-- default constructor creates variant of this type
                           int8_t,
                           uint8_t,
                           int16_t,
                           uint16_t,
                           int32_t,
                           uint32_t,
                           float,
                           std::string,
                           std::vector<int8_t>,
                           std::vector<uint8_t>,
                           std::vector<int16_t>,
                           std::vector<uint16_t>,
                           std::vector<int32_t>,
                           std::vector<uint32_t>,
                           std::vector<float> > var_t;

    var_t data_;
    TagModifier modifier_;
};

} // namespace BAM
} // namespace PacBio

#include "internal/Tag.inl"

#endif // TAG_H
