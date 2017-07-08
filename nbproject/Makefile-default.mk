#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/glbase.p1 ${OBJECTDIR}/glcd.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/rtc.p1 ${OBJECTDIR}/spi.p1 ${OBJECTDIR}/wwv.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/glbase.p1.d ${OBJECTDIR}/glcd.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/rtc.p1.d ${OBJECTDIR}/spi.p1.d ${OBJECTDIR}/wwv.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/glbase.p1 ${OBJECTDIR}/glcd.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/rtc.p1 ${OBJECTDIR}/spi.p1 ${OBJECTDIR}/wwv.p1


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F2420
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/glbase.p1: glbase.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 glbase.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  glbase.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/glbase.p1: > ${OBJECTDIR}/glbase.p1.d
	@cat ${OBJECTDIR}/glbase.dep >> ${OBJECTDIR}/glbase.p1.d
	@${FIXDEPS} "${OBJECTDIR}/glbase.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/glcd.p1: glcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 glcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  glcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/glcd.p1: > ${OBJECTDIR}/glcd.p1.d
	@cat ${OBJECTDIR}/glcd.dep >> ${OBJECTDIR}/glcd.p1.d
	@${FIXDEPS} "${OBJECTDIR}/glcd.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/main.p1: > ${OBJECTDIR}/main.p1.d
	@cat ${OBJECTDIR}/main.dep >> ${OBJECTDIR}/main.p1.d
	@${FIXDEPS} "${OBJECTDIR}/main.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/rtc.p1: rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 rtc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  rtc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/rtc.p1: > ${OBJECTDIR}/rtc.p1.d
	@cat ${OBJECTDIR}/rtc.dep >> ${OBJECTDIR}/rtc.p1.d
	@${FIXDEPS} "${OBJECTDIR}/rtc.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/spi.p1: spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 spi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  spi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/spi.p1: > ${OBJECTDIR}/spi.p1.d
	@cat ${OBJECTDIR}/spi.dep >> ${OBJECTDIR}/spi.p1.d
	@${FIXDEPS} "${OBJECTDIR}/spi.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/wwv.p1: wwv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 wwv.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  wwv.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/wwv.p1: > ${OBJECTDIR}/wwv.p1.d
	@cat ${OBJECTDIR}/wwv.dep >> ${OBJECTDIR}/wwv.p1.d
	@${FIXDEPS} "${OBJECTDIR}/wwv.p1.d" $(SILENT) -ht
	
else
${OBJECTDIR}/glbase.p1: glbase.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 glbase.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  glbase.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/glbase.p1: > ${OBJECTDIR}/glbase.p1.d
	@cat ${OBJECTDIR}/glbase.dep >> ${OBJECTDIR}/glbase.p1.d
	@${FIXDEPS} "${OBJECTDIR}/glbase.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/glcd.p1: glcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 glcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  glcd.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/glcd.p1: > ${OBJECTDIR}/glcd.p1.d
	@cat ${OBJECTDIR}/glcd.dep >> ${OBJECTDIR}/glcd.p1.d
	@${FIXDEPS} "${OBJECTDIR}/glcd.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/main.p1: > ${OBJECTDIR}/main.p1.d
	@cat ${OBJECTDIR}/main.dep >> ${OBJECTDIR}/main.p1.d
	@${FIXDEPS} "${OBJECTDIR}/main.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/rtc.p1: rtc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 rtc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  rtc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/rtc.p1: > ${OBJECTDIR}/rtc.p1.d
	@cat ${OBJECTDIR}/rtc.dep >> ${OBJECTDIR}/rtc.p1.d
	@${FIXDEPS} "${OBJECTDIR}/rtc.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/spi.p1: spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 spi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  spi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/spi.p1: > ${OBJECTDIR}/spi.p1.d
	@cat ${OBJECTDIR}/spi.dep >> ${OBJECTDIR}/spi.p1.d
	@${FIXDEPS} "${OBJECTDIR}/spi.p1.d" $(SILENT) -ht
	
${OBJECTDIR}/wwv.p1: wwv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	${MP_CC} --pass1 wwv.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@${MP_CC} --scandep  wwv.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir=${OBJECTDIR} -N31 --warn=0 --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 --cp=16 -Blarge --double=24  --mode=lite -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s"  
	@echo ${OBJECTDIR}/wwv.p1: > ${OBJECTDIR}/wwv.p1.d
	@cat ${OBJECTDIR}/wwv.dep >> ${OBJECTDIR}/wwv.p1.d
	@${FIXDEPS} "${OBJECTDIR}/wwv.p1.d" $(SILENT) -ht
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -D__DEBUG --debugger=realice -N31 --warn=0 --cp=16 -Blarge --double=24  --mode=lite  --output=default,-inhx032 -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s" ${OBJECTFILES_QUOTED_IF_SPACED}    
	${RM} dist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.hex
else
dist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/wwvb2.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+stackwarn,-config,+clib,-plib --opt=all,+asm,-asmfile,-speed,+space,9 -N31 --warn=0 --cp=16 -Blarge --double=24  --mode=lite  --output=default,-inhx032 -g --asmlist "--errformat=%%f:%%l: error: %%s" "--msgformat=%%f:%%l: advisory: %%s" "--warnformat=%%f:%%l warning: %%s" ${OBJECTFILES_QUOTED_IF_SPACED}    
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
