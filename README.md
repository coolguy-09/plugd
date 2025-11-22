# plugd
A simple udev clone that's fast and lightweight.

## How to compile plugd?
### We first need to install some tools:
For Debian/Ubuntu-based systems:
```bash
sudo apt install gcc make
```
For Arch-based systems:
```bash
sudo pacman -S gcc make
```
### From the root of the source tree:
```bash
make
```
This produces the plugd binary in the current directory.
## How to install plugd?
### After compiling, you can run:
```bash
make install
```
> [!TIP]
> You can use DESTDIR to change the place of installation, for example:
> ```bash
> make install DESTDIR=../your-linux-distro-rootfs
> ```

> [!WARNING]
> You probably will need to use sudo to install/uninstall plugd if you are not using DESTDIR:
> ```bash
> sudo make install
> ```
### You can also copy the plugd executable that was compiled manually:
```bash
cp plugd ../your-linux-distro-rootfs/sbin
```
## How to setup plugd?
### In your init script (let's say BusyBox init):
```bash
#!/bin/sh
#...do some stuff
plugd --daemon
#...do some more stuff
```
### If you don't wanna do that, you can just run it manually in the shell:
```bash
# Remove --daemon to run in foreground instead of background.
plugd --daemon
```
## How to uninstall plugd?
### Just run:
```bash
make uninstall
```
> [!WARNING]
> Again, you probably will need to use sudo to install/uninstall plugd if you are not using DESTDIR:
> ```bash
> sudo make uninstall
> ```
### You can also remove it manually:
```bash
rm -f your-linux-distro-rootfs/sbin/plugd
```
