TOP=../..

# see https://github.com/kasemir/epics-train-2021/blob/main/examples/17_deviceSupportApp/src/Makefile

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = piFace
# piFace.dbd will be created and installed
DBD += piFace.dbd

# Build an IOC support library
LIBRARY_IOC += piFaceSupport

# Compile and add code to the support library
piFaceSupport_SRCS += devMbboDirectPiFace.c
piFaceSupport_SRCS += drvPiFace.c

# piFace.dbd will be made up from these files:
piFace_DBD += base.dbd

# Include dbd files from all support applications:
#piFace_DBD += xxx.dbd
piFace_DBD += devPiFace.dbd


# Install .dbd into <top>/dbd
#DBD += devPiFace.dbd

# Add all the support libraries needed by this IOC
piFace_LIBS += piFaceSupport

# piFace_registerRecordDeviceDriver.cpp derives from piFace.dbd
piFace_SRCS += piFace_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
piFace_SRCS_DEFAULT += piFaceMain.cpp
piFace_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#piFace_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link QSRV (pvAccess Server) if available
ifdef EPICS_QSRV_MAJOR_VERSION
    piFace_LIBS += qsrv
    piFace_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    piFace_DBD += PVAServerRegister.dbd
    piFace_DBD += qsrv.dbd
endif


# Finally link to the EPICS Base libraries
piFace_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

