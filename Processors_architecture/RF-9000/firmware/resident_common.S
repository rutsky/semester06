; resident_common.S
; Common RF-9000 firmware stuff.
; Vladimir Rutsky <altsysrq@gmail.com>
; 08.04.2009

fwOffset               = 0xF95C

fwFrameBufferVarAddr   = 0x200FEA64

fwCheckYesNoKeysFunc   = 0x200237A0 + fwOffset
fwOSSleepFunc          = 0x20090D64 + fwOffset
fwDrawTextCenterFunc   = 0x20044D28 + fwOffset
fwDraw2LinesFunc       = 0x20047A6C + fwOffset
fwClearScreenFunc      = 0x20040174 + fwOffset

hwLCDC_BASE            = 0x80000f00
hwIMG_BASE             = hwLCDC_BASE + 19 * 4
hwIMG_REG_BASE0        = hwIMG_BASE + 8