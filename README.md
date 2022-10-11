# omni-midi
An omnichord-inspired MIDI controller for raspberry pi + touch screen

## Building

It's written in c++ and uses the following libraries:
- SDL2 (`libsdl2-dev`)
- SDL2_image (`libsdl2-image-dev`)
- SDL2_ttf (`libsdl2-ttf-dev`)
- yaml_cpp (`libyaml-cpp-dev`)
- rtmidi
- and cmake for building

I [built rtmidi from source](https://github.com/thestk/rtmidi) and dropped the folder in the omni-midi project folder. The rest can be installed via apt (or brew on MacOS).

## Installation

This [Raspberry pi UART MIDI Serial](https://gist.github.com/CarloCattano/8c01a6dea6ecbb459acd0b6bcd752ea6) has most of what you need for configuration, including a link to the MIDI circuit spec. The output circuit is very simple just a MIDI jack and two resistors.

I used a raspi 3 with the [official touch screen](https://www.raspberrypi.com/products/raspberry-pi-touch-display/) and I'm working on a custom version of [this case](https://www.thingiverse.com/thing:1503651) to go with it.

## Running

The program has to be run from the project folder. It has a single command line arg for a YAML configuration file  `default_config.yaml` is always loaded and used if no file is specified, or the file doesn't define a given config value. I've included `pi_config.yaml` with the configuration used on my rsapi.

```
# Build
cd omni-midi
cmake .
make

# Set up serial MIDI port. See linked guide. I have this set up to run on boot using rc.local.
ttymidi -s /dev/ttyAMA0 -b 38400 -v &

# Run omni-midi. A bash script desktop shortcut is handy for this.
./omni --config pi_config.yaml
```

The chord buttons send events in MIDI channel 1 and the harp strip sends to channel 2. If you check out `src/input/KeyboardInput.h` there's a hacky keyboard mapping for everything but the harp. Also note that the program can be quit by connecting a keyboard and pressing `esc`
