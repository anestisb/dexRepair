# Dex Repair

Command line tool to repair Android DEX bytecode files CRC checksum and SHA-1
hash


## Compile

* Clone this repository
* Install Android NDK if you want to cross-compile for Android devices
* Invoke `make.sh` bash script with desired build target
  * `$ ./make.sh` - default system compiler
  * `$ ./make.sh gcc` - prefer gcc
  * `$ ./make.sh clang` - prefer clang
  * `$ ./make.sh cross-android` - cross-compile for Android with NDK
* Executables are copied under the `bin` directory


## Usage

```
$ ./bin/dexRepair -h
    DEX Repair ver. 0.1.2

    Anestis Bechtsoudis <anestis@census-labs.com>
  Copyright 2015-2017 by CENSUS S.A. All Rights Reserved.

  -I,  --input-files=DIR : input files dirs (1 level recursion only) or single file
  -S,  --repair-sha            : repair SHA-1 hash too (default: disabled)
  -h,  --help            : this help
  -v,  --debug=LEVEL     : debug level (0 - FATAL ... 4 - DEBUG), default: '3' (INFO)
```


## Changelog

* __0.1.3__ - 26 April 2017
  * Support to repair SHA-1 hash too
* __0.1.2__ - 24 April 2017
  * Refactor readdir utils
  * Improve make script & Android.mk
* __0.1.1__ - 5 September 2016
  * Add support for ODEX 037 format
* __0.1.0__ - 14 February 2015
  * Initial commit


## License

```
   Anestis Bechtsoudis <anestis@census-labs.com>
   Copyright 2015-2017 by CENSUS S.A. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
```
