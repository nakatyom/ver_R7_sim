mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
color_sensor.o\
port_settings.o\
ultrasonic_sensor.o\
gyro_sensor.o

APPL_LIBS += -lm
APPL_DIRS += \
    $(mkfile_path)\
    $(mkfile_path)/Drv\
    $(mkfile_path)/Mid
INCLUDES += \
    -I$(mkfile_path)\
    -I$(mkfile_path)/Drv\
    -I$(mkfile_path)/Mid