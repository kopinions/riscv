echo "export PATH=${HOME}/.local/bin:${PATH}" >> ~/.bashrc

source ~/.bashrc

qemu-img create -f qcow2 ${PROJECT_PATH}/images/rootfs.img 100G
qemu-system-x86_64 -m 4096 -cpu host -accel kvm -hda ${PROJECT_PATH}/images/rootfs.img -cdrom=$1
