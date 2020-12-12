# Very simple software version comparator for Arduino
This is a simple sketch to compare software version numbers.
Tested on Arduino Nano 33 IoT.
 
## Usage
Software versions can have up to 5 sections, and each section can be a number spanning from `0` to `65535`. In other words, from `0` to `65535.65535.65535.65535.65535`.

Whenever you need it, you can call `short versionCompare(char v1[], char v2[])`, where v1 and v2 are char arrays, e.g. `char v1[] = "3.14.0.1"`.
The function will return:
- `0` if `v1` > `v2`
- `1` if `v2` > `v1`
- `2` if `v1` = `v2`
- `3` in case of errors
