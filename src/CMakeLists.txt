qt_add_executable(klog WIN32 MACOSX_BUNDLE
    aboutdialog.cpp aboutdialog.h
    adif.cpp adif.h
    awarddxmarathon.cpp awarddxmarathon.h
    awards.cpp awards.h
    awardswidget.cpp awardswidget.h
    charts/statscqzperyearbarchartwidget.cpp charts/statscqzperyearbarchartwidget.h
    charts/statsdxccsonsatswidget.cpp charts/statsdxccsonsatswidget.h
    charts/statsentitiesperyearbarchartwidget.cpp charts/statsentitiesperyearbarchartwidget.h
    charts/statsfieldperbandwidget.cpp charts/statsfieldperbandwidget.h
    charts/statsgeneralchartwidget.cpp charts/statsgeneralchartwidget.h
    charts/statsgridsonsatswidget.cpp charts/statsgridsonsatswidget.h
    charts/statsqsosperbandbarchartwidget.cpp charts/statsqsosperbandbarchartwidget.h
    charts/statsqsospercontinentbarchartwidget.cpp charts/statsqsospercontinentbarchartwidget.h
    charts/statsqsosperdxccbarchartwidget.cpp charts/statsqsosperdxccbarchartwidget.h
    charts/statsqsosperhourbarchartwidget.cpp charts/statsqsosperhourbarchartwidget.h
    charts/statsqsosperhourbarchartwidget.cpp charts/statsqsosperhourbarchartwidget.h
    charts/statsqsospermodebarchartwidget.cpp charts/statsqsospermodebarchartwidget.h
    charts/statsqsospermonthbarchartwidget.cpp charts/statsqsospermonthbarchartwidget.h
    charts/statsqsosperyearbarchartwidget.cpp charts/statsqsosperyearbarchartwidget.h
    charts/statssentconfirmedpiechartwidget.cpp charts/statssentconfirmedpiechartwidget.h
    charts/statsworkedconfirmedpiechartwidget.cpp charts/statsworkedconfirmedpiechartwidget.h
    charts/statsworkedsentpiechartwidget.cpp charts/statsworkedsentpiechartwidget.h
    database.cpp database.h
    database/db_adif_primary_subdvisions_data.cpp database/db_adif_primary_subdvisions_data.h
    database/queryexecutor.cpp database/queryexecutor.h
    dataproxy_sqlite.cpp dataproxy_sqlite.h
    downloadcty.cpp downloadcty.h
    dxccstatuswidget.cpp dxccstatuswidget.h
    dxcluster/dxcluster.cpp dxcluster/dxcluster.h
    dxcluster/dxclusterassistant.cpp dxcluster/dxclusterassistant.h
    dxcluster/dxspot.cpp dxcluster/dxspot.h
    elogclublog.cpp elogclublog.h
    elogqrzlog.cpp elogqrzlog.h
    eqslutilities.cpp eqslutilities.h
    fileawardmanager.cpp fileawardmanager.h
    filemanager.cpp filemanager.h
    frequency.cpp frequency.h
    global.h
    hamlibclass.cpp hamlibclass.h
    infowidget.cpp infowidget.h
    inputwidgets/mainwindowinputcomment.cpp inputwidgets/mainwindowinputcomment.h
    inputwidgets/mainwindowinputeqsl.cpp inputwidgets/mainwindowinputeqsl.h
    inputwidgets/mainwindowinputothers.cpp inputwidgets/mainwindowinputothers.h
    inputwidgets/mainwindowinputqsl.cpp inputwidgets/mainwindowinputqsl.h
    inputwidgets/mainwindowinputqso.cpp inputwidgets/mainwindowinputqso.h
    inputwidgets/mainwindowmydatatab.cpp inputwidgets/mainwindowmydatatab.h
    inputwidgets/mainwindowsattab.cpp inputwidgets/mainwindowsattab.h
    klogdefinitions.h
    locator.cpp locator.h
    logmodel.cpp logmodel.h
    logwindow.cpp logwindow.h
    lotwutilities.cpp lotwutilities.h
    main.cpp
    mainqsoentrywidget.cpp mainqsoentrywidget.h
    mainwindow.cpp mainwindow.h
    qso.cpp qso.h
    searchmodel.cpp searchmodel.h
    searchwidget.cpp searchwidget.h
    searchwindow.cpp searchwindow.h
    setupdialog.cpp setupdialog.h
    setuppages/hamlibnetworkconfigwidget.cpp setuppages/hamlibnetworkconfigwidget.h
    setuppages/hamlibserialconfigwidget.cpp setuppages/hamlibserialconfigwidget.h
    setuppages/setupentitydialog.cpp setuppages/setupentitydialog.h
    setuppages/setuppagebandmode.cpp setuppages/setuppagebandmode.h
    setuppages/setuppagedxcluster.cpp setuppages/setuppagedxcluster.h
    setuppages/setuppagecolors.cpp setuppages/setuppagecolors.h
    setuppages/setuppageelog.cpp setuppages/setuppageelog.h
    setuppages/setuppagehamlib.cpp setuppages/setuppagehamlib.h
    setuppages/setuppagelogs.cpp setuppages/setuppagelogs.h
    setuppages/setuppagelogsnew.cpp setuppages/setuppagelogsnew.h
    setuppages/setuppagelogview.cpp setuppages/setuppagelogview.h
    setuppages/setuppagemisc.cpp setuppages/setuppagemisc.h
    setuppages/setuppagesats.cpp setuppages/setuppagesats.h
    setuppages/setuppagesatsnew.cpp setuppages/setuppagesatsnew.h
    setuppages/setuppagesubdivisionnew.cpp setuppages/setuppagesubdivisionnew.h
    setuppages/setuppagesubdivisions.cpp setuppages/setuppagesubdivisions.h
    setuppages/setuppageudp.cpp setuppages/setuppageudp.h
    setuppages/setuppageuserdata.cpp setuppages/setuppageuserdata.h
    setuppages/setuppageworldeditor.cpp setuppages/setuppageworldeditor.h
    showerrordialog.cpp showerrordialog.h
    softwareupdate.cpp softwareupdate.h
    softwareupdatedialog.cpp softwareupdatedialog.h
    startwizard.cpp startwizard.h
    statisticswidget.cpp statisticswidget.h
    updatesatsdata.cpp updatesatsdata.h
    updatesettings.cpp updatesettings.h
    udpserver.cpp udpserver.h
    utilities.cpp utilities.h
    tipsdialog.cpp tipsdialog.h
    widgets/adiflotwexportwidget.cpp widgets/adiflotwexportwidget.h
    widgets/onlinemessagewidget.cpp widgets/onlinemessagewidget.h
    widgets/showadifimportwidget.cpp widgets/showadifimportwidget.h
    widgets/showkloglogwidget.cpp widgets/showkloglogwidget.h
    widgets/map/mapwidget.cpp widgets/map/mapwidget.h
    widgets/map/mapwindowwidget.cpp widgets/map/mapwindowwidget.h
    world.cpp world.h
)
target_compile_definitions(klog PRIVATE
    APP_NAME="KLog"
    APP_UNIX_NAME="klog"
    APP_VERSION="2.4.0"
    QT_DISABLE_DEPRECATED_UP_TO=0x050F00
    QT_STRICT_ITERATORS
    QT_USE_QSTRINGBUILDER
)

target_link_libraries(klog PRIVATE
    Qt::Charts
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Positioning
    Qt::PrintSupport
    Qt::QuickWidgets
    Qt::SerialPort
    Qt::Sql
    Qt::Widgets
)


# Resources:
set(klog_resource_files
    "flags/ad.png"
    "flags/ae.png"
    "flags/af.png"
    "flags/ag.png"
    "flags/ai.png"
    "flags/al.png"
    "flags/am.png"
    "flags/an.png"
    "flags/ao.png"
    "flags/ar.png"
    "flags/as.png"
    "flags/at.png"
    "flags/au.png"
    "flags/aw.png"
    "flags/ax.png"
    "flags/az.png"
    "flags/ba.png"
    "flags/balear.png"
    "flags/bb.png"
    "flags/bd.png"
    "flags/be.png"
    "flags/bf.png"
    "flags/bg.png"
    "flags/bh.png"
    "flags/bi.png"
    "flags/bj.png"
    "flags/bm.png"
    "flags/bn.png"
    "flags/bo.png"
    "flags/bq.png"
    "flags/br.png"
    "flags/bs.png"
    "flags/bt.png"
    "flags/bv.png"
    "flags/bw.png"
    "flags/by.png"
    "flags/bz.png"
    "flags/ca.png"
    "flags/canary.png"
    "flags/cc.png"
    "flags/cd.png"
    "flags/ceuta.png"
    "flags/cf.png"
    "flags/cg.png"
    "flags/ch.png"
    "flags/ci.png"
    "flags/ck.png"
    "flags/cl.png"
    "flags/cm.png"
    "flags/cn.png"
    "flags/co.png"
    "flags/cr.png"
    "flags/cs.png"
    "flags/cu.png"
    "flags/cv.png"
    "flags/cw.png"
    "flags/cx.png"
    "flags/cy.png"
    "flags/cz.png"
    "flags/de.png"
    "flags/dj.png"
    "flags/dk.png"
    "flags/dm.png"
    "flags/do.png"
    "flags/dz.png"
    "flags/ec.png"
    "flags/ee.png"
    "flags/eg.png"
    "flags/eh.png"
    "flags/england.png"
    "flags/er.png"
    "flags/es.png"
    "flags/et.png"
    "flags/europeanunion.png"
    "flags/fam.png"
    "flags/fi.png"
    "flags/fj.png"
    "flags/fk.png"
    "flags/fm.png"
    "flags/fo.png"
    "flags/fr.png"
    "flags/ga.png"
    "flags/gb.png"
    "flags/gd.png"
    "flags/ge.png"
    "flags/gf.png"
    "flags/gh.png"
    "flags/gi.png"
    "flags/gl.png"
    "flags/gm.png"
    "flags/gn.png"
    "flags/gp.png"
    "flags/gq.png"
    "flags/gr.png"
    "flags/gs.png"
    "flags/gt.png"
    "flags/gu.png"
    "flags/gw.png"
    "flags/gy.png"
    "flags/hk.png"
    "flags/hm.png"
    "flags/hn.png"
    "flags/hr.png"
    "flags/ht.png"
    "flags/hu.png"
    "flags/id.png"
    "flags/ie.png"
    "flags/il.png"
    "flags/in.png"
    "flags/io.png"
    "flags/iq.png"
    "flags/ir.png"
    "flags/is.png"
    "flags/it.png"
    "flags/jm.png"
    "flags/jo.png"
    "flags/jp.png"
    "flags/ke.png"
    "flags/kg.png"
    "flags/kh.png"
    "flags/ki.png"
    "flags/km.png"
    "flags/kn.png"
    "flags/kp.png"
    "flags/kr.png"
    "flags/kw.png"
    "flags/ky.png"
    "flags/kz.png"
    "flags/la.png"
    "flags/lb.png"
    "flags/lc.png"
    "flags/li.png"
    "flags/lk.png"
    "flags/lr.png"
    "flags/ls.png"
    "flags/lt.png"
    "flags/lu.png"
    "flags/lv.png"
    "flags/ly.png"
    "flags/ma.png"
    "flags/marquesas.png"
    "flags/mc.png"
    "flags/md.png"
    "flags/me.png"
    "flags/mg.png"
    "flags/mh.png"
    "flags/mk.png"
    "flags/ml.png"
    "flags/mm.png"
    "flags/mn.png"
    "flags/mo.png"
    "flags/mp.png"
    "flags/mq.png"
    "flags/mr.png"
    "flags/ms.png"
    "flags/mt.png"
    "flags/mu.png"
    "flags/mv.png"
    "flags/mw.png"
    "flags/mx.png"
    "flags/my.png"
    "flags/mz.png"
    "flags/na.png"
    "flags/nc.png"
    "flags/ne.png"
    "flags/nf.png"
    "flags/ng.png"
    "flags/ni.png"
    "flags/nl.png"
    "flags/no.png"
    "flags/northernireland.png"
    "flags/np.png"
    "flags/nr.png"
    "flags/nu.png"
    "flags/nz.png"
    "flags/om.png"
    "flags/pa.png"
    "flags/pe.png"
    "flags/pf.png"
    "flags/pg.png"
    "flags/ph.png"
    "flags/pk.png"
    "flags/pl.png"
    "flags/pm.png"
    "flags/pn.png"
    "flags/pr.png"
    "flags/ps.png"
    "flags/pt.png"
    "flags/pw.png"
    "flags/py.png"
    "flags/qa.png"
    "flags/re.png"
    "flags/ro.png"
    "flags/rs.png"
    "flags/ru.png"
    "flags/rw.png"
    "flags/sa.png"
    "flags/sardinia.png"
    "flags/sb.png"
    "flags/sc.png"
    "flags/scotland.png"
    "flags/sd.png"
    "flags/se.png"
    "flags/sg.png"
    "flags/sh.png"
    "flags/shm.png"
    "flags/si.png"
    "flags/sj.png"
    "flags/sk.png"
    "flags/sl.png"
    "flags/sm.png"
    "flags/sn.png"
    "flags/so.png"
    "flags/southbritish.png"
    "flags/sr.png"
    "flags/ss.png"
    "flags/st.png"
    "flags/sv.png"
    "flags/sy.png"
    "flags/sz.png"
    "flags/tc.png"
    "flags/td.png"
    "flags/tf.png"
    "flags/tg.png"
    "flags/th.png"
    "flags/tj.png"
    "flags/tk.png"
    "flags/tl.png"
    "flags/tm.png"
    "flags/tn.png"
    "flags/to.png"
    "flags/tr.png"
    "flags/tt.png"
    "flags/tv.png"
    "flags/tw.png"
    "flags/tz.png"
    "flags/ua.png"
    "flags/ug.png"
    "flags/um.png"
    "flags/un.png"
    "flags/us.png"
    "flags/uy.png"
    "flags/uz.png"
    "flags/va.png"
    "flags/vc.png"
    "flags/ve.png"
    "flags/vg.png"
    "flags/vi.png"
    "flags/vn.png"
    "flags/vu.png"
    "flags/wales.png"
    "flags/wf.png"
    "flags/ws.png"
    "flags/xk.png"
    "flags/ye.png"
    "flags/yt.png"
    "flags/za.png"
    "flags/zm.png"
    "flags/zw.png"
    "img/klog.ico"
    "img/klog_256x256.png"
    "img/klog_512x512.png"
    "img/klog_logo.png"
    "img/marker.png"
    "img/play.svg"
    "img/stop.svg"
    "qml/mapqmlfile.qml"
    "qml/marker.qml"
)

qt_add_resources(klog "klog"
    PREFIX
        "/"
    FILES
        ${klog_resource_files}
)

include(CMakePrintHelpers)
cmake_print_variables(PROJECT_SOURCE_DIR)

if(UNIX AND NOT APPLE)
    message (Unix and not Apple)
    target_compile_definitions(klog PRIVATE
        APP_LINUX
    )

    target_link_libraries(klog PRIVATE
        hamlib
    )
endif()

find_library (
    HAMLIB_LIB
    NAMES hamlib libhamlib libhamlib++ # what to look for
    #HINTS "${CMAKE_SOURCE_DIR}/lib" # where to look
    if (APPLE)
        HINTS "/usr/local/lib" # where to look
        message ("Lib: MacOS")
    endif
    if (WIN32)
        HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../libs/win64/hamlib/bin" # where to look
        message ("Lib: Win32")
    endif

    NO_DEFAULT_PATH # do not search system default paths
)

# check if we found the library
message(STATUS "HAMLIB_LIB: [${HAMLIB_LIB}]")

if (NOT HAMLIB_LIB)
    message(SEND_ERROR "Did not find lib hamlib")
endif()

#target_link_libraries(klog PRIVATE
#    ${HAMLIB_LIB}
#)

if(APPLE)
    message ("MacOS")

    target_include_directories(klog PRIVATE
        /usr/local/include/
    )

    target_link_libraries(klog PUBLIC
        # Remove: L"/usr/local/lib"
        ${HAMLIB_LIB}
        #hamlib
    )
endif()

if((TEST_architecture_arch STREQUAL "i386") AND WIN32)
    message ("Win32 i386")
    target_include_directories(klog PRIVATE
        ../../libs/win32/hamlib/include
    )

    target_link_libraries(klog PRIVATE
        # Remove: L"${CMAKE_CURRENT_SOURCE_DIR}/../../libs/win32/hamlib/bin"
        # Remove: L"${CMAKE_CURRENT_SOURCE_DIR}/../../libs/win32/hamlib/lib/gcc"
        hamlib
    )
endif()

if(WIN32 AND NOT (TEST_architecture_arch STREQUAL "i386"))
    message ("Win32 not i386")
    target_include_directories(klog PRIVATE
        ../../libs/win64/hamlib/include
    )

    target_link_libraries(klog PRIVATE
        # Remove: L"${CMAKE_CURRENT_SOURCE_DIR}/../../libs/win64/hamlib/bin"
        # Remove: L"${CMAKE_CURRENT_SOURCE_DIR}/../../libs/win64/hamlib/lib/gcc"
        hamlib
    )
endif()

install(TARGETS klog
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET klog
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
