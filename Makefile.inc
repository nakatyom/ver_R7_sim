mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
port_settings.o\
linetrace.o



APPL_LIBS += -lm
APPL_DIRS += \
    $(mkfile_path)\
    $(mkfile_path)/Drv\
    $(mkfile_path)/Mid
INCLUDES += \
    -I$(mkfile_path)\
    -I$(mkfile_path)/Drv\
    -I$(mkfile_path)/Mid