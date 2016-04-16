# WlanHelper

WlanHelper is used to set/get the operation mode (like monitor mode) for a wireless adapter on Windows.

## A little tip

WlanHelper tries to follow the grammar of [iwconfig](http://linux.die.net/man/8/iwconfig), a wireless management tool for Linux. So if you rename ``WlanHelper.exe`` to ``iwconfig.exe``, your command lines for configuring operation mode on Windows will be exactly the same with the alternative on Linux.

## Terminology

**Managed Mode** (for ``Linux``) = **Extensible Station Mode** (aka **ExtSTA**, for ``Windows``)

**Monitor Mode** (for ``Linux``) = **Network Monitor Mode** (aka **NetMon**, for ``Windows``)

## Usage

### Interactive way:

Run ``WlanHelper`` without parameters.

### Command-line API way:

1. Run ``netsh wlan show interfaces``, get the ``GUID`` for the interface.

2. Run ``WlanHelper -h`` to see the usage. ``{Interface Name}`` refers to the ``GUID`` in the above step.

```
Command Usage:
WlanHelper {Interface Name} mode [*null*|managed|monitor]
*null* - get interface mode
managed - set interface mode to managed mode (aka ExtSTA)
monitor - set interface mode to monitor mode (aka NetMon)
```

An example:

```
C:\>WlanHelper.exe 42dfd47a-2764-43ac-b58e-3df569c447da mode
managed
C:\>WlanHelper.exe 42dfd47a-2764-43ac-b58e-3df569c447da mode monitor
Success
C:\>WlanHelper.exe 42dfd47a-2764-43ac-b58e-3df569c447da mode 
monitor
C:\>WlanHelper.exe 42dfd47a-2764-43ac-b58e-3df569c447da mode managed
Success
C:\>WlanHelper.exe 42dfd47a-2764-43ac-b58e-3df569c447da mode
managed
C:\>WlanHelper.exe 42dfd47a-2764-43ac-b58e-3df569c447da mode master
Error: invalid parameter, type in "WlanHelper -h" for help.
```

# Build

1. Build ``WlanHelper.sln`` with **Visual Studio 2013**

# Release

https://github.com/hsluoyz/WlanHelper/releases

