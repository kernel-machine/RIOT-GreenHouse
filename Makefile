# Set the name of your application:
APPLICATION = green_house
# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401re

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/RIOT

ifeq (nucleo-f401re,$(BOARD))
	CFLAGS += -DUSE_STM32F401RE
else ifeq (native,$(BOARD))
	CFLAGS += -DUSE_NATIVE
endif

# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS
#CFLAGS += -DDEVELHELP
#CFLAGS_OPT = -Os
# If you want to use native with valgrind, you should recompile native
# with the target all-valgrind instead of all:
# make -B clean all-valgrind

UPLINK ?= ethos
# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

# Specify the mandatory networking modules for IPv6
USEMODULE += gnrc_ipv6_default

# Include MQTT-S client
USEMODULE += emcute

# Optimize network stack to for use with a single network interface
USEMODULE += gnrc_netif_single

USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
# This application dumps received packets to STDIO using the pktdump module
USEMODULE += gnrc_pktdump
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
# Add also the shell, some shell commands

#Import devices and src folders
USEMODULE += devices
DIRS += $(CURDIR)/devices
USEMODULE += src
DIRS += $(CURDIR)/src

# Modules to include:
USEMODULE += dht
USEMODULE += xtimer
USEMODULE += analog_util

ifeq (nucleo-f401re,$(BOARD))
	USEMODULE += servo
endif

USEMODULE += shell
uSEMODULE += shell_commands
USEMODULE += ps

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# include UHCP client
USE_DHCPV6 ?= 0

# If your application is very simple and doesn't use modules that use
# messaging, it can be disabled to save some memory:
ifeq (nucleo-f401re,$(BOARD))
	FEATURES_REQUIRED += periph_adc
endif
# Uncomment this to enable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 to show compiler invocation lines by default:
QUIET ?= 1

# Ethos/native TAP interface and UHCP prefix can be configured from make command
TAP ?= tap0
IPV6_PREFIX ?= fe80:2::/64

# The Broker address, port and the default MQTT topic to subscribe.
SERVER_ADDR = fe80::1
SERVER_PORT = 1885
MQTT_TOPIC = gh
MQTT_CMD_TOPIC = cmd
UDP_SERVER_PORT = 8888

CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_TOPIC='"$(MQTT_TOPIC)"'
CFLAGS += -DMQTT_CMD_TOPIC='"$(MQTT_CMD_TOPIC)"'
CFLAGS += -DUDP_SERVER_PORT='"$(UDP_SERVER_PORT)"'

ETHOS_BAUDRATE ?= 115200
include $(CURDIR)/Makefile.ethos.conf

# Specify custom dependencies for your application here ...
# APPDEPS = app_data.h config.h
include $(RIOTBASE)/Makefile.include
include $(CURDIR)/Makefile.conf.include
.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)
# ... and define them here (after including Makefile.include,
# otherwise you modify the standard target):
#proj_data.h: script.py data.tar.gz
#	./script.py
