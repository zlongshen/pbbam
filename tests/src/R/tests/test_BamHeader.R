# Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted (subject to the limitations in the
# disclaimer below) provided that the following conditions are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#
#  * Neither the name of Pacific Biosciences nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
# GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
# BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# Author: Derek Barnett

empty_program <- function(header) {
	result <- tryCatch(
		{
			pg <- header$Program("foo")
			assertTrue(FALSE) # should have thrown 
			invisible()
		},
		warning = function(w) {
			assertTrue(TRUE)
			invisible()
		}
	)
	return(result)
}

empty_readgroup <- function(header) {
	result <- tryCatch(
		{
			pg <- header$ReadGroup("foo")
			assertTrue(FALSE) # should have thrown 
			invisible()
		},
		warning = function(w) {
			assertTrue(TRUE)
			invisible()
		}
	)
	return(result)
}

empty_sequenceid <- function(header) {
	result <- tryCatch(
		{
			pg <- header$SequenceId("foo")
			assertTrue(FALSE) # should have thrown 
			invisible()
		},
		warning = function(w) {
			assertTrue(TRUE)
			invisible()
		}
	)
	return(result)
}

test_case("BamHeader_Defaults", {
	
    header <- BamHeader()
	
	assertEqual(0L, nchar(header$Version()))
	assertEqual(0L, nchar(header$SortOrder()))
	assertTrue(header$ReadGroups()$empty())
	assertTrue(header$Sequences()$empty())
	assertTrue(header$Programs()$empty())
	
	pg <- empty_program(header)
	rg <- empty_readgroup(header)
	id <- empty_sequenceid(header)
	
	# TODO: get comment fetching working
	#assertEqual(1, length(header$Comments()))
})

test_case("BamHeader_Decode", { 
	
    text <- paste("@HD\tVN:1.1\tSO:queryname\tpb:3.0.1",
			      "@SQ\tSN:chr1\tLN:2038\tSP:chocobo",
				  "@SQ\tSN:chr2\tLN:3042\tSP:chocobo",
				  "@RG\tID:rg1\tSM:control",
				  "@RG\tID:rg2\tSM:condition1",
				  "@RG\tID:rg3\tSM:condition1",
				  "@PG\tID:_foo_\tPN:ide",
				  "@CO\tipsum and so on",
				  "@CO\tcitation needed",
				  sep="\n"
	             )
				
	header <- BamHeader(text)
	
	assertEqual("1.1",       header$Version())
	assertEqual("queryname", header$SortOrder())
	assertEqual("3.0.1",     header$PacBioBamVersion())

	assertEqual(3L, header$ReadGroups()$size())
	assertTrue(header$HasReadGroup("rg1"))
	assertTrue(header$HasReadGroup("rg2"))
	assertTrue(header$HasReadGroup("rg3"))
	assertEqual("control",    header$ReadGroup("rg1")$Sample())
	assertEqual("condition1", header$ReadGroup("rg2")$Sample())
	assertEqual("condition1", header$ReadGroup("rg3")$Sample())
	
	assertEqual(2L, header$Sequences()$size())
	assertTrue(header$HasSequence("chr1"))
	assertTrue(header$HasSequence("chr2"))
	assertEqual("chocobo", header$Sequence("chr1")$Species())
	assertEqual("chocobo", header$Sequence("chr2")$Species())
	assertEqual("2038",    header$Sequence("chr1")$Length())
	assertEqual("3042",    header$Sequence("chr2")$Length())
  
	assertEqual(1L, header$Programs()$size())
	assertTrue(header$HasProgram("_foo_"))
	assertEqual("ide", header$Program("_foo_")$Name())

	# TODO: get comment fetching working
	# assertEqual(2, header$Comments()$size())
	# assertEqual("ipsum and so on", header$Comments()[1])
	# assertEqual("citation needed", header$Comments()[2])
})
	
test_case("BamHeader_Encode", { 
	
    expectedText <- paste("@HD\tVN:1.1\tSO:queryname\tpb:3.0.1",
			              "@SQ\tSN:chr1\tLN:2038\tSP:chocobo",
				          "@SQ\tSN:chr2\tLN:3042\tSP:chocobo",
                                          "@RG\tID:rg1\tPL:PACBIO\tDS:READTYPE=UNKNOWN\tSM:control\tPM:SEQUEL",
                                          "@RG\tID:rg2\tPL:PACBIO\tDS:READTYPE=UNKNOWN\tSM:condition1\tPM:SEQUEL",
                                          "@RG\tID:rg3\tPL:PACBIO\tDS:READTYPE=UNKNOWN\tSM:condition1\tPM:SEQUEL",
				          "@PG\tID:_foo_\tPN:ide",
				          "@CO\tipsum and so on",
				          "@CO\tcitation needed",
						  "",
				          sep="\n"
	                     )

	rg1 <- ReadGroupInfo("rg1")
	rg1$Sample("control")
	rg2 <- ReadGroupInfo("rg2")
	rg2$Sample("condition1")
	rg3 <- ReadGroupInfo("rg3")
	rg3$Sample("condition1")

	seq1 <- SequenceInfo("chr1")
	seq1$Length("2038")
	seq1$Species("chocobo")
	seq2 <- SequenceInfo("chr2")
	seq2$Length("3042")
	seq2$Species("chocobo")
    
	prog1 <- ProgramInfo("_foo_")
	prog1$Name("ide")

	header <- BamHeader()
	header$Version("1.1")
	header$SortOrder("queryname")
	header$PacBioBamVersion("3.0.1")
	header$AddReadGroup(rg1)
	header$AddReadGroup(rg2)
	header$AddReadGroup(rg3)
	header$AddSequence(seq1)
	header$AddSequence(seq2)
	header$AddProgram(prog1)
	header$AddComment("ipsum and so on")
	header$AddComment("citation needed")
	
	assertEqual(expectedText, header$ToSam())
})
