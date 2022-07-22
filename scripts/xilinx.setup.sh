#!/bin/bash
PROJECT_PATH=$(realpath "$(cd "${BASH_SOURCE[0]%/*}" && pwd)/..")
mkdir -p ${PROJECT_PATH}/3rd
git clone https://github.com/Xilinx/qemu.git ${PROJECT_PATH}/3rd/qemu
pushd ${PROJECT_PATH}/3rd/qemu
./configure --enable-kvm --extra-cflags="-I/opt/atomicorp/atomic/root/usr/include" --prefix=${HOME}/.local
make -C build -j`nproc`
make install
popd

