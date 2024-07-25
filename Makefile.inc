mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += Line.o

APPL_LIBS += -lm
# APPL_DIRS += zoudatest
# INCLUDES += zoudatest

APPL_DIRS += $(mkfile_path)Mid
INCLUDES += -I$(mkfile_path)Mid
