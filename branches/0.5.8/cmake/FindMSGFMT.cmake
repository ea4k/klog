# Copyright (C) 2009-2009 Jaime Robles <jaime@robles.es>
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Find the msgfmt program, used in the macro ADD_TRANSLATIONS
#
# Defined variables:
#  MSGFMT_FOUND
#  MSGFMT_EXECUTABLE
#
# Macro:
#  ADD_TRANSLATIONS
#
# Usage:
#   find_package(MSGFMT)
#   if(MSGFMT_FOUND)
#       file(GLOB PO_FILES *.po)
#       add_translations(project_name ${PO_FILES})
#   endif(MSGFMT_FOUND)

IF(MSGFMT_EXECUTABLE)
    SET(MSGFMT_FOUND TRUE)
ELSE(MSGFMT_EXECUTABLE)
    FIND_PROGRAM(MSGFMT_EXECUTABLE
    	NAMES msgfmt gmsgfmt
    	PATHS /bin /usr/bin /usr/local/bin )
    IF(MSGFMT_EXECUTABLE)
        SET(MSGFMT_FOUND TRUE)
    ELSE(MSGFMT_EXECUTABLE)
    	IF(NOT MSGFMT_FIND_QUIETLY)
    	    IF(MSGFMT_FIND_REQUIRED)
                MESSAGE(FATAL_ERROR "msgfmt program couldn't be found")
    	    ENDIF(MSGFMT_FIND_REQUIRED)
    	ENDIF(NOT MSGFMT_FIND_QUIETLY)
    ENDIF(MSGFMT_EXECUTABLE)
    MARK_AS_ADVANCED(MSGFMT_EXECUTABLE)
ENDIF (MSGFMT_EXECUTABLE)

MACRO(ADD_TRANSLATIONS _baseName)
    SET(_outputs)
    FOREACH(_file ${ARGN})
    	GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
    	SET(_out "${CMAKE_CURRENT_BINARY_DIR}/${_file_we}.gmo")
    	SET(_in  "${CMAKE_CURRENT_SOURCE_DIR}/${_file_we}.po")
    	ADD_CUSTOM_COMMAND(
    	    OUTPUT ${_out}
    	    COMMAND ${MSGFMT_EXECUTABLE} -o ${_out} ${_in}
    	    DEPENDS ${_in} )
    	INSTALL(FILES ${_out}
    	    DESTINATION ${LOCALE_INSTALL_DIR}/${_file_we}/LC_MESSAGES/
    	    RENAME ${_baseName}.mo )
    	SET(_outputs ${_outputs} ${_out})
    ENDFOREACH(_file)
    ADD_CUSTOM_TARGET(translations ALL DEPENDS ${_outputs})
ENDMACRO(ADD_TRANSLATIONS)
