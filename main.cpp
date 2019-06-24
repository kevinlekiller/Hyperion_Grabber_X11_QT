#include <QCoreApplication>
#include <QCommandLineParser>
#include <signal.h>
#include "hgx11.h"

static hgx11 *grab;
static QCoreApplication *qapp;

static void quit(int)
{
    if (grab != nullptr) {
        grab->~hgx11();
    }
    qapp->exit();
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
    QCommandLineOption redAdjust(QStringList() << "r" << "redadjust",
        QCoreApplication::translate("main", "Adjustment of the red color (requires 3 space seperated values between 0 and 255) (ex. \"255,10,0\")"),
        QCoreApplication::translate("main", "adjusts red color of LEDs")
    );
    parser.addOption(redAdjust);
    QCommandLineOption greenAdjust(QStringList() << "g" << "greenadjust",
        QCoreApplication::translate("main", "Adjustment of the green color (requires 3 space seperated values between 0 and 255) (ex. \"75,210,0\")"),
        QCoreApplication::translate("main", "adjusts green color of LEDs")
    );
    parser.addOption(greenAdjust);
    QCommandLineOption blueAdjust(QStringList() << "b" << "blueadjust",
        QCoreApplication::translate("main", "Adjustment of the blue color (requires 3 space seperated values between 0 and 255) (ex. \"0,10,160\")"),
        QCoreApplication::translate("main", "adjusts blue color of LEDs")
    );
    parser.addOption(blueAdjust);
    QCommandLineOption temperature(QStringList() << "t" << "temperature",
        QCoreApplication::translate("main", "Adjustment of the LED color temperature (requires 3 space seperated values between 0 and 255) (ex. \"255,255,250\")"),
        QCoreApplication::translate("main", "adjusts LED color temperature")
    );
    parser.addOption(temperature);
    QCommandLineOption threshold(QStringList() << "d" << "threshold",
        QCoreApplication::translate("main", "Set the threshold of the leds (requires 3 space seperated values between 0.0 and 1.0) (ex. \"0.0025,0.005,0.01\")"),
        QCoreApplication::translate("main", "adjusts LED threshold")
    );
    parser.addOption(threshold);
    QCommandLineOption transform(QStringList() << "l" << "transform",
        QCoreApplication::translate("main", "Adjusts the luminance / saturation of the leds values are in this order: luminanceGain, luminanceMin, saturationL (requires 3 space seperated values between 0.0 and 1.0) (ex. \"1.0,0.01,1.0\")"),
        QCoreApplication::translate("main", "adjusts luminance / saturation")
    );
    parser.addOption(transform);
    parser.process(*qapp);

    if (!parser.isSet("address")) {
        qWarning() << "Hyperion address missing.";
        parser.showHelp();
    }

    if (!parser.isSet("port")) {
        qWarning() << "Hyperion port missing.";
        parser.showHelp();
    }

    QHash<QString, QString> opts;
    QString optName;
    for (int i = 0; i < parser.optionNames().size(); i++) {
        optName = parser.optionNames().at(i);
        opts.insert(optName, parser.value(optName));
    }

    grab = new hgx11(opts);
    return qapp->exec();
}

