### Generated by Winemaker


SRCDIR                = .
SUBDIRS               =
DLLS                  =
EXES                  = view3d



### Common settings

CEXTRA                = -mno-cygwin
CXXEXTRA              = -mno-cygwin
RCEXTRA               =
INCLUDE_PATH          =
DLL_PATH              =
LIBRARY_PATH          =
LIBRARIES             =


### view3d sources and settings

view3d_MODULE         = view3d
view3d_C_SRCS         = Src/DiCamera.c \
			Src/DiClip.c \
			Src/DiErr.c \
			Src/DiGeo.c \
			Src/DiMain.c \
			Src/DiMem.c \
			Src/DiMove.c \
			Src/DiParse.c \
			Src/DiRender.c \
			Src/DiScene.c \
			Src/DiTime.c \
			Src/DiTransf.c
view3d_CXX_SRCS       =
view3d_RC_SRCS        = Src/View.rc
view3d_LDFLAGS        = -mwindows \
			-mno-cygwin
view3d_DLL_PATH       =
view3d_DLLS           = odbc32 \
			ole32 \
			oleaut32 \
			winspool
view3d_LIBRARY_PATH   =
view3d_LIBRARIES      = uuid

view3d_OBJS           = $(view3d_C_SRCS:.c=.o) \
			$(view3d_CXX_SRCS:.cpp=.o) \
			$(view3d_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(view3d_C_SRCS)
CXX_SRCS              = $(view3d_CXX_SRCS)
RC_SRCS               = $(view3d_RC_SRCS)


### Tools

CC = winegcc
CXX = wineg++
RC = wrc


### Generic targets

all: $(SUBDIRS) $(DLLS:%=%.so) $(EXES:%=%.so)

### Build rules

.PHONY: all clean dummy

$(SUBDIRS): dummy
	@cd $@ && $(MAKE)

# Implicit rules

.SUFFIXES: .cpp .rc .res
DEFINCL = $(INCLUDE_PATH) $(DEFINES) $(OPTIONS)

.c.o:
	$(CC) -c $(CFLAGS) $(CEXTRA) $(DEFINCL) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.rc.res:
	$(RC) $(RCFLAGS) $(RCEXTRA) $(DEFINCL) -fo$@ $<

# Rules for cleaning

CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \
                  \\\#*\\\# *~ *% .\\\#*

clean:: $(SUBDIRS:%=%/__clean__) $(EXTRASUBDIRS:%=%/__clean__)
	$(RM) $(CLEAN_FILES) $(RC_SRCS:.rc=.res) $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)
	$(RM) $(DLLS:%=%.so) $(EXES:%=%.so) $(EXES:%.exe=%)

$(SUBDIRS:%=%/__clean__): dummy
	cd `dirname $@` && $(MAKE) clean

$(EXTRASUBDIRS:%=%/__clean__): dummy
	-cd `dirname $@` && $(RM) $(CLEAN_FILES)

### Target specific build rules
DEFLIB = $(LIBRARY_PATH) $(LIBRARIES) $(DLL_PATH)

$(view3d_MODULE).so: $(view3d_OBJS)
	$(CC) $(view3d_LDFLAGS) -o $@ $(view3d_OBJS) $(view3d_LIBRARY_PATH) $(DEFLIB) $(view3d_DLLS:%=-l%) $(view3d_LIBRARIES:%=-l%)


