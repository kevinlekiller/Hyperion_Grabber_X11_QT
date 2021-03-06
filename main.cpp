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
    parser.addOption({{"a", "address"}, "Address to the hyperion json server. (ex. 127.0.0.1)", "address"});
    parser.addOption({{"p", "port"}, "Port for the hyperion json server. (ex. 19444)", "port"});
    parser.addOption({{"c", "priority"}, "Priority to send to Hyperion, lower number means higher priority, defaults to 100. Range 0-255", "number"});
    parser.addOption({{"s", "scale"}, "Divisor used to scale your screen resolution (ex. 8 ; if your screen is 1920x1080, the result image sent to hyperion is 240x135", "scale"});
    parser.addOption({{"f", "frameskip"}, "How many X11 frames to skip over. (ex. 4 ; 1 frame will be scaled and sent to hyperion and 4 will be ignored)", "frames"});
    parser.addOption({{"i", "inactive"}, "How many seconds after the screen is inactive to turn off the LED's. Set to 0 to disable.", "seonds"});
    parser.addOption({{"j", "inactivetype"}, "If `i` or `inactive` is set, how to determine activity, using (1) Xscreensaver (based on amount of time since user input activity) or (0) Xdamage (based on amount of time since screen activity change)", "type"});
    parser.addOption({{"r", "redadjust"}, "Adjustment of the LED's red color (requires 3 space seperated values between 0 and 255) (ex. \"255,10,0\")", "value"});
    parser.addOption({{"g", "greenadjust"}, "Adjustment of the LED's green color (requires 3 space seperated values between 0 and 255) (ex. \"75,210,0\")", "value"});
    parser.addOption({{"b", "blueadjust"}, "Adjustment of the LED's blue color (requires 3 space seperated values between 0 and 255) (ex. \"0,10,160\")", "value"});
    parser.addOption({{"t", "temperature"}, "Adjustment of the LED's color temperature (requires 3 space seperated values between 0 and 255) (ex. \"255,255,250\")", "value"});
    parser.addOption({{"d", "threshold"}, "Set the threshold of the LED's (requires 3 space seperated values between 0.0 and 1.0) (ex. \"0.0025,0.005,0.01\")", "value"});
    parser.addOption({{"l", "transform"}, "Adjusts the luminance / saturation of the LED's values are in this order: luminanceGain, luminanceMin, saturationL (requires 3 space seperated values between 0.0 and 1.0) (ex. \"1.0,0.01,1.0\")", "value"});
    parser.addOption({{"x", "scalefilter"}, "X11 scaling filter to use (see X11's render.h). 0 nearest (default), 1 bilinear, 2 fast, 3 good, 4 best", "value"});
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

