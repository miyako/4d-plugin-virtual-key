# 4d-plugin-virtual-key
4D plugin to post virtual key codes (CAPS, KANA, PRINT_SCREEN, etc.) on Windows and Mac.

### Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
||<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|

### Version

<img width="32" height="32" src="https://user-images.githubusercontent.com/1725068/73986501-15964580-4981-11ea-9ac1-73c5cee50aae.png"> <img src="https://user-images.githubusercontent.com/1725068/73987971-db2ea780-4984-11ea-8ada-e25fb9c3cf4e.png" width="32" height="32" />

### Remarks

On Mac, privacy access must be granted.

<img width="573" alt="Screen Shot 2020-02-21 at 7 01 27" src="https://user-images.githubusercontent.com/1725068/74982777-13031800-5478-11ea-9210-019c216ae263.png">

## Examples

* invoke Runtimer Explorer

```
Case of 
	: (Is macOS)
		$kVK_F9:=0x0065
	: (Is Windows)
		$kVK_F9:=0x0078
End case 

POST VIRTUAL KEY ($kVK_F9;Command key mask | Shift key mask)
```

* Press "Windows" key:

```
POST VIRTUAL KEY (0x5B)
```

* Exposé the desktop (assuming the default fn+F11): 

```
POST VIRTUAL KEY (0x67;Function key mask)

$pressed:=Test virtual key (keyCode)

  //full list of VKs can be found here
  //https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731

POST VIRTUAL KEY (VK_CAPITAL)
$caps:=Test virtual key (VK_CAPITAL)

POST VIRTUAL KEY (VK_NUMLOCK)
$num:=Test virtual key (VK_NUMLOCK)

POST VIRTUAL KEY (VK_KANA)
$kana:=Test virtual key (VK_KANA)

POST VIRTUAL KEY (VK_SNAPSHOT)
GET PICTURE FROM PASTEBOARD($image)
```

## Syntax

```
POST VIRTUAL KEY (keyCode;modifiers)
```

Parameter|Type|Description
------------|------------|----
keyCode|LONGINT|
modifiers|LONGINT|

```
pressed:=Test virtual key (keyCode)
```

Parameter|Type|Description
------------|------------|----
keyCode|LONGINT|
pressed|LONGINT|

In ```keyCode``` pass a standard Windows [virtual keycode](https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731).

In ```modifiers``` pass one of the following [4D modifiers](http://doc.4d.com/4Dv14/4D/14.3/Events-Modifiers.302-1697268.en.html):

* Control key mask 
* Right control key mask 
* Option key mask 
* Right option key mask 
* Shift key mask 
* Right shift key mask 

On Mac, pass a standard Apple keyCode in ```keyCode```. You might find a list of keycodes [here](http://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes).

In addition to the modifiers listed above, you can also pass the following constants: 

* Function key mask 
* Command control key mask 
