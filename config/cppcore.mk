CPPCORE_DOC_DIR     = $(CPPCORE_TOP)/doc
CPPCORE_ARCH_DIR    = $(CPPCORE_TOP)/arch
CPPCORE_DRIVERS_DIR = $(CPPCORE_TOP)/drivers
CPPCORE_INCLUDE_DIR = $(CPPCORE_TOP)/include

CPPCORE_SRC_DIRS    = $(CPPCORE_ARCH_DIR)/$(CPPCORE_ARCH)

CPPCORE_SRC  = $(wildcard $(addsuffix /*.cpp, $(CPPCORE_SRC_DIRS)))

CPPCORE_INCLUDE     = -I $(CPPCORE_INCLUDE_DIR)
CPPCORE_INCLUDE    += -I $(CPPCORE_DRIVERS_DIR)
CPPCORE_INCLUDE    += -I $(CPPCORE_ARCH_DIR)/$(CPPCORE_ARCH)/include


#CPPCORE_OBJ_DIR     = $(OBJDIR)/cppcore

#CPPCORE_OBJS := $(CPPCORE_SRC:$(CPPCORE_TOP)%=$(CPPCORE_OBJ_DIR)%)
#CPPCORE_OBJS := $(CPPCORE_OBJS:.cpp=.o)

#$(CPPCORE_OBJ_DIR)/%.o: $(CPPCORE_TOP)/%.cpp obj/gnabber
##	echo --- compiling $(*F).cpp
#	@$(MKDIR_P) $(dir $@) 

#	$(CXX) -c $(CXXFLAGS) -o $@ $<
