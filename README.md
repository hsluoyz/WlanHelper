# WlanHelper

WlanHelper is used to set/get the operation mode (like monitor mode) for a wireless adapter on Windows.

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

# Build

1. Build ``WlanHelper.sln`` with **Visual Studio 2013**

# Release

https://github.com/hsluoyz/WlanHelper/releases

