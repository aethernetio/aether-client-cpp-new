# Copyright 2024 Aethernet Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

Import("env")
import json
from os.path import join
# figure out correct name of strip tool, differs per ESP32 type
platform = env.PioPlatform()
board = env.BoardConfig()
mcu = board.get("build.mcu", "esp32")

riscv32_mcus_list = ["esp32c2", "esp32c3", "esp32c5", "esp32c6", "esp32c61", "esp32h2", "esp32p4"]
xtensa_mcus_list = ["esp32", "esp32s2", "esp32s3"] 
if mcu in riscv32_mcus_list:
    toolchain_arch = "riscv32-esp"
if mcu in xtensa_mcus_list:
    toolchain_arch = "xtensa-esp32"
strip_tool = "%s-elf-strip" % toolchain_arch
# add post action to ELF
env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        strip_tool, "$BUILD_DIR/${PROGNAME}.elf -o $BUILD_DIR/${PROGNAME}_str.elf"
    ]), "Stripping $BUILD_DIR/${PROGNAME}.elf")
)
