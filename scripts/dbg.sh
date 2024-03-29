/bin/gdb --args /home/neo/.local/bin/qemu-system-x86_64 \
    -name test \
    -M q35,kernel-irqchip=split \
    -m 8G \
    -smp 2 \
    -cpu host \
    -accel kvm \
    -no-user-config \
    -nodefaults \
    -boot order=c \
    -kernel ../images/vmlinuz-3.10.0-1160.59.1.el7.x86_64 \
    -append "root=/dev/mapper/centos-root ro crashkernel=auto rd.lvm.lv=centos/root rd.lvm.lv=centos/swap console=ttyS0 console=hvc0 edd=off LANG=en_US.UTF-8" \
    -initrd ../images/initramfs-3.10.0-1160.59.1.el7.x86_64.img \
    -device intel-iommu,intremap=on,device-iotlb=on \
    -hda ../images/rootfs.img \
    -vga virtio \
    -vnc :100 \
    -chardev file,id=file0,path=serial.log -serial chardev:file0 \
    -chardev stdio,id=monitor \
    -monitor /dev/pts/7 \
    -net nic -net user,hostfwd=tcp::2222-:22 \
    -device ioh3420,id=remoteport,slot=0 \
    -device ioh3420,id=remoteport1,slot=1 \
    -machine-path /tmp/sim 
