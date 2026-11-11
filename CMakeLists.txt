CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

PROJECT("cassandra" VERSION 1.1.0)

# RPM/DEB and CENTOS8/ROCKY9/ROCKY10/UBU2204/UBU2404/UBU2604
IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/OSRELEASE.txt")
	FILE(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/OSRELEASE.txt OSRELEASE)
ELSEIF (EXISTS "/OSRELEASE.txt")
	FILE(STRINGS /OSRELEASE.txt OSRELEASE)
ELSE()
	MESSAGE(FATAL_ERROR "Missing OSRELEASE")
ENDIF()

IF (OSRELEASE MATCHES "CENTOS8")
	SET(CPACK_GENERATOR "RPM")
ELSEIF (OSRELEASE MATCHES "ROCKY9")
	SET(CPACK_GENERATOR "RPM")
ELSEIF (OSRELEASE MATCHES "ROCKY10")
	SET(CPACK_GENERATOR "RPM")
ELSEIF (OSRELEASE MATCHES "UBU2204")
	SET(CPACK_GENERATOR "DEB")
ELSEIF (OSRELEASE MATCHES "UBU2404")
	SET(CPACK_GENERATOR "DEB")
ELSEIF (OSRELEASE MATCHES "UBU2604")
	SET(CPACK_GENERATOR "DEB")
ELSE()
	MESSAGE(FATAL_ERROR "Invalid OSRELEASE")
ENDIF()

IF (OSRELEASE MATCHES "^(CENTOS8|ROCKY8|ROCKY9|UBU2204|UBU2404|AZURE3)$")
	SET(HALON_PACKAGE "halon")
ELSE()
	SET(HALON_PACKAGE "halon-smtpd")
ENDIF()

SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -Wall -Wvla -Wshadow -Wconversion -Wno-sign-conversion -Wno-c++11-narrowing -O2 -fno-strict-aliasing -Wextra -Wno-unused-parameter")

ADD_LIBRARY(cassandra SHARED
	src/cassandra.cpp
	src/configuration.cpp
	src/argument.cpp
	src/cassandra/cassandra.cpp
	src/cassandra/statement/statement.cpp
	src/cassandra/statement/bind.cpp
	src/cassandra/statement/execute.cpp
	src/cassandra/statement/paging.cpp
	src/cassandra/batch/batch.cpp
	src/cassandra/batch/execute.cpp
)

# The cpp-driver is linked statically and its symbols are hidden, so the package doesn't ship
# libcassandra and can't clash with another copy loaded in the same process
FIND_LIBRARY(CASSANDRA_STATIC cassandra_static REQUIRED)
FIND_LIBRARY(LIBUV uv REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

TARGET_LINK_LIBRARIES(cassandra
	${CASSANDRA_STATIC}
	${LIBUV}
	OpenSSL::SSL
	OpenSSL::Crypto
	ZLIB::ZLIB
	Threads::Threads
)
TARGET_LINK_OPTIONS(cassandra PRIVATE "LINKER:--exclude-libs,ALL")
# The driver gives OpenSSL pointers to its own functions (CRYPTO_set_mem_functions), which OpenSSL
# calls during its exit cleanup, so the plugin must stay mapped until the process exits
TARGET_LINK_OPTIONS(cassandra PRIVATE "LINKER:-z,nodelete")

SET_TARGET_PROPERTIES(cassandra PROPERTIES PREFIX "")

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	/opt/halon/include
)

IF(EXISTS "/etc/os-release")
	EXECUTE_PROCESS(COMMAND "sed" "-ne" "s/^ID=\"\\?\\([a-z]\\+\\)\"\\?$/\\1/p" "/etc/os-release" OUTPUT_VARIABLE OS_RELEASE_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
	EXECUTE_PROCESS(COMMAND "sed" "-ne" "s/^VERSION_ID=\"\\?\\([0-9\\.]\\+\\)\"\\?$/\\1/p" "/etc/os-release" OUTPUT_VARIABLE OS_RELEASE_VERSION_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
	EXECUTE_PROCESS(COMMAND "uname" "-m" OUTPUT_VARIABLE OS_RELEASE_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
	SET(SYSTEM_NAME "${OS_RELEASE_ID}-${OS_RELEASE_VERSION_ID}-${OS_RELEASE_ARCH}")
ENDIF()

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/cassandra.so DESTINATION plugins)
INSTALL(FILES cassandra.classes.json DESTINATION share/plugins)
INSTALL(FILES cassandra-app.schema.json DESTINATION share/plugins)

IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14.0") 
	FILE(CREATE_LINK "/opt/halon/plugins/cassandra.so" "${CMAKE_CURRENT_BINARY_DIR}/cassandra.so.link" SYMBOLIC)
ELSE()
	ADD_CUSTOM_TARGET(link_target ALL COMMAND ${CMAKE_COMMAND} -E create_symlink "/opt/halon/plugins/cassandra.so" "${CMAKE_CURRENT_BINARY_DIR}/cassandra.so.link")
ENDIF()

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/cassandra.so.link" DESTINATION plugins/autoload/smtpd RENAME cassandra.so)
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/cassandra.so.link" DESTINATION plugins/autoload/hsh RENAME cassandra.so)

INSTALL(FILES "LICENSE" DESTINATION share/doc/plugins/cassandra)

# Licenses for the cpp-driver and the third-party code compiled into it, copied from the cpp-driver
# source the Dockerfiles leave in /tmp/halon. sbom.json lists the same components, update both
# together with the cpp-driver version
SET(CPP_DRIVER_SOURCE_DIR "/tmp/halon/cassandra-cpp-driver-2.17.1" CACHE PATH "cpp-driver source directory")

FUNCTION(INSTALL_CPP_DRIVER_LICENSE NAME FILE)
	IF (NOT EXISTS "${CPP_DRIVER_SOURCE_DIR}/${FILE}")
		MESSAGE(FATAL_ERROR "Missing license for ${NAME}: ${CPP_DRIVER_SOURCE_DIR}/${FILE}")
	ENDIF()
	INSTALL(FILES "${CPP_DRIVER_SOURCE_DIR}/${FILE}" DESTINATION share/doc/plugins/cassandra RENAME "LICENSE.${NAME}")
ENDFUNCTION()

# For code whose license is only in a source file comment, copies the text from BEGIN up to and including END
FUNCTION(INSTALL_CPP_DRIVER_LICENSE_FROM_SOURCE NAME FILE BEGIN END)
	IF (NOT EXISTS "${CPP_DRIVER_SOURCE_DIR}/${FILE}")
		MESSAGE(FATAL_ERROR "Missing license for ${NAME}: ${CPP_DRIVER_SOURCE_DIR}/${FILE}")
	ENDIF()
	FILE(READ "${CPP_DRIVER_SOURCE_DIR}/${FILE}" CONTENT)
	STRING(FIND "${CONTENT}" "${BEGIN}" START)
	IF (START EQUAL -1)
		MESSAGE(FATAL_ERROR "License for ${NAME} not found in ${CPP_DRIVER_SOURCE_DIR}/${FILE}")
	ENDIF()
	STRING(SUBSTRING "${CONTENT}" ${START} -1 CONTENT)
	STRING(FIND "${CONTENT}" "${END}" STOP)
	IF (STOP EQUAL -1)
		MESSAGE(FATAL_ERROR "End of license for ${NAME} not found in ${CPP_DRIVER_SOURCE_DIR}/${FILE}")
	ENDIF()
	STRING(LENGTH "${END}" END_LENGTH)
	MATH(EXPR LENGTH "${STOP} + ${END_LENGTH}")
	STRING(SUBSTRING "${CONTENT}" 0 ${LENGTH} CONTENT)
	FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.${NAME}" "${CONTENT}\n")
	INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.${NAME}" DESTINATION share/doc/plugins/cassandra)
ENDFUNCTION()

INSTALL_CPP_DRIVER_LICENSE(cpp-driver "LICENSE.txt")
INSTALL_CPP_DRIVER_LICENSE(curl "src/third_party/curl/COPYING")
INSTALL_CPP_DRIVER_LICENSE(hdr_histogram "src/third_party/hdr_histogram/LICENSE.txt")
INSTALL_CPP_DRIVER_LICENSE(http-parser "src/third_party/http-parser/LICENSE-MIT")
INSTALL_CPP_DRIVER_LICENSE(rapidjson "src/third_party/rapidjson/license.txt")
INSTALL_CPP_DRIVER_LICENSE(sparsehash "src/third_party/sparsehash/COPYING")
INSTALL_CPP_DRIVER_LICENSE_FROM_SOURCE(minizip "src/third_party/minizip/unzip.h" "unzip.h -- IO" "from any source distribution.")
INSTALL_CPP_DRIVER_LICENSE_FROM_SOURCE(mt19937_64 "src/third_party/mt19937_64/mt19937_64.hpp" "Copyright (C) 2004, Makoto Matsumoto" "POSSIBILITY OF SUCH DAMAGE.")

INSTALL(FILES "sbom.json" DESTINATION share/sbom/plugins RENAME ${PROJECT_NAME}.json)

SET(CMAKE_INSTALL_PREFIX "/opt/halon")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/opt/halon")

IF (CPACK_GENERATOR MATCHES "DEB")
	SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
	# Library dependencies (libuv, OpenSSL, zlib) are added by shlibdeps
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${HALON_PACKAGE} (>= 5.10)")
	SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/postinst;${CMAKE_CURRENT_SOURCE_DIR}/postrm")
ENDIF()
IF (CPACK_GENERATOR MATCHES "RPM")
	# Library dependencies (libuv, OpenSSL, zlib) are added by autoreqprov
	SET(CPACK_RPM_PACKAGE_AUTOREQPROV ON)
	SET(CPACK_RPM_PACKAGE_REQUIRES "${HALON_PACKAGE} >= 5.10")
	SET(CPACK_RPM_PACKAGE_LICENSE "MIT AND Apache-2.0 AND BSD-3-Clause AND curl AND Zlib AND (CC0-1.0 OR BSD-2-Clause)")
	SET(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/postinst")
	SET(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/postrm")
ENDIF()

SET(CPACK_PACKAGE_NAME "halon-extras-cassandra")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Halon extras cassandra client")
SET(CPACK_PACKAGE_DESCRIPTION "Halon extras cassandra client")
SET(CPACK_PACKAGE_CONTACT "Halon support@halon.io")
SET(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/halon-extras/cassandra")
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${SYSTEM_NAME}")

SET(CPACK_DEBIAN_PACKAGE_SECTION "mail")

INCLUDE(CPack)
