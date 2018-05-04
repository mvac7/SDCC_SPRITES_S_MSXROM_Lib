/* ============================================================================= 
  SDCC MSX ROM SPRITE Small Functions Library (object type)
  Version: 1.1
  Author: mvac7/303bcn
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C
  WEB: 
  mail: mvac7303b@gmail.com

  Description:
    Open Source library with functions to directly access to sprites of the 
    TMS9918A.
    
    It uses the functions from the MSX BIOS, so it is designed to create 
    applications in ROM format.

    It's complemented with the TMS9918A MSX ROM Library.
    
    For SDCC 3.6.0 or higher. 
    
  History of versions:
    v1.1 (2/2/2017)                   
============================================================================= */ 

#include "../include/VDP_SPRITES_S.h"

#include "../include/msxsystemvars.h"
#include "../include/msxBIOS.h"



/* =============================================================================
 SetSpritesSize
 Description: Set size type for the sprites.
 Input:       [char] size: 0=8x8; 1=16x16
 Output:      -
============================================================================= */ 
void SetSpritesSize(char size)
{
size;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   HL,#RG1SAV ; --- read vdp(1) from mem
  ld   B,(HL)

  ld   A,4(ix)    
  cp   #1
  jr   NZ,size8
  set  1,B ; 16x16
  jr   setSize
  
size8:
  res  1,B  ; 8x8    

setSize:  
  ld   C,#0x01
  call WRTVDP
  
  pop  IX
__endasm;
}
  


/* =============================================================================
 SetSpritesZoom
 Description: Set zoom type for the sprites.
 Input:       [boolean] zoom: false/0 = x1; true/1 = x2
 Output:      -
============================================================================= */
void SetSpritesZoom(boolean zoom)
{
zoom;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   HL,#RG1SAV ; --- read vdp(1) from mem
  ld   B,(HL)

  ld   A,4(ix)
  or   A
  jr   Z,nozoom
  set  0,B ; zoom
  jr   setZoom
  
nozoom:
  res  0,B  ;nozoom    

setZoom:  
  ld   C,#0x01
  call WRTVDP
  
  pop  IX
__endasm;
}



/* =============================================================================
 ClearSprites
 Description: 
             Initialises all sprite data The sprite pattern is cleared to
             null, the sprite number to the sprite plane number, the
             sprite colour to the foregtound colour. The vertical location
             of the sprite is set to 209 (mode 0 to 3) or 217 (mode 4 to 8).
 Input:       -
 Output:      -
============================================================================= */
void ClearSprites()
{
__asm
  call CLRSPR
__endasm;
}    



/* =============================================================================
 PUTSPRITE
 Description: Displays the sprite pattern.
 Input:       [char] sprite plane (0-31) 
              [char] x 
              [char] y
              [char] color (0-15)
              [char] pattern
 Output:      -
============================================================================= */
void PUTSPRITE(char plane, char x, char y, char color, char pattern)
{
plane;x;y;color;pattern;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX) ;num sprite plane
  call CALATR
  
  ld   A,6(IX) ;y
  call WRTVRM
  
  inc  HL  
  ld   A,5(IX) ;x
  call WRTVRM
  
  inc  HL  
  ld   E,8(IX)
  call SPRPATNOV  ;pattern
  
  inc  HL
  ld   A,7(IX) ;color
  call WRTVRM
  
  pop  IX
  ret
  


; set sprite pattern number
; Multiply * 4 when its a 16x16 sprite.
; E - Sprite Number  
SPRPATNOV:  ;sprite pattern no calc vram address
  call GSPSIZ          ;0x008A get sprite size in bytes 
  cp   #32             ;16x16
  jr   NZ,WRTPAT

  ;IF 16x16
  SLA  E
  SLA  E ;multiply x 4  

WRTPAT:
  LD   A,E
  call WRTVRM
  
  ;ret
__endasm;
}



