# i3AutoLayout
![alt text](https://github.com/Monsterduty/i3AutoLayout/blob/main/example.gif?raw=true)
## dependencies
* g++
* make
* xlib
* xorg
* i3wm

## how to build
```bash

git clone https://github.com/Monsterduty/i3AutoLayout.git
cd i3AutoLayout
make

```
## how to use

just copy the executable where you want

```bash

sudo cp i3-autolayout /usr/local/bin

```
then daemonize the proccess from terminal

```bash

i3-autolayout --daemon

```
or add in your i3 config file a line that say something like:

```bash

exec --no-startup-id i3-autolayout

```
if your file is in a folder that $PATH doesn't have, add a absolute path:
```bash

 exec --no-startup-id /opt/programs/i3-autolayout

```
