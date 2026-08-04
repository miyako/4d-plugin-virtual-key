//%attributes = {}
Case of 
	: (Is macOS:C1572)
		$kVK_F9:=0x0065
	: (Is Windows:C1573)
		$kVK_F9:=0x0078
End case 

POST VIRTUAL KEY($kVK_F9; Command key mask:K16:1 | Shift key mask:K16:3)