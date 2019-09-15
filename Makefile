BUILD_DIR=`pwd`/build

CMAKE_TARGET=cmake -S . -B ${BUILD_DIR}

CMAKE_OPTIONS=-DCMAKE_EXPORT_COMPILE_COMMANDS=ON

MAKE_TARGET=make -C ${BUILD_DIR}

FILE_SUFFIXES=-iname *.h -o -iname *.cpp -o -iname *.hpp

FORMAT_TARGET=clang-format -i -sort-includes=false -style=Google

CLEAN=rm -rf ${BUILD_DIR}

CLEAN_ALL=rm -rf ${BUILD_DIR}

UBUNDUT_DEPS=cmake curl libcurl4-gnutls-dev autoconf automake libtool g++ unzip libzmq3-dev xargs

all :: cmake compile

cmake:
	@${CMAKE_TARGET} ${CMAKE_OPTIONS}

compile:
	@${MAKE_TARGET}

clean:
	@${CLEAN}

format ::
	@find . ${FILE_SUFFIXES} | xargs ${FORMAT_TARGET}

clean-all:
	@${CLEAN_ALL}

install-deps ::
	@sudo apt install ${UBUNDUT_DEPS}


