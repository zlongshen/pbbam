
set(PbindexdumpSrcDir ${PacBioBAM_ToolsDir}/pbindexdump/src)

# create version header
set(PbIndexDump_VERSION ${PacBioBAM_VERSION})
configure_file(
    ${PbindexdumpSrcDir}/PbIndexDumpVersion.h.in PbIndexDumpVersion.h @ONLY
)

# list source files
set(PBINDEXDUMP_SOURCES
    ${ToolsCommonDir}/OptionParser.cpp
    ${PbindexdumpSrcDir}/CppFormatter.cpp
    ${PbindexdumpSrcDir}/JsonFormatter.cpp
    ${PbindexdumpSrcDir}/PbIndexDump.cpp
    ${PbindexdumpSrcDir}/main.cpp
)

# build pbindexdump executable
include(PbbamTool)
create_pbbam_tool(
    TARGET  pbindexdump
    SOURCES ${PBINDEXDUMP_SOURCES}
)

# cram tests
if (PacBioBAM_build_tests)
    add_test(
        NAME pbindexdump_CramTests
        WORKING_DIRECTORY ${PacBioBAM_TestsDir}/scripts
        COMMAND "python" cram.py
            ${PacBioBAM_CramTestsDir}/pbindexdump_json.t
            ${PacBioBAM_CramTestsDir}/pbindexdump_cpp.t
    )
endif()
