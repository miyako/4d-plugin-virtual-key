# 4d-plugin-virtual-key
4D plugin to post virtual key codes (CAPS, KANA, PRINT_SCREEN, etc.) on Windows

Example
---

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
