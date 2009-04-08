; resident_common.S
; Common RF-9000 firmware stuff.
; Vladimir Rutsky <altsysrq@gmail.com>
; 08.04.2009

fwOffset             = 0xF95C

fwFrameBufferVarAddr = 0x200FEA64

fwCheckYesNoKeysFunc = 0x200237A0 + fwOffset
fwOSSleepFunc        = 0x20090D64 + fwOffset
fwDrawTextCenterFunc = 0x20044D28 + fwOffset
fwClearScreenFunc    = 0x20040174 + fwOffset
