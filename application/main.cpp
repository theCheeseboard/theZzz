#include <QCommandLineParser>
#include <QJsonArray>
#include <QUrl>
#include <libthebranch_global.h>
#include <tapplication.h>
#include <tsettings.h>
#include <tstylemanager.h>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    tApplication a(argc, argv);
    a.setApplicationShareDir("thezzz");
    a.installTranslators();

    a.setApplicationVersion("1.0");
    a.setGenericName(QApplication::translate("main", "REST Client"));
    a.setApplicationLicense(tApplication::Gpl3OrLater);
    a.setCopyrightHolder("Victor Tran");
    a.setCopyrightYear("2023");
    a.setOrganizationName("theCheeseboard");
    a.setApplicationName(T_APPMETA_READABLE_NAME);
    a.setDesktopFileName(T_APPMETA_DESKTOP_ID);

#if defined(Q_OS_MAC)
    a.setQuitOnLastWindowClosed(false);
#endif

    tSettings settings;
    QObject::connect(&settings, &tSettings::settingChanged, [=](QString key, QVariant value) {
        if (key == "theme/mode") {
            tStyleManager::setOverrideStyleForApplication(value.toString() == "light" ? tStyleManager::ContemporaryLight : tStyleManager::ContemporaryDark);
        }
    });
    tStyleManager::setOverrideStyleForApplication(settings.value("theme/mode").toString() == "light" ? tStyleManager::ContemporaryLight : tStyleManager::ContemporaryDark);

    theBranch::init();

    MainWindow* w = new MainWindow();
    w->show();

    int retval = a.exec();

    theBranch::teardown();

    return retval;
}
