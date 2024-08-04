qt_add_executable(klog WIN32 MACOSX_BUNDLE
    aboutdialog.cpp aboutdialog.h
    adif.cpp adif.h
    awarddxmarathon.h
    awards.h
    awardswidget.cpp awardswidget.h
    charts/statscqzperyearbarchartwidget.h
    charts/statsdxccsonsatswidget.h
    charts/statsentitiesperyearbarchartwidget.h
    charts/statsfieldperbandwidget.cpp charts/statsfieldperbandwidget.h
    charts/statsgeneralchartwidget.h
    charts/statsgridsonsatswidget.h
    charts/statsqsosperbandbarchartwidget.h
    charts/statsqsospercontinentbarchartwidget.h
    charts/statsqsosperdxccbarchartwidget.h
    charts/statsqsosperhourbarchartwidget.h
    charts/statsqsospermodebarchartwidget.h
    charts/statsqsospermonthbarchartwidget.h
    charts/statsqsosperyearbarchartwidget.h
    charts/statssentconfirmedpiechartwidget.h
    charts/statsworkedconfirmedpiechartwidget.h
    charts/statsworkedsentpiechartwidget.h
    database.h
    database/db_adif_primary_subdvisions_data.cpp database/db_adif_primary_subdvisions_data.h
    database/queryexecutor.cpp database/queryexecutor.h
    dataproxy_sqlite.h
    downloadcty.h
    dxccstatuswidget.h
    dxcluster/dxcluster.cpp dxcluster/dxcluster.h
    dxcluster/dxclusterassistant.cpp dxcluster/dxclusterassistant.h
    dxcluster/dxspot.cpp dxcluster/dxspot.h
    elogqrzlog.cpp elogqrzlog.h
    eqslutilities.cpp eqslutilities.h
    frequency.cpp frequency.h
    global.h
    hamlibclass.h
    inputwidgets/mainwindowinputcomment.cpp inputwidgets/mainwindowinputcomment.h
    inputwidgets/mainwindowinputeqsl.cpp inputwidgets/mainwindowinputeqsl.h
    inputwidgets/mainwindowinputothers.cpp inputwidgets/mainwindowinputothers.h
    inputwidgets/mainwindowinputqsl.cpp inputwidgets/mainwindowinputqsl.h
    inputwidgets/mainwindowinputqso.cpp inputwidgets/mainwindowinputqso.h
    inputwidgets/mainwindowmydatatab.cpp inputwidgets/mainwindowmydatatab.h
    inputwidgets/mainwindowsattab.cpp inputwidgets/mainwindowsattab.h
    klogdefinitions.h
    lotwutilities.cpp lotwutilities.h
    main.cpp
    mainqsoentrywidget.cpp mainqsoentrywidget.h
    mainwindow.cpp mainwindow.h
    qso.h
    searchmodel.cpp searchmodel.h
    searchwindow.cpp searchwindow.h
    setupdialog.cpp setupdialog.h
    setuppages/hamlibnetworkconfigwidget.cpp
    setuppages/hamlibserialconfigwidget.cpp
    setuppages/setuppageelog.cpp
    setuppages/setuppagelogview.cpp
    setuppages/setuppagesubdivisionnew.cpp
    setuppages/setuppagesubdivisions.cpp
    widgets/adiflotwexportwidget.cpp
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

if(APPLE)
    message ("MacOS")
    target_include_directories(klog PRIVATE
        /usr/local/include/
    )

    target_link_libraries(klog PRIVATE
        # Remove: L"/usr/local/lib"
        hamlib
    )
endif()

if((TEST_architecture_arch STREQUAL "i386") AND WIN32)
    message (Win32 i386)
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
    message (Win32 not i386)
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
