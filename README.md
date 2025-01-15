# aether-client-cpp

cd G:/projects/prj_aether/Test/TestBuild

git clone https://github.com/aethernet-io/aether-client-cpp.git

cd aether-client-cpp

git submodule update --init --recursive

cd third_party/libbcrypt

git apply "../libbcrypt.patch"

cd ../libhydrogen

git apply "../libhydrogen.patch"

cd ../libsodium

git apply "../libsodium.patch"

cd ../Unity

git apply "../Unity.patch"

Or just run git_init.ps1 for Windows or git_init.sh for Linux.
Select a platform in the aether/platform.h file.