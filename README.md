![version](https://img.shields.io/badge/version-18%2B-EB8E5F)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-64&color=blue)

![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-virtual-key/total)

# 4d-plugin-virtual-key
4D plugin to post virtual key codes (CAPS, KANA, PRINT_SCREEN, etc.) on Windows and Mac.

### Remarks

On Mac, privacy access must be granted.

<img width="573" alt="Screen Shot 2020-02-21 at 7 01 27" src="https://user-images.githubusercontent.com/1725068/74982777-13031800-5478-11ea-9210-019c216ae263.png">

## Examples

* invoke Runtimer Explorer

```4d
Case of 
	: (Is macOS)
		$kVK_F9:=0x0065
	: (Is Windows)
		$kVK_F9:=0x0078
End case 

POST VIRTUAL KEY ($kVK_F9;Command key mask | Shift key mask)
```

* Press "Windows" key:

```4d
POST VIRTUAL KEY (0x5B)
```

* Exposé the desktop (assuming the default fn+F11): 

```4d
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

```4d
POST VIRTUAL KEY (keyCode;modifiers)
```

Parameter|Type|Description
------------|------------|----
keyCode|LONGINT|
modifiers|LONGINT|

```4d
pressed:=Test virtual key (keyCode)
```

Parameter|Type|Description
------------|------------|----
keyCode|LONGINT|
pressed|LONGINT|

In ```keyCode``` pass a standard Windows [virtual keycode](https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731) or a [standard Apple keyCode](http://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes).

In ```modifiers``` pass one of the following [4D modifiers](http://doc.4d.com/4Dv14/4D/14.3/Events-Modifiers.302-1697268.en.html):

* Control key mask 
* Right control key mask 
* Option key mask 
* Right option key mask 
* Shift key mask 
* Right shift key mask 

In addition to the modifiers listed above, you can also pass the following constants: 

* Function key mask 
* Command control key mask 
