# name of your application
APPLICATION = greenhouse

# If no BOARD is found in the environment, use this default:
BOARD ?= b-l072z-lrwan1
FAKE ?= 1

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

DEVEUI ?= 00944D5980908C0C
APPEUI ?= 70B3D57ED00432E1
APPKEY ?= BDCD6967FD7FCDEF7A0D622852B2CBFE

ifeq (nucleo-f401re,$(BOARD))
	CFLAGS += -DUSE_STM32F401RE
endif
ifeq (1,$(FAKE))
	CFLAGS += -DFAKE_SENSOR
else
	USEMODULE += analog_util
	USEMODULE += dht
	FEATURES_REQUIRED += periph_adc
endif

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += random

#Import devices and main folders
USEMODULE += devices
DIRS += $(CURDIR)/devices
USEMODULE += main
DIRS += $(CURDIR)/main

USEMODULE += xtimer
ifeq (nucleo-f401re,$(BOARD))
	USEMODULE += servo
endif

LORA_REGION ?= EU868
USEPKG += semtech-loramac
USEMODULE += sx1276
USEMODULE += fmt
FEATURES_REQUIRED += periph_rtc

DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(CURDIR)/Makefile.conf.include

include $(RIOTBASE)/Makefile.include

ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
  # OTAA compile time configuration keys
  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
endif