#
# Input variables:
#
# - OPENMPTL_TOP  : OpenMPTL top directory
# - OPENMPTL_ARCH : Architecture (e.g. "stm32f10x")
#
#
# Output variables:
#
# - OPENMPTL_INCLUDE : compiler include flags
#

OPENMPTL_DOC_DIR     = $(OPENMPTL_TOP)/doc
OPENMPTL_ARCH_DIR    = $(OPENMPTL_TOP)/arch
OPENMPTL_DRIVERS_DIR = $(OPENMPTL_TOP)/drivers
OPENMPTL_LIB_DIR     = $(OPENMPTL_TOP)/lib
OPENMPTL_INCLUDE_DIR = $(OPENMPTL_TOP)/include

OPENMPTL_SRC_DIRS    = $(OPENMPTL_LIB_DIR)
OPENMPTL_SRC         = $(wildcard $(addsuffix /*.cpp, $(OPENMPTL_SRC_DIRS)))

OPENMPTL_INCLUDE     = -I $(OPENMPTL_INCLUDE_DIR)
OPENMPTL_INCLUDE    += -I $(OPENMPTL_DRIVERS_DIR)
OPENMPTL_INCLUDE    += -I $(OPENMPTL_LIB_DIR)/include
OPENMPTL_INCLUDE    += -I $(OPENMPTL_ARCH_DIR)/$(OPENMPTL_ARCH)/include
