#include <QCoreApplication>
#include <QCommandLineParser>
#include <signal.h>
#include "hgx11.h"

static hgx11 *grab;
static QCoreApplication *qapp;

static void quit(int sig)
{
    grab->~hgx11();
    qapp->exit(sig);
}

int main(int argc, char *argv[])
{
    qapp = new QCoreApplication(argc, argv);
    signal(SIGINT, quit);
    signal(SIGTERM, quit);

    QCoreApplication::setApplicationName("hgx11");
    QCoreApplication::setApplicationVersion("0.1");
    QCommandLineParser parser;
    parser.setApplicationDescription("X11 grabber for Hyperion using QT");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption address(QStringList() << "a" << "address",
        QCoreApplication::translate("main", "Address to the hyperion json server. (ex. 127.0.0.1)"),
        QCoreApplication::translate("main", "hyperion address")
    );
    parser.addOption(address);
    QCommandLineOption port(QStringList() << "p" << "port",
        QCoreApplication::translate("main", "Port for the hyperion json server. (ex. 19444)"),
        QCoreApplication::translate("main", "hyperion port")
    );
    parser.addOption(port);
    QCommandLineOption scale(QStringList() << "s" << "scale",
        QCoreApplication::translate("main", "Divisor used to scale your screen resolution (ex. 8 ; if your screen is 1920x1080, the result image sent to hyperion is 240x135"),
        QCoreApplication::translate("main", "scale dividor")
    );
    parser.addOption(scale);
    QCommandLineOption frameskip(QStringList() << "f" << "frameskip",
        QCoreApplication::translate("main", "How many X11 frames to skip over. (ex. 4 ; 1 frame will be scaled and sent to hyperion and 4 will be ignored)"),
        QCoreApplication::translate("main", "X11 frames to skip")
    );
    parser.addOption(frameskip);
    QCommandLineOption inactiveTime(QStringList() << "i" << "inactive",
        QCoreApplication::translate("main", "How many seconds after the screen has not changed to turn off the LED's. Set to 0 to disable."),
        QCoreApplication::translate("main", "seconds before turning off LEDs")
    );
    parser.addOption(inactiveTime);
    parser.process(*qapp);

    if (!parser.isSet("address")) {
        qWarning() << "Hyperion address missing.";
        parser.showHelp();
    }

    if (!parser.isSet("port")) {
        qWarning() << "Hyperion port missing.";
        parser.showHelp();
    }

    grab = new hgx11(parser.value("address"), parser.value("port"), parser.value("scale"), parser.value("frameskip"), parser.value("inactive"));
    return qapp->exec();
}

