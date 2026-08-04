//%attributes = {}
//on mac
//modifiers you can pass one of the following
$modifiers:=Shift key mask:K16:3
$modifiers:=Right shift key mask:K16:11
$modifiers:=Option key mask:K16:7
$modifiers:=Right option key mask:K16:13
$modifiers:=Control key mask:K16:9
$modifiers:=Right control key mask:K16:15
$modifiers:=Command key mask:K16:1
$modifiers:=Function key mask

kVK_F10:=0x006D

//POST VIRTUAL KEY (kVK_F10;$modifiers)



$kVK_Space:=0x0031

POST VIRTUAL KEY($kVK_Space; Command key mask:K16:1 | Control key mask:K16:9)