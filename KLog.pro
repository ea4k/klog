TEMPLATE = subdirs

SUBDIRS += src

CONFIG(debug, debug) {
        SUBDIRS += tests
}