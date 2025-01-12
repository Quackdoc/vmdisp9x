/*****************************************************************************

Copyright (c) 2023 Jaroslav Hensl <emulator@emulace.cz>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*****************************************************************************/
#include "winhack.h"
#include "vmm.h"
#include "vmwsvxd.h"

#include "minivdd32.h"

#ifdef SVGA
#include "svga_all.h"
#endif

/* code and data is same segment */
#include "code32.h"

#ifdef SVGA
extern Bool svga_init_success;
#endif

/*
You can implement all the VESA support entirely in your mini-VDD. Doing so will
cause VESA applications to run more efficiently since all of the VESA support is
done at Ring 0. You can expect a 20% speed increase by implementing VESA 
unctionality in your mini-VDD over using a real mode VESA driver. 
Following are the VESA functions you can implement in your mini-VDD: 
	CHECK_HIRES_MODE
	CHECK_SCREEN_SWITCH_OK
	GET_BANK_SIZE 
	GET_CURRENT_BANK_READ
	GET_CURRENT_BANK_WRITE
	GET_TOTAL_VRAM_SIZE
	POST_HIRES_SAVE_RESTORE
	PRE_HIRES_SAVE_RESTORE
	SET_BANK
	SET_HIRES_MODE
	VESA_CALL_POST_PROCESSING
	VESA_SUPPORT
*/


/**
REGISTER_DISPLAY_DRIVER (Function 0) 
Call With
EBX: Contains the Windows VM handle. 
EBP: Contains the Windows VM's Client Registers. 
All other registers are via agreement between the display driver and the mini-VDD. 
Return Values
Whatever is agreed upon by the display driver and mini-VDD. 
Remarks
This function is called in response to a display driver call to the Main VDD function VDD_REGISTER_DISPLAY_DRIVER_INFO. 
See also VDD_REGISTER_DISPLAY_DRIVER_INFO. 

**/
VDDPROC(REGISTER_DISPLAY_DRIVER, register_display_driver)
{
	VDD_NC;
}

/**
GET_CHIP_ID (Function 42) 
Call With
EBX: Contains the VM handle (always the Windows VM). 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. EAX contains the ChipID. 
Remarks
Several mini-VDD's support multiple chipsets that utilize different display drivers.
For example, ATI's mini-VDD supports the VGA Wonder which uses SUPERVGA.DRV, the Mach8
which uses ATIM8.DRV, the Mach32 which uses ATIM32.DRV, and the Mach64 which uses ATIM64.DRV.
Therefore, the detection for each of these chipsets is included in the ATI mini-VDD.
GET_CHIP_ID is the only way the Main VDD Plug & Play support code can differentiate between
display cards that use the same mini-VDD. 

The Main VDD calls the function MiniVDD_Dynamic_Init when the mini-VDD is loaded.
If MiniVDD_Dynamic_Init returns with the carry flag clear (indicating success),
the Main VDD calls GET_CHIP_ID as a second check to make sure that the user has
not changed the video card since the last time Windows was run. The Main VDD compares
the value returned by GET_CHIP_ID with the value stored in the registry and if they
are different, it reports the error to the Plug & Play subsystem. 

If the mini-VDD fails to detect one of the cards it supports, MiniVDD_Dynamic_Init
returns with the carry flag set (indicating failure) and the Windows 95's Plug & Play
code loads the standard VGA driver. 

If the value returned by GET_CHIP_ID is different than the value stored in the registry,
or if MiniVDD_Dynamic_Init returns failure, the system displays an error message to
the user concerning the problem with the display settings and allows the user to run
hardware detection to re-detect the video card. 

**/
VDDPROC(GET_CHIP_ID, get_chip_id)
{
#ifdef SVGA
	if(svga_init_success)
	{
		uint32 chip_id = (((uint32)gSVGA.vendorId) << 16) || ((uint32)gSVGA.deviceId);
		state->Client_EAX = chip_id;
		return;
	}
	state->Client_EAX = 0;
#endif
}

/**
CHECK_SCREEN_SWITCH_OK (Function 43) 
Call With
EAX: Contains -1 if running in a known VESA mode. 
EBX: Contains the VM handle (always the Windows VM). 
ECX: Contains the video mode number (if known). 
EBP: Points to the Windows VM's Client Registers. 
Return Values
CY indicates that the hi-res application may not be switched away from. NC indicates
that it is safe to switch away from the hi-res application.
Remarks
The Main VDD calls this routine whenever a user presses ALT-ENTER or ALT-TAB to
switch away from a full-screen MS-DOS prompt. The mini-VDD should determine if
it knows how to restore this mode. If it is a VESA mode or standard VGA mode,
the Main VDD knows how to restore it and unless the mini-VDD has special
considerations, it should return NC. Otherwise, it should return CY causing the
system to beep to alert the user that the hi-res VM cannot be switched away from.
This notification is not given if a user presses CTRL-ALT-DEL to terminate a full-screen
application. In this case, no save of the screen is attempted and it will be impossible
to restore the screen if the user tries to switch back. In this case, if the user tries
to switch back, the system terminates the application.

**/
VDDPROC(CHECK_SCREEN_SWITCH_OK, check_screen_switch_ok)
{
	
}

/**
GET_BANK_SIZE (Function 37) 
Call With
EBX: Contains the VM handle (always the currently executing VM). 
ECX: Contains the VESA BIOS mode number that is currently running. 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. CY returned means that mini-VDD handled the call. EDX contains the current bank size. EAX contains the physical address of the memory aperture or zero to indicate a standard memory aperture at physical address A000:0h. 
Remarks
This routine is called during the save process of a VESA hi-res screen. It tells the Main VDD how large each bank is (so that during the save and restore process, it will know how many bytes to process per pass of the save/restore loop). It also informs the Main VDD where to access the VRAM. Most VESA programs currently set their VRAM at A000:0h. However, VESA version 2 does allow for flat linear apertures. The mini-VDD should determine if the VESA program is using an aperture and return the correct data to the Main VDD. 

**/
VDDPROC(GET_BANK_SIZE, get_bank_size)
{
	
}

/**
GET_CURRENT_BANK_READ (Function 33) 
The parameters and return values for this function are the same as for GET_CURRENT_BANK_WRITE. See GET_CURRENT_BANK_WRITE for details. 
See also GET_CURRENT_BANK_WRITE. 

**/
VDDPROC(GET_CURRENT_BANK_READ, get_current_bank_read)
{
	
}

/**
GET_CURRENT_BANK_WRITE (Function 32) 
Call With
EBX: Contains the VM handle (always the currently executing VM). 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. CY returned means that the mini-VDD handled the call. NC returned means that Main VDD should use a VESA call to retrieve the bank. If successful, EDX contains the current bank (write or read) as set in hardware. 
Remarks
GET_CURRENT_BANK_WRITE and GET_CURRENT_BANK_READ are made when the user presses ALT-TAB to switch away from a VESA hi-res application. The Main VDD uses GET_CURRENT_BANK_WRITE to retrieve the current state of the banking registers (which are "Windows" in VESA terminology). It then saves these for later restoration when the user presses ALT-TAB back to the VESA hi-res application. The Main VDD uses VESA function 4F05h to get the bank if the mini-VDD fails this call. 

**/
VDDPROC(GET_CURRENT_BANK_WRITE, get_current_bank_write)
{
	
}

/**
GET_TOTAL_VRAM_SIZE (Function 36) 
Call With
EBX: Contains the VM handle (always the currently executing VM). 
EBP: Contains the Windows VM's Client Registers. 
Return Values
Save everything that you use. CY returned means that mini-VDD handled the call.
ECX contains the total size of VRAM on the card. 
Remarks
Whenever the VDD saves a hi-res mode, it saves all of the card's video memory to
the swap file. This is because VESA applications have full access to the total memory
on the card, even if their visible screen size is less than the total VRAM size on the card.
Therefore, the Main VDD must know the total VRAM size. If the mini-VDD does not
handle this call, the Main VDD will do a time-consuming call to
VESA BIOS function 4F00h to obtain this information.
For performance reasons, you should implement this function. 

**/
VDDPROC(GET_TOTAL_VRAM_SIZE, get_total_vram_size)
{
#ifdef SVGA
	if(svga_init_success)
	{
		uint32 vram_size = SVGA_ReadReg(SVGA_REG_VRAM_SIZE);
		state->Client_ECX = vram_size;
		
		VDD_CY;
		return;
	}
	
	state->Client_ECX = 0;
	VDD_NC;
#endif
}

/**
PRE_HIRES_SAVE_RESTORE (Function 39) 
Call With
EBX: Contains the VM handle (always the currently executing VM). 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. No values or flags need to be returned. 
Remarks
This call is very similar to PRE_HIRES_TO_VGA in that it allows the mini-VDD to modify port trapping, set flags, etc. in preparation for the mode change into the VESA/hi-res mode. In fact, the S3 example mini-VDD dispatches this call to the exact same routine as PRE_HIRES_TO_VGA. 

**/
VDDPROC(PRE_HIRES_SAVE_RESTORE, pre_hires_save_restore)
{
	
}

/**
POST_HIRES_SAVE_RESTORE (Function 40) 
Call With
EBX: Contains the VM handle (always the currently executing VM). 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. No values or flags need to be returned. 
Remarks
This function is very similar to POST_HIRES_TO_VGA in that it allows the mini-VDD to modify port trapping, set flags, etc. after the mode change into the VESA/hi-res mode. The S3 example mini-VDD dispatches this call to the exact same routine as POST_HIRES_TO_VGA. 

**/
VDDPROC(POST_HIRES_SAVE_RESTORE, post_hires_save_restore)
{
	
}

/**
SET_BANK (Function 34) 
Call With
EAX: Contains the read bank to set. 
EBX: Contains the VM handle (always the currently executing VM). 
EDX: Contains the write bank to set. 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. CY returned means that the mini-VDD handled the call. NC returned means that the Main VDD should use a VESA call to set the bank. 
Remarks
This call requests the mini-VDD to set the read/write bank passed in EAX/EDX. The mini-VDD simply needs to set the bank into hardware and return CY to the Main VDD. 

**/
VDDPROC(SET_BANK, set_bank)
{
	
}

/**
SET_HIRES_MODE (Function 38) 
Call With
EAX: Contains hi-res mode number to set (may be a VESA or non-VESA mode). 
EBX: Contains the VM handle (always the currently executing VM). 
EBP: Points to the Windows VM's Client Registers. 
Return Values
Save everything that you use. CY returned means that the mini-VDD handled the call. NC returned indicates that the mini-VDD did not handle the call. 
Remarks
This routine is called by the VESA/hi-res restore routine in the Main VDD when the user switches back to a full screen VESA/hi-res mode VM. If you are only interested in being able to restore VESA standard hi-res modes, then you do not need to implement this function since the Main VDD will call Interrupt 10h Function 4F02h in order to set the VESA mode number. You should only implement this function if you are going to save/restore chipset specific modes that are not VESA modes. 
If the mode number passed in EAX is a VESA mode number, you should return NC and let the Main VDD set the mode. If the mode number passed in EAX is a non-VESA hi-res mode that is particular to your card, if possible, this function should not touch VRAM since this could cause page faults and confuse the register state of the mode set. In other words, try not to erase the screen during the mode set if possible. 

**/
VDDPROC(SET_HIRES_MODE, set_hires_mode)
{
	
}

/**
VESA_CALL_POST_PROCESSING (Function 47) 
Call With
EBX: Contains the VM handle in which the VESA call was made. 
EDX: The low word contains the VESA function code that was just done. The high word contains the VESA mode number if a VESA mode change (function 4F02h) has just occurred. 
EBP: Points to the VM's client registers. The client registers contain the return values from the VESA call. 
Return Values
Save everything that you use. Nothing is returned to the caller. 
Remarks
This function allows a mini-VDD to perform any necessary processing after a VESA call. For example, this function could fix up the hardware that might have been put in an unexpected state by the VESA call, or it could readjust register trapping. The S3 sample mini-VDD has an example of how this hook could be used to by a mini-VDD. 

**/
VDDPROC(VESA_CALL_POST_PROCESSING, vesa_call_post_processing)
{
	
}

/**
VESA_SUPPORT (Function 41) 
Call With
EBX: Contains the VM handle (always the currently executing VM). 
EBP: Points to the Windows VM's Client Registers. Client registers contain the VESA call values. 
Return Values
Save everything that you use. CY returned means that the mini-VDD completely handled the VESA call and that the VESA.COM or VESA BIOS should not be called. NC returned means that the mini-VDD did not completely handle the call and that the VESA.COM or VESA BIOS should be called. The client registers contain the return values from the VESA call if the mini-VDD handles the call. 
Remarks
This routine is the "hook" by which a mini-VDD could implement an entire Ring 0 protected mode VESA support. This is recommended, since it eliminates all of the problems of old VESA.COM programs. It also allows much faster VESA performance since the functions are supported at 32 bit Ring 0. 
The mini-VDD's VESA support decides what to do based on values in the Client registers. For example, Client_AX will contain 4Fxx indicating what VESA call the application is doing. Then, the mini-VDD can handle the call, filling in return structures (such as those returned by VESA function 4F00h), etc., and return CY to the Main VDD. 
This routine could also be used to setup a VESA call while still letting the Ring 3 VESA BIOS handle the call. The mini-VDD would do what it wants to do, and then return NC indicating that the Main VDD should call the Ring 3 VESA BIOS or VESA.COM program. 

**/
VDDPROC(VESA_SUPPORT, vesa_support)
{
	
}

void Enable_Global_Trapping(DWORD port)
{
	static DWORD sPort = 0;
	sPort = port;
	
	_asm push edx
	_asm mov edx, [sPort];
	VMMCall(Enable_Global_Trapping);
	_asm pop edx
}


void Disable_Global_Trapping(DWORD port)
{
	static DWORD sPort = 0;
	sPort = port;
	
	_asm push edx
	_asm mov edx, [sPort];
	VMMCall(Disable_Global_Trapping);
	_asm pop edx
}

/* for QEMU */
VDDPROC(PRE_HIRES_TO_VGA, pre_hires_to_vga)
{
	Disable_Global_Trapping(0x1CE);
	Disable_Global_Trapping(0x1CF);
}

VDDPROC(POST_HIRES_TO_VGA, post_hires_to_vga)
{
	Enable_Global_Trapping(0x1CE);
	Enable_Global_Trapping(0x1CF);
}

VDDPROC(ENABLE_TRAPS, enable_traps)
{
	Enable_Global_Trapping(0x1CE);
	Enable_Global_Trapping(0x1CF);
}

VDDPROC(DISPLAY_DRIVER_DISABLING, display_driver_disabling)
{
	Disable_Global_Trapping(0x1CE);
	Disable_Global_Trapping(0x1CF);
}
