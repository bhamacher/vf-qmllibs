[back](../README.md)

# Networkmanager applet

This is an simple QML Linux Networkmanager applet. 
The applet communicates via DBUS with the networkmanager.

Release History:
* 1.0  25.03.2020

## Features

## Tabs
- Ethernet (ipv4/ipv6)
	- automatic mode
	- manual mode
- Wifi (ipv4/ipv6)
	- infrastructure (client) in automatic mode (ipv4/ipv6)
	- Hotspot (access point) in automatic mode(ipv4/ipv6)

## Constraints
It is not possible to open more then one connection as client and one as accesspoint at the same Time
It is also not possible to connect to more then one wired network.

These constraints are independent from the amount of network adapters.

Best case is that only one networkdevice wired and one networkdevice wifi is available.
Nevertheless, the applet will be stable with more than one device.

## How it works

The applet creates the following connections:
- EthernetConnectionAuto
- EthernetConnectionManual
- WifiConnectionClient
- WifiConnectionAccessPoint

each connection handles one Feature. The information will be stored persistant by 
the networkmanager as long as the connections exist. The networkmangager will not 
access connections other then these mentioned above.



## using in QML

### Add the import path to your project

Option 1 (in source code)
```
engine.addImportPath("<CMAKE_INSTALL_PREFIX>/lib64/zera-qml-modules");
```

Option 2 (in our evironment)(not tested!)
```
QML2_IMPORT_PATH=<CMAKE_INSTALL_PREFIX>/lib64/zera-qml-modules
export QML2_IMPORT_PATH
```



Option3 (in qt creator)

* Go to project
* in part Build Environment press add
* In Key: QML2_IMPORT_PATH
* In Value: <CMAKE_INSTALL_PREFIX>/lib64/zera-qml-modules
* press Apply 
* Build project


### In your qml file

```
import networksettings x.x
```


now add the Components EthernetTab and WifiTab:
```
EthernetTab{
	id: <name>
	width: ?
	Layout.fillHeight: true
	height: ?
	fontPixelSize: <font.pixelsize>
}
```
```
WifiTab{
	id: <name>
	width: ?
	Layout.fillHeight: ?
	height: ?
	fontPixelSize: <font.pixelsize>
}
```

The best reuslts are achieved putting them into StackLayout or SwipeView etc.





