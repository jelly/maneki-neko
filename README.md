# Maneki-neko for the Revelation Space

A modified Maneki-neko which contains an Wemos D1 mini and a little servo to make the cat wave on command.
Created to wave at people who are currently in the hackerspace.

![Maneki-neko](https://raw.githubusercontent.com/jelly/maneki-neko/master/neko.png)

## OTA via PlatformIO

1. Install platformio (`sudo pip3 install platformio`)
2. Run `./upload.sh`
3. If the password was wrong, `rm OTA_PASSWORD.h` and try again

## OTA via Arduino IDE (untested; does this work?)

1. Create `OTA_PASSWORD.h` and add:

    #define OTA_PASSWORD "passwordhere"

2. Make sure Arduino IDE is closed (all windows!)
3. Start Arduino IDE

