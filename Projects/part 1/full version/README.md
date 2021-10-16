### Setup
To install *make*, *gcc* and *qemu* packages run the following commands:
```
sudo apt update
sudo apt install make
sudo apt install make-guile
sudo apt install gcc
sudo apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virtinst virt-manager
```

### Code Editor
to view and edit the codes use VS Code:
```
sudo apt install --classic code
```

### Compilation
CD into the *xv6-public* directory and run the following commands in the terminal:
```
make
make qemu
```
Finally do `code .` to view the code in VS Code.

### Adding System Calls
Quick guide on how to add system calls [here](https://stackoverflow.com/questions/8021774/how-do-i-add-a-system-call-utility-in-xv6?rq=1).

### Acknowledgments

Copyright 2006-2018 Frans Kaashoek, Robert Morris, and Russ Cox.
