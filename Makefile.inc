mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += zoudatest/motor_test.o

APPL_LIBS += -lm
APPL_DIRS += zoudatest
INCLUDES += zoudatest