# 4d-plugin-virtual-key
4D plugin to post virtual key codes (CAPS, KANA, PRINT_SCREEN, etc.) on Windows and Mac.

##Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
|🆗|🆗|🆗|🆗|

Commands
---

```c
// --- Virtual Key
POST_VIRTUAL_KEY
Test_virtual_key
```

Examples
---

```
POST VIRTUAL KEY (keyCode;modifiers)
```
In ```keyCode```, pass a standard Windows [virtual keycode](https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731).

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

Press "Windows" key:

```
POST VIRTUAL KEY (0x5B)
```

Exposé the desktop (assuming the default fn+F11): 

```
POST VIRTUAL KEY (0x67;Function key mask)

$pressed:=Test virtual key (keyCode)
```

```
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
