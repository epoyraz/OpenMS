### the directory name
set(directory include/OpenMS/APPLICATIONS)

### list all MOC filenames of the directory here
set(sources_list
)

### add path to the filenames
set(sources)
foreach(i ${sources_list})
  list(APPEND sources ${directory}/${i})
endforeach(i)

### Apply MOC compiler
QT4_WRAP_CPP(mocced_sources ${sources} OPTIONS ${BOOST_MOC_ARGS})

### pass source file list to the upper instance
set(OpenMS_sources ${OpenMS_sources} ${mocced_sources})

source_group("Source Files\\OpenMS\\APPLICATIONS" FILES ${mocced_sources})

### list all header files of the directory here
set(sources_list_h
INIUpdater.h
ToolHandler.h
TOPPBase.h
ParameterInformation.h
ConsoleUtils.h
)

### add path to the filenames
set(sources_h)
foreach(i ${sources_list_h})
	list(APPEND sources_h ${directory}/${i})
endforeach(i)

### source group definition
source_group("Header Files\\OpenMS\\APPLICATIONS" FILES ${sources_h})

set(OpenMS_sources_h ${OpenMS_sources_h} ${sources_h})

