TEMPLATE += fakelib
QTWINMIGRATE_LIBNAME = $$qtLibraryTarget(QtSolutions_MFCMigrationFramework-head)
TEMPLATE -= fakelib
QTWINMIGRATE_LIBDIR = $$PWD/lib
unix:qtwinmigrate-uselib:!qtwinmigrate-buildlib:QMAKE_RPATHDIR += $$QTWINMIGRATE_LIBDIR
