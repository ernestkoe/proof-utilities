# Settings

FM_SDK_VERSION = 13

# Compile the plug-in in 32- and 64-bit mode


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

##############################################################################
# Determine the current platform

ifeq ($(shell uname -s),Darwin)
  platform := mac
else ifeq ($(shell uname -o),Cygwin)
  platform := win
endif

.PHONY: proof proof-32 proof-64

tahoma-c = \
  $(patsubst Example/%.cpp, %, $(wildcard Example/*.cpp) \
                               $(wildcard Example/Support/*.cpp))
tahoma_win_32_o = $(addprefix build/win/32/,$(addsuffix .obj,$(tahoma-c)))
tahoma_win_64_o = $(addprefix build/win/64/,$(addsuffix .obj,$(tahoma-c)))
tahoma_mac_o    = $(addprefix build/mac/,$(addsuffix .o  ,$(tahoma-c)))

tahoma_mac_b    = build/mac/Tahoma
tahoma_mac      = $(addprefix build/mac/Tahoma.fmplugin/Contents/, \
    Info.plist PkgInfo MacOS/Tahoma)

tahoma_mac_32_o = $(addprefix build/mac/32/,$(addsuffix .o  ,$(tahoma-c)))
tahoma_mac_64_o = $(addprefix build/mac/64/,$(addsuffix .o  ,$(tahoma-c)))

$(info $(tahoma_mac_o))

tahoma_win_32_res = build/win/32/resources.res
tahoma_win_64_res = build/win/64/resources.res
tahoma_win_res = build/win/FMPluginExample.res

tahoma_win_32 = build/win/32/Tahoma.fmx
tahoma_win_64 = build/win/64/Tahoma.fmx64

# Standard CPP compilation flags

CPPFLAGS += -IExample/Support -IPlugInSDK/Headers \
    -IPluginSDK/v$(FM_SDK_VERSION)/Headers/FMWrapper

# Pattern rule to build 32-bit Windows objects

build/win/32/%.obj build/win/64/%.obj: Example/%.cpp
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Compiling C++]$${z} "
	cl $(CPPFLAGS) -c -EHsc -Fo$@ -O1 $<

# -c    : compile only
# -EHsc : only catch C++ exceptions and assume that "extern C" functions
#         never throw one
# -Fo<FILE> : output file name
# -O1   : optimize for size

# Pattern rule to build Windows resources

build/win/32/%.res build/win/64/%.res: Example/Support/%.rc
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Compiling Windows resources]$${z} "
	rc -nologo -IExample -Fo$@ $<

build/win/32/%.res build/win/64/%.res: code/%.rc
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Compiling Windows resources]$${z} "
	rc -nologo -IExample -IPluginSDK/v$(FM_SDK_VERSION)/Headers/FMWrapper -Fo$@ $<
    

# Rule to build the Win32 version

build/win/32/Tahoma.fmx: $(tahoma_win_32_o) $(tahoma_win_32_res)
	@$(MAKE-PARENT-DIRECTORY?)
	$(MESSAGE) "$${g}[Linking Windows 32-bit version]$${z} "
	link -dll -machine:x86 -manifest:no $^ advapi32.lib user32.lib \
	PlugInSDK/Libraries/Win/win32/FMWrapper.lib -out:$@

# Rule to build the Win64 version

build/win/64/Tahoma.fmx64: $(tahoma_win_64_o) $(tahoma_win_64_res)
	@$(MAKE-PARENT-DIRECTORY?)
	$(MESSAGE) "$${g}[Linking Windows 64-bit version]$${z} "
	link -dll -machine:x64 -manifest:no $^ advapi32.lib user32.lib \
	PlugInSDK/Libraries/Win/x64/FMWrapper.lib -out:$@

##############################################################################
# Building Mac versions

build/mac/%.o: Example/%.cpp
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Compiling C++]$${z} "
	g++ $(CPPFLAGS) -arch i386 -arch x86_64 -c -Os \
        -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk -mmacosx-version-min=10.4 \
        -o$@ $<

build/mac/Tahoma: $(tahoma_mac_o)
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Linking Mac code]$${z} "
	g++ -arch i386 -arch x86_64 -bundle -FPluginSDK/Libraries/Mac \
        -framework FMWrapper -o$@ $^ \
        -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk -mmacosx-version-min=10.4

build/mac/Tahoma.fmplugin/Contents/PkgInfo: sources/PkgInfo
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Copying PkgInfo]$${z} "
	cp $< $@

build/mac/Tahoma.fmplugin/Contents/Info.plist: sources/Info.plist
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Copying InfoPlist.]$${z} "
	cp $< $@

build/mac/Tahoma.fmplugin/Contents/MacOS/Tahoma: build/mac/Tahoma
	@$(MAKE-PARENT-DIRECTORY?)
	@$(MESSAGE) "$${g}[Copying the Tahoma bundle binary.]$${z} "
	cp $< $@

##############################################################################
# Windows-specific settings

ifeq ($(platform),win)
  # Correct environment variables for the 32-bit compiler

  WIN-MVS = C:\Program Files (x86)\Microsoft Visual Studio 10.0
  WIN-SDK = C:\Program Files\Microsoft SDKs\Windows\v7.1

  # The INCLUDE variable is same for both 32- and 64-bit builds.

  build/win/32/% build/win/64/%: \
    export INCLUDE = $(WIN-MVS)\VC\include;$(WIN-SDK)\Include;

  # The APPVER (minimal target OS) is also same and I set it to Windows XP

  build/win/32/% build/win/64/%:
    export APPVER = 5.01

  # The LIB variable differs

  build/win/32/%: \
    export LIB := $(WIN-MVS)\VC\lib;$(WIN-SDK)\Lib;
  build/win/64/%: \
    export LIB := $(WIN-MVS)\VC\lib\amd64;$(WIN-SDK)\Lib\x64;

  # Finally I need to alter the PATH to point to right directories:

  WIN-PATH-COMMON = \
  $(shell cygpath "$(WIN-MVS)\Common7\IDE"):$(shell cygpath "$(WIN-MVS)\VC\vcpackages")

  build/win/32/%: export PATH := \
  $(shell cygpath "$(WIN-MVS)\VC\bin"):$(shell cygpath "$(WIN-SDK)\Bin"):$(WIN-PATH-COMMON):$(PATH)

  build/win/64/%: export PATH := \
  $(shell cygpath "$(WIN-MVS)\VC\bin\amd64"):$(shell cygpath "$(WIN-SDK)\Bin\x64"):$(WIN-PATH-COMMON):$(PATH)
endif


.PHONY: tahoma-win \
        tahoma-win-32 tahoma-win-32-o \
        tahoma-win-64 tahoma-win-64-o \
        tahoma-mac tahoma-mac-b tahoma-mac-o
tahoma-win: tahoma-win-32 tahoma-win-64
tahoma-win-32: $(tahoma_win_32)
tahoma-win-32-o: $(tahoma_win_32_o) $(tahoma_win_res)
tahoma-win-64: $(tahoma_win_64)
tahoma-win-64-o: $(tahoma_win_64_o) $(tahoma_win_res)
tahoma-mac:   $(tahoma_mac)
tahoma-mac-b: $(tahoma_mac_b)
tahoma-mac-o: $(tahoma_mac_o)


.PHONY: test

test:
	@echo Test

