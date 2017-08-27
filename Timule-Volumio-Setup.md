# Volumio APP

### Setting MOVE AP info

File ```/volumio/app/plugins/system_controller/network/index.js```

Change AP SSID (Line **142**): 

```
if (config.get('hotspot_name') == undefined) {
	uiconf.sections[4].content[1].value = 'Volumio';
	} else {
		uiconf.sections[4].content[1].value = config.get('hotspot_name');
	}
```

to: 

```
if (config.get('hotspot_name') == undefined) {
	uiconf.sections[4].content[1].value = 'MOVE-0100001';
	} else {
		uiconf.sections[4].content[1].value = config.get('hotspot_name');
	}
```

Change AP Password (Line **154**):
```
if (config.get('hotspot_password') == undefined) {
	uiconf.sections[4].content[3].value = 'volumio2';
	} else {
		uiconf.sections[4].content[3].value = config.get('hotspot_password');
	}
```

To:

```
if (config.get('hotspot_password') == undefined) {
	uiconf.sections[4].content[3].value = 'Timule';
	} else {
		uiconf.sections[4].content[3].value = config.get('hotspot_password');
	}
```


