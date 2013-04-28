##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=omega
ConfigurationName      :=Release
WorkspacePath          := "/home/ephelps/.codelite/analysis"
ProjectPath            := "/home/ephelps/.codelite/analysis/omega"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Evan Phelps
Date                   :=03/19/2013
CodeLitePath           :="/home/ephelps/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="/home/ephelps/.codelite/analysis/omega/omega.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -fPIC -m64
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)$(ROOTSYS)/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Gui $(LibrarySwitch)Core $(LibrarySwitch)Cint $(LibrarySwitch)RIO $(LibrarySwitch)Net $(LibrarySwitch)Hist $(LibrarySwitch)Graf $(LibrarySwitch)Graf3d $(LibrarySwitch)Gpad $(LibrarySwitch)Tree $(LibrarySwitch)Rint $(LibrarySwitch)Postscript $(LibrarySwitch)Matrix $(LibrarySwitch)Physics $(LibrarySwitch)MathCore $(LibrarySwitch)Thread $(LibrarySwitch)m $(LibrarySwitch)dl 
ArLibs                 :=  "Gui" "Core" "Cint" "RIO" "Net" "Hist" "Graf" "Graf3d" "Gpad" "Tree" "Rint" "Postscript" "Matrix" "Physics" "MathCore" "Thread" "m" "dl" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)$(ROOTSYS)/lib 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -fPIC -O2 -Wall -pthread -m64 $(Preprocessors)
CFLAGS   :=  -fPIC -O2 -Wall -pthread -m64 $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
ROOTSYS:=/opt/cernroot/root-5.34.04
LD_LIBRARY_PATH:=$(ROOTSYS)/lib
Objects=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/h10exp$(ObjectSuffix) $(IntermediateDirectory)/h10sim$(ObjectSuffix) $(IntermediateDirectory)/config-data$(ObjectSuffix) $(IntermediateDirectory)/eid$(ObjectSuffix) $(IntermediateDirectory)/fid$(ObjectSuffix) $(IntermediateDirectory)/data$(ObjectSuffix) $(IntermediateDirectory)/data-handler$(ObjectSuffix) $(IntermediateDirectory)/dh-3pi-to-phys-frm$(ObjectSuffix) $(IntermediateDirectory)/dh-calc-luminosity$(ObjectSuffix) \
	$(IntermediateDirectory)/dh-eid$(ObjectSuffix) $(IntermediateDirectory)/dh-fid$(ObjectSuffix) $(IntermediateDirectory)/dh-skim-charge$(ObjectSuffix) $(IntermediateDirectory)/dh-skim-charged-parts$(ObjectSuffix) $(IntermediateDirectory)/dh-skim-mass$(ObjectSuffix) $(IntermediateDirectory)/dh-tm-3pi$(ObjectSuffix) $(IntermediateDirectory)/dh-tm-h10clone$(ObjectSuffix) $(IntermediateDirectory)/handler-chain$(ObjectSuffix) $(IntermediateDirectory)/h10looper$(ObjectSuffix) $(IntermediateDirectory)/evt$(ObjectSuffix) \
	$(IntermediateDirectory)/mom_corr$(ObjectSuffix) $(IntermediateDirectory)/acc$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/main.cpp"

$(IntermediateDirectory)/h10exp$(ObjectSuffix): h10exp.cpp $(IntermediateDirectory)/h10exp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/h10exp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/h10exp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/h10exp$(DependSuffix): h10exp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/h10exp$(ObjectSuffix) -MF$(IntermediateDirectory)/h10exp$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/h10exp.cpp"

$(IntermediateDirectory)/h10exp$(PreprocessSuffix): h10exp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/h10exp$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/h10exp.cpp"

$(IntermediateDirectory)/h10sim$(ObjectSuffix): h10sim.cpp $(IntermediateDirectory)/h10sim$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/h10sim.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/h10sim$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/h10sim$(DependSuffix): h10sim.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/h10sim$(ObjectSuffix) -MF$(IntermediateDirectory)/h10sim$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/h10sim.cpp"

$(IntermediateDirectory)/h10sim$(PreprocessSuffix): h10sim.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/h10sim$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/h10sim.cpp"

$(IntermediateDirectory)/config-data$(ObjectSuffix): config-data.cpp $(IntermediateDirectory)/config-data$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/config-data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config-data$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config-data$(DependSuffix): config-data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/config-data$(ObjectSuffix) -MF$(IntermediateDirectory)/config-data$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/config-data.cpp"

$(IntermediateDirectory)/config-data$(PreprocessSuffix): config-data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config-data$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/config-data.cpp"

$(IntermediateDirectory)/eid$(ObjectSuffix): eid.cpp $(IntermediateDirectory)/eid$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/eid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eid$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eid$(DependSuffix): eid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/eid$(ObjectSuffix) -MF$(IntermediateDirectory)/eid$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/eid.cpp"

$(IntermediateDirectory)/eid$(PreprocessSuffix): eid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eid$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/eid.cpp"

$(IntermediateDirectory)/fid$(ObjectSuffix): fid.cpp $(IntermediateDirectory)/fid$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/fid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fid$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fid$(DependSuffix): fid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fid$(ObjectSuffix) -MF$(IntermediateDirectory)/fid$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/fid.cpp"

$(IntermediateDirectory)/fid$(PreprocessSuffix): fid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fid$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/fid.cpp"

$(IntermediateDirectory)/data$(ObjectSuffix): data.cpp $(IntermediateDirectory)/data$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data$(DependSuffix): data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data$(ObjectSuffix) -MF$(IntermediateDirectory)/data$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/data.cpp"

$(IntermediateDirectory)/data$(PreprocessSuffix): data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/data.cpp"

$(IntermediateDirectory)/data-handler$(ObjectSuffix): data-handler.cpp $(IntermediateDirectory)/data-handler$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/data-handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/data-handler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/data-handler$(DependSuffix): data-handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/data-handler$(ObjectSuffix) -MF$(IntermediateDirectory)/data-handler$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/data-handler.cpp"

$(IntermediateDirectory)/data-handler$(PreprocessSuffix): data-handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/data-handler$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/data-handler.cpp"

$(IntermediateDirectory)/dh-3pi-to-phys-frm$(ObjectSuffix): dh-3pi-to-phys-frm.cpp $(IntermediateDirectory)/dh-3pi-to-phys-frm$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-3pi-to-phys-frm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-3pi-to-phys-frm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-3pi-to-phys-frm$(DependSuffix): dh-3pi-to-phys-frm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-3pi-to-phys-frm$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-3pi-to-phys-frm$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-3pi-to-phys-frm.cpp"

$(IntermediateDirectory)/dh-3pi-to-phys-frm$(PreprocessSuffix): dh-3pi-to-phys-frm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-3pi-to-phys-frm$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-3pi-to-phys-frm.cpp"

$(IntermediateDirectory)/dh-calc-luminosity$(ObjectSuffix): dh-calc-luminosity.cpp $(IntermediateDirectory)/dh-calc-luminosity$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-calc-luminosity.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-calc-luminosity$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-calc-luminosity$(DependSuffix): dh-calc-luminosity.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-calc-luminosity$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-calc-luminosity$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-calc-luminosity.cpp"

$(IntermediateDirectory)/dh-calc-luminosity$(PreprocessSuffix): dh-calc-luminosity.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-calc-luminosity$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-calc-luminosity.cpp"

$(IntermediateDirectory)/dh-eid$(ObjectSuffix): dh-eid.cpp $(IntermediateDirectory)/dh-eid$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-eid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-eid$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-eid$(DependSuffix): dh-eid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-eid$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-eid$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-eid.cpp"

$(IntermediateDirectory)/dh-eid$(PreprocessSuffix): dh-eid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-eid$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-eid.cpp"

$(IntermediateDirectory)/dh-fid$(ObjectSuffix): dh-fid.cpp $(IntermediateDirectory)/dh-fid$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-fid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-fid$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-fid$(DependSuffix): dh-fid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-fid$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-fid$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-fid.cpp"

$(IntermediateDirectory)/dh-fid$(PreprocessSuffix): dh-fid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-fid$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-fid.cpp"

$(IntermediateDirectory)/dh-skim-charge$(ObjectSuffix): dh-skim-charge.cpp $(IntermediateDirectory)/dh-skim-charge$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-skim-charge.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-skim-charge$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-skim-charge$(DependSuffix): dh-skim-charge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-skim-charge$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-skim-charge$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-skim-charge.cpp"

$(IntermediateDirectory)/dh-skim-charge$(PreprocessSuffix): dh-skim-charge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-skim-charge$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-skim-charge.cpp"

$(IntermediateDirectory)/dh-skim-charged-parts$(ObjectSuffix): dh-skim-charged-parts.cpp $(IntermediateDirectory)/dh-skim-charged-parts$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-skim-charged-parts.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-skim-charged-parts$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-skim-charged-parts$(DependSuffix): dh-skim-charged-parts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-skim-charged-parts$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-skim-charged-parts$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-skim-charged-parts.cpp"

$(IntermediateDirectory)/dh-skim-charged-parts$(PreprocessSuffix): dh-skim-charged-parts.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-skim-charged-parts$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-skim-charged-parts.cpp"

$(IntermediateDirectory)/dh-skim-mass$(ObjectSuffix): dh-skim-mass.cpp $(IntermediateDirectory)/dh-skim-mass$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-skim-mass.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-skim-mass$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-skim-mass$(DependSuffix): dh-skim-mass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-skim-mass$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-skim-mass$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-skim-mass.cpp"

$(IntermediateDirectory)/dh-skim-mass$(PreprocessSuffix): dh-skim-mass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-skim-mass$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-skim-mass.cpp"

$(IntermediateDirectory)/dh-tm-3pi$(ObjectSuffix): dh-tm-3pi.cpp $(IntermediateDirectory)/dh-tm-3pi$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-tm-3pi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-tm-3pi$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-tm-3pi$(DependSuffix): dh-tm-3pi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-tm-3pi$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-tm-3pi$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-tm-3pi.cpp"

$(IntermediateDirectory)/dh-tm-3pi$(PreprocessSuffix): dh-tm-3pi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-tm-3pi$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-tm-3pi.cpp"

$(IntermediateDirectory)/dh-tm-h10clone$(ObjectSuffix): dh-tm-h10clone.cpp $(IntermediateDirectory)/dh-tm-h10clone$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/dh-tm-h10clone.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dh-tm-h10clone$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dh-tm-h10clone$(DependSuffix): dh-tm-h10clone.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dh-tm-h10clone$(ObjectSuffix) -MF$(IntermediateDirectory)/dh-tm-h10clone$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/dh-tm-h10clone.cpp"

$(IntermediateDirectory)/dh-tm-h10clone$(PreprocessSuffix): dh-tm-h10clone.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dh-tm-h10clone$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/dh-tm-h10clone.cpp"

$(IntermediateDirectory)/handler-chain$(ObjectSuffix): handler-chain.cpp $(IntermediateDirectory)/handler-chain$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/handler-chain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/handler-chain$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/handler-chain$(DependSuffix): handler-chain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/handler-chain$(ObjectSuffix) -MF$(IntermediateDirectory)/handler-chain$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/handler-chain.cpp"

$(IntermediateDirectory)/handler-chain$(PreprocessSuffix): handler-chain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/handler-chain$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/handler-chain.cpp"

$(IntermediateDirectory)/h10looper$(ObjectSuffix): h10looper.cpp $(IntermediateDirectory)/h10looper$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/h10looper.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/h10looper$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/h10looper$(DependSuffix): h10looper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/h10looper$(ObjectSuffix) -MF$(IntermediateDirectory)/h10looper$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/h10looper.cpp"

$(IntermediateDirectory)/h10looper$(PreprocessSuffix): h10looper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/h10looper$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/h10looper.cpp"

$(IntermediateDirectory)/evt$(ObjectSuffix): evt.cpp $(IntermediateDirectory)/evt$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/evt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/evt$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/evt$(DependSuffix): evt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/evt$(ObjectSuffix) -MF$(IntermediateDirectory)/evt$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/evt.cpp"

$(IntermediateDirectory)/evt$(PreprocessSuffix): evt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/evt$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/evt.cpp"

$(IntermediateDirectory)/mom_corr$(ObjectSuffix): mom_corr.cpp $(IntermediateDirectory)/mom_corr$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/mom_corr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mom_corr$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mom_corr$(DependSuffix): mom_corr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mom_corr$(ObjectSuffix) -MF$(IntermediateDirectory)/mom_corr$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/mom_corr.cpp"

$(IntermediateDirectory)/mom_corr$(PreprocessSuffix): mom_corr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mom_corr$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/mom_corr.cpp"

$(IntermediateDirectory)/acc$(ObjectSuffix): acc.cpp $(IntermediateDirectory)/acc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ephelps/.codelite/analysis/omega/acc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/acc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/acc$(DependSuffix): acc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/acc$(ObjectSuffix) -MF$(IntermediateDirectory)/acc$(DependSuffix) -MM "/home/ephelps/.codelite/analysis/omega/acc.cpp"

$(IntermediateDirectory)/acc$(PreprocessSuffix): acc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/acc$(PreprocessSuffix) "/home/ephelps/.codelite/analysis/omega/acc.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/h10exp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/h10exp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/h10exp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/h10sim$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/h10sim$(DependSuffix)
	$(RM) $(IntermediateDirectory)/h10sim$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/config-data$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/config-data$(DependSuffix)
	$(RM) $(IntermediateDirectory)/config-data$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/eid$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/eid$(DependSuffix)
	$(RM) $(IntermediateDirectory)/eid$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/fid$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/fid$(DependSuffix)
	$(RM) $(IntermediateDirectory)/fid$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/data$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/data$(DependSuffix)
	$(RM) $(IntermediateDirectory)/data$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/data-handler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/data-handler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/data-handler$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-3pi-to-phys-frm$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-3pi-to-phys-frm$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-3pi-to-phys-frm$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-calc-luminosity$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-calc-luminosity$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-calc-luminosity$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-eid$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-eid$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-eid$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-fid$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-fid$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-fid$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-charge$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-charge$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-charge$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-charged-parts$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-charged-parts$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-charged-parts$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-mass$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-mass$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-skim-mass$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-tm-3pi$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-tm-3pi$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-tm-3pi$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dh-tm-h10clone$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dh-tm-h10clone$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dh-tm-h10clone$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/handler-chain$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/handler-chain$(DependSuffix)
	$(RM) $(IntermediateDirectory)/handler-chain$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/h10looper$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/h10looper$(DependSuffix)
	$(RM) $(IntermediateDirectory)/h10looper$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/evt$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/evt$(DependSuffix)
	$(RM) $(IntermediateDirectory)/evt$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mom_corr$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mom_corr$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mom_corr$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/acc$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/acc$(DependSuffix)
	$(RM) $(IntermediateDirectory)/acc$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/ephelps/.codelite/analysis/.build-release/omega"


