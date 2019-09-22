TRANSLATIONS += \
    $$PWD/fc100_ru.ts

lrelease.input = TRANSLATIONS
lrelease.output = ${QMAKE_FILE_BASE}.qm
lrelease.commands = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
lrelease.CONFIG += no_link target_predeps

QMAKE_EXTRA_COMPILERS += lrelease

translations.files += $$OUT_PWD/*.qm
translations.CONFIG = no_check_exist
