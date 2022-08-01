#!/bin/bash
PROJECT_PATH=$(realpath "$(cd "${BASH_SOURCE[0]%/*}" && pwd)/..")

mkdir -p ${PROJECT_PATH}/3rd

git clone https://github.com/Xilinx/qemu.git ${PROJECT_PATH}/3rd/qemu

pushd ${PROJECT_PATH}/3rd/qemu
./configure \
    --target-list=x86_64-softmmu \
    --enable-kvm \
    --extra-cflags="-I/opt/atomicorp/atomic/root/usr/include -g3" \
    --prefix=${HOME}/.local \
    --enable-debug \
    --extra-ldflags="-g3" \
    --disable-strip \
    --disable-pie

make -C build -j`nproc`
make install
popd

