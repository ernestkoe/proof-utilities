##############################################################################
# Compile the Proof Utilities plug-in in 32- and 64-bit mode.

# Settings

FM_SDK_VERSION = 14
PLUGIN_FILE_NAME = ProofUtilities

##############################################################################
# Standard CPPFLAGS

CPPFLAGS += -IPluginSDK/v$(FM_SDK_VERSION)/Headers

# -I<DIRECTORY>   Look for included files in this directory. Works both for gcc
# and Microsoft cl.

##############################################################################
# Include platform-specific rules

ifeq ($(shell uname -s),Darwin)
  include Makefile.mac
else ifeq ($(shell uname -o),Cygwin)
  include Makefile.win
endif

##############################################################################
# Rule to output colored messages

define MESSAGE
  @if [ -t 1 ] ; \
    then r="\e[0;32m" ; g="\e[0;32m" ; y="\e[0;33m" ; b="\e[0;34m" ; \
         R="\e[1;32m" ; G="\e[1;32m" ; Y="\e[1;33m" ; B="\e[1;34m" ; \
         z="\e[0m"; \
    else r=""         ; g=""         ; y=""         ; b=""         ; \
         R=""         ; G=""         ; Y=""         ; B=""         ; \
         z=""     ; \
  fi ; printf 
endef

##############################################################################
# Rule to automatically add directories.

define MAKE-PARENT-DIRECTORY   # $@
  $(MESSAGE) "$${y}Creating directory:$${z} $(@D)\n"
  mkdir -p $(@D)
endef
define MAKE-PARENT-DIRECTORY?   # $@
  $(if $(wildcard $(@D)),,$(MAKE-PARENT-DIRECTORY))
endef

# Legend
#   -p  create intermediate directories as required

