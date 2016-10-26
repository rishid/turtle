
BUILD_DIR   :=  build
OUTPUT_DIR  :=  output

CMAKE_BUILD_TYPE ?= Debug
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

# Extra CMake flags which extend the default set
CMAKE_EXTRA_FLAGS ?=

# For use where we want to make sure only a single job is run. This also avoids
# any warnings from the sub-make.
SINGLE_MAKE = export MAKEFLAGS= ; $(MAKE)

all: project

project: ${BUILD_DIR}/Makefile
	@$(MAKE) -C ${BUILD_DIR}

cmake:
	touch CMakeLists.txt
	$(MAKE) ${BUILD_DIR}/Makefile

${BUILD_DIR}/Makefile:
	@[ -d ${BUILD_DIR} ] || mkdir -p ${BUILD_DIR}
	@[ -f ${BUILD_DIR}/Makefile ] || (cd ${BUILD_DIR} && cmake $(CMAKE_FLAGS) $(CMAKE_EXTRA_FLAGS) ..)
	touch $@

install: project
	@$(MAKE) -C ${BUILD_DIR} install

clean:
	@([ -d ${BUILD_DIR} ] && $(MAKE) -C ${BUILD_DIR} clean) || echo Nothing to clean

distclean:
	@([ -f src/libffi/Makefile ] && $(MAKE) -C src/libffi distclean) || echo
	@rm -rf ${BUILD_DIR} ${OUTPUT_DIR}

.PHONY: install clean distclean project cmake ${BUILD_DIR}/Makefile

