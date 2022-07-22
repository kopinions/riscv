#!/bin/bash
source "$(cd "${BASH_SOURCE[0]%/*}" && pwd)/lib/lib/oo-bootstrap.sh"

import util/exception util/tryCatch
try {

    yum install -y rh-git227
    echo "source /opt/rh/rh-git227/enable" >> ~/.bashrc
    yum install -y python3
    yum install -y devtoolset-8
    echo "source /opt/rh/devtoolset-8/enable" >> ~/.bashrc

    yum install -y glib2 glib2-devel pixman-devel zlib-devel gtk3-devel SDL-devel 
    yum install -y automake

    # yum install -y epel-release
    yum install -y ninja-build

    #yum install -y http://www6.atomicorp.com/channels/atomic/centos/7/x86_64/RPMS/atomic-release-1.0-21.el7.art.noarch.rpm || echo "bypassed"
    yum install -y atomic-libgcrypt-devel atomic-libgcrypt
    echo "source /opt/atomicorp/atomic/enable" >> ~/.bashrc

    yum install -y expect
    yum install -y tigervnc
}
catch {
    exit 1
    echo "Faile to setup system"
}


