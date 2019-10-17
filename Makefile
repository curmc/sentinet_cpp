# Output directory
BUILD_DIR=`pwd`/build

# CMAKE build target
CMAKE_TARGET=cmake -S . -B ${BUILD_DIR} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Target options
CMAKE_EXE_OPTIONS=-DBUILD_EXE=ON
CMAKE_RMT_CORE_OPTIONS=-DRMT_CORE=ON
CMAKE_EXAMPLES_OPTIONS=-DEXAMPLES=ON

# Global make target commands
MAKE_TARGET=make -C ${BUILD_DIR}
INSTALL_TARGET=${MAKE_TARGET} install

# Files
FILE_SUFFIXES='.*\.\(cpp\|hpp\|h\)'
FORMAT_TARGET=clang-format -i -sort-includes=false -style=Mozilla
CLEAN=rm -rf ${BUILD_DIR}
CLEAN_ALL=rm -rf ${BUILD_DIR}
UBUNDUT_DEPS=cmake curl libcurl4-gnutls-dev autoconf automake libtool g++ unzip libzmq3-dev xargs
ARCH_DEPS=cmake curl unzip zeromq


.PHONY: all
all: rmtcore examples sentinet cmake compile
	@echo "Compiling rmt_core, examples and sentinet libs"

.PHONY: install
install: rmtcore sentinet cmake compile
	@echo "Compiling and building"
	@sudo ${INSTALL_TARGET}


.PHONY: examples
examples: 
	@echo "Compiling rmt examples"
	$(eval CMAKE_TARGET = ${CMAKE_TARGET} ${CMAKE_EXAMPLES_OPTIONS})

.PHONY: rmtcore
rmtcore: 
	@echo "Compiling rmt core"
	$(eval CMAKE_TARGET = ${CMAKE_TARGET} ${CMAKE_RMT_CORE_OPTIONS})

.PHONY: sentinet
	@echo "Nothing done herE"

.PHONY: cmake
cmake:
	@echo "Generating cmake command"
	${CMAKE_TARGET}

.PHONY: compile
compile:
	@echo "Compiling"
	@${MAKE_TARGET}

.PHONY: format
format:
	@find ./src -regex ${FILE_SUFFIXES} | xargs ${FORMAT_TARGET}
	@find ./include -regex ${FILE_SUFFIXES} | xargs ${FORMAT_TARGET}


.PHONY: install-deps
install-deps:
	@echo "Installing requirements"
	@-pacman --version > /dev/null 2>&1 && sudo pacman -S ${ARCH_DEPS}
	@-apt --version > /dev/null 2>&1 && sudo apt install ${UBUNDUT_DEPS}

.PHONY: clean-all
clean-all: 
	@echo "Removing build"
	@rm -rf ${BUILD_DIR}
