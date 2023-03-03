QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    KMeans.cpp \
    color_selecter.cpp \
    curve_fitting.cpp \
    cutplane.cpp \
    dbscan_arg.cpp \
    filter_csf.cpp \
    filter_guass.cpp \
    filter_pt.cpp \
    filter_voxel.cpp \
    fitting.cpp \
    fix.cpp \
    fix_arg.cpp \
    main.cpp \
    mainwindow.cpp \
    pcl_function.cpp \
    tinyxml2.cpp \
    view_rendering.cpp

HEADERS += \
    DBSCAN.h \
    KMeans.h \
    changepoint.h \
    color_selecter.h \
    curve_fitting.h \
    cutplane.h \
    dbscan_arg.h \
    filter_csf.h \
    filter_guass.h \
    filter_pt.h \
    filter_voxel.h \
    fitting.h \
    fix.h \
    fix_arg.h \
    mainwindow.h \
    pcl_function.h \
    tinyxml2.h \
    view_rendering.h

FORMS += \
    cutplane.ui \
    dbscan_arg.ui \
    filter_csf.ui \
    filter_guass.ui \
    filter_pt.ui \
    filter_voxel.ui \
    fix_arg.ui \
    mainwindow.ui \
    view_rendering.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# ---------------------------- PCL 依赖 ----------------------------------
INCLUDEPATH += D:/PCL/PCL_1.11.1/include/pcl-1.11\
INCLUDEPATH += D:/PCL/PCL_1.11.1/bin\
INCLUDEPATH += D:/Boost\
INCLUDEPATH += D:/PCL/PCL_1.11.1/include/pcl-1.11/pcl\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/Boost/include/boost-1_64\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/Eigen/eigen3\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/FLANN/include\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/FLANN/include/flann\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/OpenNI2/Include\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/Qhull/include\
INCLUDEPATH += D:/PCL/PCL_1.11.1/3rdParty/VTK/include/vtk-8.0\
INCLUDEPATH += D:/VTK/VTK_8.2/include/vtk-8.2\
INCLUDEPATH += D:/VTK/VTK_8.2/bin\
INCLUDEPATH += D:/Lastools/LASlib/inc\
INCLUDEPATH += D:/Lastools/LASlib/src\
INCLUDEPATH += D:/Lastools/LASzip/src\
INCLUDEPATH += D:/CSF\
INCLUDEPATH += D:/CSF/\
INCLUDEPATH += D:/CSF/src\
INCLUDEPATH += D:/gdal/bin\
INCLUDEPATH += D:/gdal/include/gdal\
#INCLUDEPATH += D:/Tinyxml2/tinyxml2/include

LIBS += -LD:/Lastools/lib\
        -lLASlib
        -lLASlibD

CONFIG(debug,debug|release){

LIBS += -LD:/PCL/PCL_1.11.1/lib\
        -lpcl_commond\
        -lpcl_featuresd\
        -lpcl_filtersd\
        -lpcl_iod\
        -lpcl_io_plyd\
        -lpcl_kdtreed\
        -lpcl_keypointsd\
        -lpcl_mld\
        -lpcl_octreed\
        -lpcl_outofcored\
        -lpcl_peopled\
        -lpcl_recognitiond\
        -lpcl_registrationd\
        -lpcl_sample_consensusd\
        -lpcl_searchd\
        -lpcl_segmentationd\
        -lpcl_stereod\
        -lpcl_surfaced\
        -lpcl_trackingd\
        -lpcl_visualizationd\

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/Boost/lib\
        -llibboost_atomic-vc142-mt-gd-x64-1_74\
        -llibboost_bzip2-vc142-mt-gd-x64-1_74\
        -llibboost_chrono-vc142-mt-gd-x64-1_74\
        -llibboost_container-vc142-mt-gd-x64-1_74\
        -llibboost_context-vc142-mt-gd-x64-1_74\
        -llibboost_contract-vc142-mt-gd-x64-1_74\
        -llibboost_coroutine-vc142-mt-gd-x64-1_74\
        -llibboost_date_time-vc142-mt-gd-x64-1_74\
        -llibboost_exception-vc142-mt-gd-x64-1_74\
        -llibboost_fiber-vc142-mt-gd-x64-1_74\
        -llibboost_filesystem-vc142-mt-gd-x64-1_74\
        -llibboost_graph-vc142-mt-gd-x64-1_74\
        -llibboost_graph_parallel-vc142-mt-gd-x64-1_74\
        -llibboost_iostreams-vc142-mt-gd-x64-1_74\
        -llibboost_locale-vc142-mt-gd-x64-1_74\
        -llibboost_log-vc142-mt-gd-x64-1_74\
        -llibboost_log_setup-vc142-mt-gd-x64-1_74\
        -llibboost_math_c99-vc142-mt-gd-x64-1_74\
        -llibboost_math_c99f-vc142-mt-gd-x64-1_74\
        -llibboost_math_c99l-vc142-mt-gd-x64-1_74\
        -llibboost_math_tr1-vc142-mt-gd-x64-1_74\
        -llibboost_math_tr1f-vc142-mt-gd-x64-1_74\
        -llibboost_math_tr1l-vc142-mt-gd-x64-1_74\
        -llibboost_mpi-vc142-mt-gd-x64-1_74\
        -llibboost_nowide-vc142-mt-gd-x64-1_74\
        -llibboost_numpy38-vc142-mt-gd-x64-1_74\
        -llibboost_prg_exec_monitor-vc142-mt-gd-x64-1_74\
        -llibboost_program_options-vc142-mt-gd-x64-1_74\
        -llibboost_python38-vc142-mt-gd-x64-1_74\
        -llibboost_random-vc142-mt-gd-x64-1_74\
        -llibboost_regex-vc142-mt-gd-x64-1_74\
        -llibboost_serialization-vc142-mt-gd-x64-1_74\
        -llibboost_stacktrace_noop-vc142-mt-gd-x64-1_74\
        -llibboost_stacktrace_windbg-vc142-mt-gd-x64-1_74\
        -llibboost_stacktrace_windbg_cached-vc142-mt-gd-x64-1_74\
        -llibboost_system-vc142-mt-gd-x64-1_74\
        -llibboost_test_exec_monitor-vc142-mt-gd-x64-1_74\
        -llibboost_thread-vc142-mt-gd-x64-1_74\
        -llibboost_timer-vc142-mt-gd-x64-1_74\
        -llibboost_type_erasure-vc142-mt-gd-x64-1_74\
        -llibboost_unit_test_framework-vc142-mt-gd-x64-1_74\
        -llibboost_wave-vc142-mt-gd-x64-1_74\
        -llibboost_wserialization-vc142-mt-gd-x64-1_74\
        -llibboost_zlib-vc142-mt-gd-x64-1_74\

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/FLANN/lib\
        -lflann-gd\
        -lflann_cpp-gd\
        -lflann_cpp_s-gd\
        -lflann_s-gd

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/OpenNI2\
        -lOpenNI2

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/Qhull/lib\
        -lqhullcpp_d\
        -lqhullstatic_d\
        -lqhullstatic_r_d\
        -lqhull_d\
        -lqhull_p_d\
        -lqhull_r_d\

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/VTK/lib/Debug\
        -lQVTKWidgetPlugin-gd\
        -lvtkChartsCore-8.2-gd\
        -lvtkCommonColor-8.2-gd\
        -lvtkCommonComputationalGeometry-8.2-gd\
        -lvtkCommonCore-8.2-gd\
        -lvtkCommonDataModel-8.2-gd\
        -lvtkCommonExecutionModel-8.2-gd\
        -lvtkCommonMath-8.2-gd\
        -lvtkCommonMisc-8.2-gd\
        -lvtkCommonSystem-8.2-gd\
        -lvtkCommonTransforms-8.2-gd\
        -lvtkDICOMParser-8.2-gd\
        -lvtkDomainsChemistry-8.2-gd\
        -lvtkDomainsChemistryOpenGL2-8.2-gd\
        -lvtkdoubleconversion-8.2-gd\
        -lvtkexodusII-8.2-gd\
        -lvtkexpat-8.2-gd\
        -lvtkFiltersAMR-8.2-gd\
        -lvtkFiltersCore-8.2-gd\
        -lvtkFiltersExtraction-8.2-gd\
        -lvtkFiltersFlowPaths-8.2-gd\
        -lvtkFiltersGeneral-8.2-gd\
        -lvtkFiltersGeneric-8.2-gd\
        -lvtkFiltersGeometry-8.2-gd\
        -lvtkFiltersHybrid-8.2-gd\
        -lvtkFiltersHyperTree-8.2-gd\
        -lvtkFiltersImaging-8.2-gd\
        -lvtkFiltersModeling-8.2-gd\
        -lvtkFiltersParallel-8.2-gd\
        -lvtkFiltersParallelImaging-8.2-gd\
        -lvtkFiltersPoints-8.2-gd\
        -lvtkFiltersProgrammable-8.2-gd\
        -lvtkFiltersSelection-8.2-gd\
        -lvtkFiltersSMP-8.2-gd\
        -lvtkFiltersSources-8.2-gd\
        -lvtkFiltersStatistics-8.2-gd\
        -lvtkFiltersTexture-8.2-gd\
        -lvtkFiltersTopology-8.2-gd\
        -lvtkFiltersVerdict-8.2-gd\
        -lvtkfreetype-8.2-gd\
        -lvtkGeovisCore-8.2-gd\
        -lvtkgl2ps-8.2-gd\
        -lvtkglew-8.2-gd\
        -lvtkGUISupportQt-8.2-gd\
        -lvtkGUISupportQtSQL-8.2-gd\
        -lvtkhdf5-8.2-gd\
        -lvtkhdf5_hl-8.2-gd\
        -lvtkImagingColor-8.2-gd\
        -lvtkImagingCore-8.2-gd\
        -lvtkImagingFourier-8.2-gd\
        -lvtkImagingGeneral-8.2-gd\
        -lvtkImagingHybrid-8.2-gd\
        -lvtkImagingMath-8.2-gd\
        -lvtkImagingMorphological-8.2-gd\
        -lvtkImagingSources-8.2-gd\
        -lvtkImagingStatistics-8.2-gd\
        -lvtkImagingStencil-8.2-gd\
        -lvtkInfovisCore-8.2-gd\
        -lvtkInfovisLayout-8.2-gd\
        -lvtkInteractionImage-8.2-gd\
        -lvtkInteractionStyle-8.2-gd\
        -lvtkInteractionWidgets-8.2-gd\
        -lvtkIOAMR-8.2-gd\
        -lvtkIOAsynchronous-8.2-gd\
        -lvtkIOCityGML-8.2-gd\
        -lvtkIOCore-8.2-gd\
        -lvtkIOEnSight-8.2-gd\
        -lvtkIOExodus-8.2-gd\
        -lvtkIOExport-8.2-gd\
        -lvtkIOExportOpenGL2-8.2-gd\
        -lvtkIOExportPDF-8.2-gd\
        -lvtkIOGeometry-8.2-gd\
        -lvtkIOImage-8.2-gd\
        -lvtkIOImport-8.2-gd\
        -lvtkIOInfovis-8.2-gd\
        -lvtkIOLegacy-8.2-gd\
        -lvtkIOLSDyna-8.2-gd\
        -lvtkIOMINC-8.2-gd\
        -lvtkIOMovie-8.2-gd\
        -lvtkIONetCDF-8.2-gd\
        -lvtkIOParallel-8.2-gd\
        -lvtkIOParallelXML-8.2-gd\
        -lvtkIOPLY-8.2-gd\
        -lvtkIOSegY-8.2-gd\
        -lvtkIOSQL-8.2-gd\
        -lvtkIOTecplotTable-8.2-gd\
        -lvtkIOVeraOut-8.2-gd\
        -lvtkIOVideo-8.2-gd\
        -lvtkIOXML-8.2-gd\
        -lvtkIOXMLParser-8.2-gd\
        -lvtkjpeg-8.2-gd\
        -lvtkjsoncpp-8.2-gd\
        -lvtklibharu-8.2-gd\
        -lvtklibxml2-8.2-gd\
        -lvtklz4-8.2-gd\
        -lvtklzma-8.2-gd\
        -lvtkmetaio-8.2-gd\
        -lvtkNetCDF-8.2-gd\
        -lvtkogg-8.2-gd\
        -lvtkParallelCore-8.2-gd\
        -lvtkpng-8.2-gd\
        -lvtkproj-8.2-gd\
        -lvtkpugixml-8.2-gd\
        -lvtkRenderingAnnotation-8.2-gd\
        -lvtkRenderingContext2D-8.2-gd\
        -lvtkRenderingContextOpenGL2-8.2-gd\
        -lvtkRenderingCore-8.2-gd\
        -lvtkRenderingFreeType-8.2-gd\
        -lvtkRenderingGL2PSOpenGL2-8.2-gd\
        -lvtkRenderingImage-8.2-gd\
        -lvtkRenderingLabel-8.2-gd\
        -lvtkRenderingLOD-8.2-gd\
        -lvtkRenderingOpenGL2-8.2-gd\
        -lvtkRenderingQt-8.2-gd\
        -lvtkRenderingVolume-8.2-gd\
        -lvtkRenderingVolumeOpenGL2-8.2-gd\
        -lvtksqlite-8.2-gd\
        -lvtksys-8.2-gd\
        -lvtktheora-8.2-gd\
        -lvtktiff-8.2-gd\
        -lvtkverdict-8.2-gd\
        -lvtkViewsContext2D-8.2-gd\
        -lvtkViewsCore-8.2-gd\
        -lvtkViewsInfovis-8.2-gd\
        -lvtkViewsQt-8.2-gd\
        -lvtkzlib-8.2-gd\

#LIBS += -LD:/Tinyxml2/tinyxml2 -tinyxml2\

LIBS += -LD:/gdal/lib -lgdal_i_d\

} else {


LIBS += -LD:/PCL/PCL_1.11.1/lib\
        -lpcl_common\
        -lpcl_features\
        -lpcl_filters\
        -lpcl_io\
        -lpcl_io_ply\
        -lpcl_kdtree\
        -lpcl_keypoints\
        -lpcl_ml\
        -lpcl_octree\
        -lpcl_outofcore\
        -lpcl_people\
        -lpcl_recognition\
        -lpcl_registration\
        -lpcl_sample_consensus\
        -lpcl_search\
        -lpcl_segmentation\
        -lpcl_stereo\
        -lpcl_surface\
        -lpcl_tracking\
        -lpcl_visualization\

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/Boost/lib\
        -llibboost_atomic-vc142-mt-x64-1_74\
        -llibboost_bzip2-vc142-mt-x64-1_74\
        -llibboost_chrono-vc142-mt-x64-1_74\
        -llibboost_container-vc142-mt-x64-1_74\
        -llibboost_context-vc142-mt-x64-1_74\
        -llibboost_contract-vc142-mt-x64-1_74\
        -llibboost_coroutine-vc142-mt-x64-1_74\
        -llibboost_date_time-vc142-mt-x64-1_74\
        -llibboost_exception-vc142-mt-x64-1_74\
        -llibboost_fiber-vc142-mt-x64-1_74\
        -llibboost_filesystem-vc142-mt-x64-1_74\
        -llibboost_graph-vc142-mt-x64-1_74\
        -llibboost_graph_parallel-vc142-mt-x64-1_74\
        -llibboost_iostreams-vc142-mt-x64-1_74\
        -llibboost_locale-vc142-mt-x64-1_74\
        -llibboost_log-vc142-mt-x64-1_74\
        -llibboost_log_setup-vc142-mt-x64-1_74\
        -llibboost_math_c99-vc142-mt-x64-1_74\
        -llibboost_math_c99f-vc142-mt-x64-1_74\
        -llibboost_math_c99l-vc142-mt-x64-1_74\
        -llibboost_math_tr1-vc142-mt-x64-1_74\
        -llibboost_math_tr1f-vc142-mt-x64-1_74\
        -llibboost_math_tr1l-vc142-mt-x64-1_74\
        -llibboost_mpi-vc142-mt-x64-1_74\
        -llibboost_nowide-vc142-mt-x64-1_74\
        -llibboost_numpy38-vc142-mt-x64-1_74\
        -llibboost_prg_exec_monitor-vc142-mt-x64-1_74\
        -llibboost_program_options-vc142-mt-x64-1_74\
        -llibboost_python38-vc142-mt-x64-1_74\
        -llibboost_random-vc142-mt-x64-1_74\
        -llibboost_regex-vc142-mt-x64-1_74\
        -llibboost_serialization-vc142-mt-x64-1_74\
        -llibboost_stacktrace_noop-vc142-mt-x64-1_74\
        -llibboost_stacktrace_windbg-vc142-mt-x64-1_74\
        -llibboost_stacktrace_windbg_cached-vc142-mt-x64-1_74\
        -llibboost_system-vc142-mt-x64-1_74\
        -llibboost_test_exec_monitor-vc142-mt-x64-1_74\
        -llibboost_thread-vc142-mt-x64-1_74\
        -llibboost_timer-vc142-mt-x64-1_74\
        -llibboost_type_erasure-vc142-mt-x64-1_74\
        -llibboost_unit_test_framework-vc142-mt-x64-1_74\
        -llibboost_wave-vc142-mt-x64-1_74\
        -llibboost_wserialization-vc142-mt-x64-1_74\
        -llibboost_zlib-vc142-mt-x64-1_74\


LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/FLANN/lib\
        -lflann\
        -lflann_cpp\
        -lflann_cpp_s\
        -lflann_s

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/OpenNI2\
        -lOpenNI2

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/Qhull/lib\
        -lqhull\
        -lqhullcpp\
        -lqhullstatic\
        -lqhullstatic_r\
        -lqhull_p\
        -lqhull_r

LIBS += -LD:/PCL/PCL_1.11.1/3rdParty/VTK/lib/Release\
        -lQVTKWidgetPlugin\
        -lvtkChartsCore-8.2\
        -lvtkCommonColor-8.2\
        -lvtkCommonComputationalGeometry-8.2\
        -lvtkCommonCore-8.2\
        -lvtkCommonDataModel-8.2\
        -lvtkCommonExecutionModel-8.2\
        -lvtkCommonMath-8.2\
        -lvtkCommonMisc-8.2\
        -lvtkCommonSystem-8.2\
        -lvtkCommonTransforms-8.2\
        -lvtkDICOMParser-8.2\
        -lvtkDomainsChemistry-8.2\
        -lvtkDomainsChemistryOpenGL2-8.2\
        -lvtkdoubleconversion-8.2\
        -lvtkexodusII-8.2\
        -lvtkexpat-8.2\
        -lvtkFiltersAMR-8.2\
        -lvtkFiltersCore-8.2\
        -lvtkFiltersExtraction-8.2\
        -lvtkFiltersFlowPaths-8.2\
        -lvtkFiltersGeneral-8.2\
        -lvtkFiltersGeneric-8.2\
        -lvtkFiltersGeometry-8.2\
        -lvtkFiltersHybrid-8.2\
        -lvtkFiltersHyperTree-8.2\
        -lvtkFiltersImaging-8.2\
        -lvtkFiltersModeling-8.2\
        -lvtkFiltersParallel-8.2\
        -lvtkFiltersParallelImaging-8.2\
        -lvtkFiltersPoints-8.2\
        -lvtkFiltersProgrammable-8.2\
        -lvtkFiltersSelection-8.2\
        -lvtkFiltersSMP-8.2\
        -lvtkFiltersSources-8.2\
        -lvtkFiltersStatistics-8.2\
        -lvtkFiltersTexture-8.2\
        -lvtkFiltersTopology-8.2\
        -lvtkFiltersVerdict-8.2\
        -lvtkfreetype-8.2\
        -lvtkGeovisCore-8.2\
        -lvtkgl2ps-8.2\
        -lvtkglew-8.2\
        -lvtkGUISupportQt-8.2\
        -lvtkGUISupportQtSQL-8.2\
        -lvtkhdf5-8.2\
        -lvtkhdf5_hl-8.2\
        -lvtkImagingColor-8.2\
        -lvtkImagingCore-8.2\
        -lvtkImagingFourier-8.2\
        -lvtkImagingGeneral-8.2\
        -lvtkImagingHybrid-8.2\
        -lvtkImagingMath-8.2\
        -lvtkImagingMorphological-8.2\
        -lvtkImagingSources-8.2\
        -lvtkImagingStatistics-8.2\
        -lvtkImagingStencil-8.2\
        -lvtkInfovisCore-8.2\
        -lvtkInfovisLayout-8.2\
        -lvtkInteractionImage-8.2\
        -lvtkInteractionStyle-8.2\
        -lvtkInteractionWidgets-8.2\
        -lvtkIOAMR-8.2\
        -lvtkIOAsynchronous-8.2\
        -lvtkIOCityGML-8.2\
        -lvtkIOCore-8.2\
        -lvtkIOEnSight-8.2\
        -lvtkIOExodus-8.2\
        -lvtkIOExport-8.2\
        -lvtkIOExportOpenGL2-8.2\
        -lvtkIOExportPDF-8.2\
        -lvtkIOGeometry-8.2\
        -lvtkIOImage-8.2\
        -lvtkIOImport-8.2\
        -lvtkIOInfovis-8.2\
        -lvtkIOLegacy-8.2\
        -lvtkIOLSDyna-8.2\
        -lvtkIOMINC-8.2\
        -lvtkIOMovie-8.2\
        -lvtkIONetCDF-8.2\
        -lvtkIOParallel-8.2\
        -lvtkIOParallelXML-8.2\
        -lvtkIOPLY-8.2\
        -lvtkIOSegY-8.2\
        -lvtkIOSQL-8.2\
        -lvtkIOTecplotTable-8.2\
        -lvtkIOVeraOut-8.2\
        -lvtkIOVideo-8.2\
        -lvtkIOXML-8.2\
        -lvtkIOXMLParser-8.2\
        -lvtkjpeg-8.2\
        -lvtkjsoncpp-8.2\
        -lvtklibharu-8.2\
        -lvtklibxml2-8.2\
        -lvtklz4-8.2\
        -lvtklzma-8.2\
        -lvtkmetaio-8.2\
        -lvtkNetCDF-8.2\
        -lvtkogg-8.2\
        -lvtkParallelCore-8.2\
        -lvtkpng-8.2\
        -lvtkproj-8.2\
        -lvtkpugixml-8.2\
        -lvtkRenderingAnnotation-8.2\
        -lvtkRenderingContext2D-8.2\
        -lvtkRenderingContextOpenGL2-8.2\
        -lvtkRenderingCore-8.2\
        -lvtkRenderingFreeType-8.2\
        -lvtkRenderingGL2PSOpenGL2-8.2\
        -lvtkRenderingImage-8.2\
        -lvtkRenderingLabel-8.2\
        -lvtkRenderingLOD-8.2\
        -lvtkRenderingOpenGL2-8.2\
        -lvtkRenderingQt-8.2\
        -lvtkRenderingVolume-8.2\
        -lvtkRenderingVolumeOpenGL2-8.2\
        -lvtksqlite-8.2\
        -lvtksys-8.2\
        -lvtktheora-8.2\
        -lvtktiff-8.2\
        -lvtkverdict-8.2\
        -lvtkViewsContext2D-8.2\
        -lvtkViewsCore-8.2\
        -lvtkViewsInfovis-8.2\
        -lvtkViewsQt-8.2\
        -lvtkzlib-8.2\

LIBS += -LD:/CSF/build/src/Release -lCSF\

LIBS += -LD:/gdal/lib -lgdal_i\

#LIBS += -LD:/Tinyxml2/tinyxml2 -tinyxml2\
}

RESOURCES += \
    Icons.qrc

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world460
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world460d
else:unix: LIBS += -LD:/OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world460

INCLUDEPATH += D:/OpenCV/opencv/build/include
DEPENDPATH += D:/OpenCV/opencv/build/include
