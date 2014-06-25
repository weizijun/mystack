cmake_minimum_required(VERSION 2.4)

PROJECT(mystack)

SET(COMMON_HOME ./common/)
SET(SRC_HOME ./mystack/)
SET(CORE_HOME ./core/)
SET(STACK_COMLIB /usr/local/avenue/stack)


INCLUDE_DIRECTORIES(
	./
	${COMMON_HOME}
	${COMMON_HOME}/detail	
    $ENV{BOOST_ROOT}
	${SRC_HOME}
	${CORE_HOME}
	${STACK_COMLIB}/release/include/xlog
    ${STACK_COMLIB}/release/include	
)

FIND_LIBRARY(BOOSTSYSTEM_LIB boost_system $ENV{BOOST_LIB} NO_DEFAULT_PATH)
FIND_LIBRARY(XLOG_LIB xlog ${STACK_COMLIB}/release/Linux2.6_Gcc3.4 NO_DEFAULT_PATH)
FIND_LIBRARY(TINYXML_LIB tinyxml ${STACK_COMLIB}/release/Linux2.6_Gcc3.4 NO_DEFAULT_PATH)


LINK_DIRECTORIES(
	/usr/lib
    $ENV{BOOST_LIB}
	${STACK_COMLIB}/release/Linux2.6_Gcc3.4
)

ADD_DEFINITIONS(
	-DTIXML_USE_STL
    -O3 -g -Wno-unused-variable -Wall 
    -D_REENTRANT
    -D_LINUX
	-DLINUX
    -DDEBUG
)

AUX_SOURCE_DIRECTORY(${COMMON_HOME} COMMON_FILE_LIST)
AUX_SOURCE_DIRECTORY(${SRC_HOME} SRC_FILE_LIST)
AUX_SOURCE_DIRECTORY(${CORE_HOME} CORE_HOME)

ADD_EXECUTABLE( mystackserver
    ${COMMON_FILE_LIST}
	${SRC_FILE_LIST}
	${CORE_HOME}
)


TARGET_LINK_LIBRARIES(mystackserver boost_system boost_date_time boost_thread pthread xlog tinyxml)