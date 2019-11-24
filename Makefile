# Output directory
BUILD_DIR=`pwd`/build

# CMAKE build target
CMAKE_TARGET_PREFIX=cmake -S . -B ${BUILD_DIR} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_SHARED=ON

# Target options
CMAKE_EXE_OPTIONS=-DBUILD_EXE=ON
CMAKE_RMT_CORE_OPTIONS=-DRMT_CORE=ON
CMAKE_EXAMPLES_OPTIONS=-DEXAMPLES=ON
CMAKE_TESTS_OPTIONS=-DBUILD_TESTS=ON

# Global make target commands - common for all 
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
all: validate
	@echo "Compiling rmt_core, examples and sentinet libs"

.PHONY: install
install: rmtcore sentinet tests cmake compile validate
	@echo "Compiling and building"
	@sudo ${INSTALL_TARGET}

.PHONY: rmtcore
rmtcore: 
	$(eval CMAKE_TARGET = ${CMAKE_TARGET_PREFIX} ${CMAKE_RMT_CORE_OPTIONS})
	@printf "\n\n\n\n============== Generating rmt core ============= \n\n\n\n"
	${CMAKE_TARGET}
	${MAKE_TARGET}

.PHONY: validate 
validate: tests
	@printf "\n\n\n\n=============== Running Unit Tests ====================\n\n\n\n"
	@`pwd`/build/x86_64/core/bin/run_unit_tests

.PHONY: tests 
tests:
	$(eval CMAKE_TARGET = ${CMAKE_TARGET_PREFIX} ${CMAKE_TESTS_OPTIONS} ${CMAKE_RMT_CORE_OPTIONS})
	@printf "\n\n\n\n============== Generating Tests ============= \n\n\n\n"
	${CMAKE_TARGET}
	${MAKE_TARGET}

.PHONY: examples
examples: 
	$(eval CMAKE_TARGET = ${CMAKE_TARGET_PREFIX} ${CMAKE_EXAMPLES_OPTIONS} ${CMAKE_RMT_CORE_OPTIONS})
	@printf "\n\n\n\n============== Generating Examples ============= \n\n\n\n"
	${CMAKE_TARGET}
	${MAKE_TARGET}

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
