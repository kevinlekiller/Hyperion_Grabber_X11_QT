# Hyperion_Grabber_X11_QT
Hyperion image grabber for X11 / Linux. https://github.com/hyperion-project

Based on Hyperion's X11 grabber https://github.com/hyperion-project/hyperion/blob/master/libsrc/grabber/x11/X11Grabber.cpp

Uses [libXdamage](https://www.freedesktop.org/wiki/Software/XDamage/) to detect X11 display changes, only grabbing / sending images when required.

The -i option can turn off the Hyperion LED's after configurable amount of time the X11 display has not changed.

The -f option will skip grabbing frames, it can be used to limit the amount of images sent to hyperion. Since the images sent to hyperion is based on the amount of X11 display activity, if this option is set high and not much is going on, there may be a long delay between when images are sent to Hyperion, this option is useful if you're watching videos for example and want to reduce the CPU usage of the grabbing.

The -s option will scale your input resolution by dividing it by this option. If your display is 3840x2160 and you put in -s 10, the image sent to Hyperion will be 384x216 for example. You should set this option so the image resolution sent to Hyperion is at least the size of your LED array, if you have for example 64 high by 36 wide of LED's, you would want to set -s to at least 60 on a 3840x2160 display. The higher this setting, the more bandwith / cpu usage the program will take.

There are probably bugs, please submit a pull request if you can.

To compile run `cmake . && make`, qt5 / x11 libraries are required.
