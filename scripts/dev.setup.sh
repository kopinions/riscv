#!/bin/bash
PROJECT_PATH=$(realpath "$(cd "${BASH_SOURCE[0]%/*}" && pwd)/..")

grep -E "PATH=.*${HOME}/.local/bin" ~/.bashrc || echo "export PATH=${HOME}/.local/bin:${PATH}" >> ~/.bashrc
source ~/.bashrc
mkdir -p ${PROJECT_PATH}/images

if ! [[ -e ${PROJECT_PATH}/images/rootfs.img ]]; then
    qemu-img create -f qcow2 ${PROJECT_PATH}/images/rootfs.img 500G
else
    echo "image have been setup already"
fi

sudo cp /boot/initramfs-$(uname -r).img ${PROJECT_PATH}/images/
cp /boot/vmlinuz-$(uname -r) ${PROJECT_PATH}/images/
sudo chown -R $(whoami):$(echo $(groups $(whoami)) | sed -E 's/\s*//g;s/.*://') ${PROJECT_PATH}/images
qemu-system-x86_64 \
    -m 4096 \
    -cpu host \
    -accel kvm \
    -hda ${PROJECT_PATH}/images/rootfs.img \
    -cdrom $1
