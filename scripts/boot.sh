PROJECT_PATH=$(realpath "$(cd "${BASH_SOURCE[0]%/*}" && pwd)/..")
imgroot=${PROJECT_PATH}/images
accel="kvm"
kernel_release=$(uname -r)
vncport=59

gdb --args /home/neo/.local/bin/qemu-system-x86_64 \
    -name test \
    -M q35,kernel-irqchip=split \
    -m 8G \
    -smp 2 \
    -cpu host \
    -accel ${accel} \
    -no-user-config \
    -nodefaults \
    -boot order=c \
    -kernel ${imgroot}/vmlinuz-${kernel_release} \
    -append 'root=/dev/mapper/centos-root ro crashkernel=auto rd.lvm.lv=centos/root rd.lvm.lv=centos/swap console=ttyS0 console=hvc0 edd=off LANG=en_US.UTF-8' \
    -initrd ${imgroot}/initramfs-${kernel_release}.img \
    -device intel-iommu,intremap=on,device-iotlb=on \
    -hda ${imgroot}/rootfs.img \
    -vga virtio \
    -display gtk \
    -vnc :${vncport} \
    -chardev file,id=file0,path=serial.log -serial chardev:file0 \
    -chardev stdio,id=monitor \
    -mon chardev=monitor \
    -net nic -net user,hostfwd=tcp::2222-:22 \
    -device ioh3420,id=remoteport,slot=0 \
    -device ioh3420,id=remoteport1,slot=1 \
    -machine-path /tmp/sim \
    -device remote-port-pci-adaptor,bus=remoteport,id=rp0



