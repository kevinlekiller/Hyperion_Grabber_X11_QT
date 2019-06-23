# Hyperion_Grabber_X11_QT
Hyperion image grabber for X11 / Linux. https://github.com/hyperion-project

Based on Hyperion's X11 grabber https://github.com/hyperion-project/hyperion/blob/master/libsrc/grabber/x11/X11Grabber.cpp

Uses [libXdamage](https://www.freedesktop.org/wiki/Software/XDamage/) to detect X11 display changes, only grabbing / sending images when required.

## Compilation

To compile run `cmake . && make`, qt5 (core / network ; qt5-base on arch) / x11 (damage / render / xlib ; libx11 / libxdamage / libxrender on arch) libraries are required.

## Usage

Run `Hyperion_Grabber_X11_QT` with no arguments or argument `--help` to get a list of options and descriptions.

Example usage: `Hyperion_Grabber_X11_QT --address 192.168.2.213 --port 19444 --frameskip 1 --scale 48 --inactive 600 --redadjust "255,10,0" --greenadjust "75,210,0" --blueadjust "0,10,160" --temperature "255,255,255" --threshold "0.0,0.0,0.0" --transform "1.0,0.0,1.0"`

`hgx11.service` is included as an example if you wish to use a systemd service to start the grabber on login.
Edit it to set your options.
You would copy it to `~/.config/systemd/user/` and enable it with `systemctl --user enable hypergrabx11.service`,
to start it run `systemctl --user enable hypergrabx11.service`.

## Info

The -i option can turn off the Hyperion LED's after configurable amount of time the X11 display has not changed.

The -f option will skip grabbing frames, it can be used to limit the amount of images sent to hyperion. Since the images sent to hyperion is based on the amount of X11 display activity, if this option is set high and not much is going on, there may be a long delay between when images are sent to Hyperion, this option is useful if you're watching videos for example and want to reduce the CPU usage of the grabbing.

The -s option will scale your input resolution by dividing it by this option.
If your display is 3840x2160 and you put in -s 10, the image sent to Hyperion will be 384x216 for example
You should set this option so the image resolution sent to Hyperion is at least the size of your LED array, if you have for example 64 high by 36 wide of LED's, you would want to set -s to 60 or smaller (between 1 and 60) on a 3840x2160 display. The closer to 1 this setting is, the more CPU / bandwith will be used.
Ideally you want a perfect dividor, for example if your LED array is 70x39 and screen sis 3840x2160, then you get (3840/70) 54.86 for the width and (2160/39) 55.38 for the height, the nearest perfect dividor is 48 (3840/48 = 80 ; 2160/48 = 45), if you were to use 54, then there would be rounding error (3840/54 = 71.111, but is truncated to 71) and the image sent to hyperion will not be correctly scaled.


While watching a video with a 3840x2160 60hz display, frameskip at 1 (30fps), divisor at 24 (160x90 output resolution) CPU usage is about 2% of one cpu thread, memory usage is 17.3 KB according to HTOP. CPU usage is 0% when nothing is changing on the display.

## Contributing

There are probably bugs, please submit a pull request if you can.
