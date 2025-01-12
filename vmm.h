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

/**
 * This is attempt to port VMM/VXD api to OPEN WATCOM, generaly this file
 * is fuzzy of my own macros and MSVC H header form DDK98 and H header from
 * VMM.H from tutorial sample by Bryan A. Woodruff
 *
 **/
#ifndef __VMM_H__INCLUDED__
#define __VMM_H__INCLUDED__

/* MS types */
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned long   ULONG;
typedef void *          PVOID;
typedef char *          PSTR;

#pragma pack(push)
#pragma pack(1)
typedef struct tagVxD_Desc_Block
{
   DWORD  DDB_Next ;                // VMM reserved field
   WORD   DDB_SDK_Version  ;        // VMM reserved field
   WORD   DDB_Req_Device_Number ;   // Required device number
   BYTE   DDB_Dev_Major_Version ;   // Major device number
   BYTE   DDB_Dev_Minor_Version ;   // Minor device number
   WORD   DDB_Flags ;               // Flags for init calls complete
   BYTE   DDB_Name[ 8 ] ;           // Device name
   DWORD  DDB_Init_Order ;          // Initialization Order
   DWORD  DDB_Control_Proc ;        // Offset of control procedure
   DWORD  DDB_V86_API_Proc ;        // Offset of API procedure
   DWORD  DDB_PM_API_Proc ;         // Offset of API procedure
   DWORD  DDB_V86_API_CSIP ;        // CS:IP of API entry point
   DWORD  DDB_PM_API_CSIP ;         // CS:IP of API entry point
   DWORD  DDB_Reference_Data ;      // Reference data from real mode
   DWORD  DDB_Service_Table_Ptr ;   // Pointer to service table
   DWORD  DDB_Service_Table_Size ;  // Number of services
   DWORD  DDB_Win32_Service_Table ;
   DWORD  DDB_Prev ;
   DWORD  DDB_Size;
   DWORD  DDB_Reserved1;
   DWORD  DDB_Reserved2;
   DWORD  DDB_Reserved3;
} DDB ;
#pragma pack(pop)

#define DDK_VERSION 0x30A
//#define DDK_VERSION 0x400
//#define DDK_VERSION 0x40A
// version 3.10 is required or Windows 95 can't load VXD

#define UNDEFINED_DEVICE_ID    0x00000
#define VMM_DEVICE_ID          0x00001   // USED FOR DYNALINK TABLE
#define DEBUG_DEVICE_ID        0x00002
#define VPICD_DEVICE_ID        0x00003
#define VDMAD_DEVICE_ID        0x00004
#define VTD_DEVICE_ID          0x00005
#define V86MMGR_DEVICE_ID      0x00006
#define PAGESWAP_DEVICE_ID     0x00007
#define PARITY_DEVICE_ID       0x00008
#define REBOOT_DEVICE_ID       0x00009
#define VDD_DEVICE_ID          0x0000A
#define VSD_DEVICE_ID          0x0000B
#define VMD_DEVICE_ID          0x0000C
#define VKD_DEVICE_ID          0x0000D
#define VCD_DEVICE_ID          0x0000E
#define VPD_DEVICE_ID          0x0000F
#define BLOCKDEV_DEVICE_ID     0x00010
#define VMCPD_DEVICE_ID        0x00011
#define EBIOS_DEVICE_ID        0x00012
#define BIOSXLAT_DEVICE_ID     0x00013
#define VNETBIOS_DEVICE_ID     0x00014
#define DOSMGR_DEVICE_ID       0x00015
#define WINLOAD_DEVICE_ID      0x00016
#define SHELL_DEVICE_ID        0x00017
#define VMPOLL_DEVICE_ID       0x00018
#define VPROD_DEVICE_ID        0x00019
#define DOSNET_DEVICE_ID       0x0001A
#define VFD_DEVICE_ID          0x0001B
#define VDD2_DEVICE_ID         0x0001C   // SECONDARY DISPLAY ADAPTER
#define WINDEBUG_DEVICE_ID     0x0001D
#define TSRLOAD_DEVICE_ID      0x0001E   // TSR INSTANCE UTILITY ID
#define BIOSHOOK_DEVICE_ID     0x0001F   // BIOS INTERRUPT HOOKER VXD
#define INT13_DEVICE_ID        0x00020
#define PAGEFILE_DEVICE_ID     0x00021   // PAGING FILE DEVICE
#define SCSI_DEVICE_ID         0x00022   // SCSI DEVICE
#define MCA_POS_DEVICE_ID      0x00023   // MCA_POS DEVICE
#define SCSIFD_DEVICE_ID       0x00024   // SCSI FASTDISK DEVICE
#define VPEND_DEVICE_ID        0x00025   // PEN DEVICE
#define APM_DEVICE_ID          0x00026   // POWER MANAGEMENT DEVICE

#define VMM_Init_Order         0x000000000
#define APM_Init_Order         0x001000000
#define Debug_Init_Order       0x004000000
#define BiosHook_Init_Order    0x006000000
#define VPROD_Init_Order       0x008000000
#define VPICD_Init_Order       0x00C000000
#define VTD_Init_Order         0x014000000
#define PageFile_Init_Order    0x018000000
#define PageSwap_Init_Order    0x01C000000
#define Parity_Init_Order      0x020000000
#define Reboot_Init_Order      0x024000000
#define EBIOS_Init_Order       0x026000000
#define VDD_Init_Order         0x028000000
#define VSD_Init_Order         0x02C000000
#define VCD_Init_Order         0x030000000
#define VMD_Init_Order         0x034000000
#define VKD_Init_Order         0x038000000
#define VPD_Init_Order         0x03C000000
#define BlockDev_Init_Order    0x040000000
#define MCA_POS_Init_Order     0x041000000
#define SCSIFD_Init_Order      0x041400000
#define SCSIMaster_Init_Order  0x041800000
#define Int13_Init_Order       0x042000000
#define VFD_Init_Order         0x044000000
#define VMCPD_Init_Order       0x048000000
#define BIOSXlat_Init_Order    0x050000000
#define VNETBIOS_Init_Order    0x054000000
#define DOSMGR_Init_Order      0x058000000
#define DOSNET_Init_Order      0x05C000000
#define WINLOAD_Init_Order     0x060000000
#define VMPoll_Init_Order      0x064000000

#define Undefined_Init_Order   0x080000000

#define WINDEBUG_Init_Order    0x081000000
#define VDMAD_Init_Order       0x090000000
#define V86MMGR_Init_Order     0x0A0000000

//----------------------------------------------------------------------------
//
//                      System control call messages
//
//----------------------------------------------------------------------------
                            
#define Sys_Critical_Init     0x0000
#define Device_Init           0x0001
#define Init_Complete         0x0002
#define Sys_VM_Init           0x0003
#define Sys_VM_Terminate      0x0004
#define System_Exit           0x0005
#define Sys_Critical_Exit     0x0006
#define Create_VM             0x0007
#define VM_Critical_Init      0x0008
#define VM_Init               0x0009
#define VM_Terminate          0x000A
#define VM_Not_Executable     0x000B
#define Destroy_VM            0x000C
#define VM_Suspend            0x000D
#define VM_Resume             0x000E
#define Set_Device_Focus      0x000F
#define Begin_Message_Mode    0x0010
#define End_Message_Mode      0x0011

#define Reboot_Processor      0x0012
#define Query_Destroy         0x0013
#define Debug_Query           0x0014

#define Begin_PM_App          0x0015

#define BPA_32_Bit            0x0001
#define BPA_32_Bit_Flag       0x0001

#define End_PM_App            0x0016

#define Device_Reboot_Notify  0x0017
#define Crit_Reboot_Notify    0x0018

#define Close_VM_Notify       0x0019

#define CVNF_Crit_Close       0x0001
#define CNVF_Crit_Close_Bit   0x0000

#define Power_Event           0x001A


/* 
 * Macro to call VXD service
 * @param _vxd: VXD name (numeric id must be defined as _vxd ## _DEVICE_ID)
 * @param _service: service name (numeric order must be defined as _vxd ## __ ## _service)
 */
#define VxDCall(_vxd, _service) \
    _asm int 20h \
    _asm dw (_vxd ## __ ## _service) \
    _asm dw (_vxd ## _DEVICE_ID)


#define VxDJmp(_vxd, _service) \
    _asm int 20h \
    _asm dw 0x8000 + (_vxd ## __ ## _service) \
    _asm dw (_vxd ## _DEVICE_ID)

/*
 * Macro to VMM call
 * shortcut to VXDCall(VMM, _service_)
 */
#define VMMCall(_service) VxDCall(VMM, _service)

#define VMMJmp(_service) VxDJmp(VMM, _service)

/*
 * WATCOM not supporting enum labels in inline assembly and
 * C macro expansions, so service IDs needs to be manually
 * defined as numbers
 */

#define VMM__Get_VMM_Version 0
#define VMM__Get_Cur_VM_Handle 1
#define VMM__Test_Cur_VM_Handle 2
#define VMM__Get_Sys_VM_Handle 3
#define VMM__Test_Sys_VM_Handle 4
#define VMM__Validate_VM_Handle 5

#define VMM__Get_VMM_Reenter_Count 6
#define VMM__Begin_Reentrant_Execution 7
#define VMM__End_Reentrant_Execution 8

#define VMM__Install_V86_Break_Point 9
#define VMM__Remove_V86_Break_Point 10
#define VMM__Allocate_V86_Call_Back 11
#define VMM__Allocate_PM_Call_Back 12

#define VMM__Call_When_VM_Returns 13

#define VMM__Schedule_Global_Event 14
#define VMM__Schedule_VM_Event 15
#define VMM__Call_Global_Event 16
#define VMM__Call_VM_Event 17
#define VMM__Cancel_Global_Event 18
#define VMM__Cancel_VM_Event 19
#define VMM__Call_Priority_VM_Event 20
#define VMM__Cancel_Priority_VM_Event 21

#define VMM__Get_NMI_Handler_Addr 22
#define VMM__Set_NMI_Handler_Addr 23
#define VMM__Hook_NMI_Event 24

#define VMM__Call_When_VM_Ints_Enabled 25
#define VMM__Enable_VM_Ints 26
#define VMM__Disable_VM_Ints 27

#define VMM__Map_Flat 28
#define VMM__Map_Lin_To_VM_Addr 29

//   Scheduler services

#define VMM__Adjust_Exec_Priority 30
#define VMM__Begin_Critical_Section 31
#define VMM__End_Critical_Section 32
#define VMM__End_Crit_And_Suspend 33
#define VMM__Claim_Critical_Section 34
#define VMM__Release_Critical_Section 35
#define VMM__Call_When_Not_Critical 36
#define VMM__Create_Semaphore 37
#define VMM__Destroy_Semaphore 38
#define VMM__Wait_Semaphore 39
#define VMM__Signal_Semaphore 40
#define VMM__Get_Crit_Section_Status 41
#define VMM__Call_When_Task_Switched 42
#define VMM__Suspend_VM 43
#define VMM__Resume_VM 44
#define VMM__No_Fail_Resume_VM 45
#define VMM__Nuke_VM 46
#define VMM__Crash_Cur_VM 47

#define VMM__Get_Execution_Focus 48
#define VMM__Set_Execution_Focus 49
#define VMM__Get_Time_Slice_Priority 50
#define VMM__Set_Time_Slice_Priority 51
#define VMM__Get_Time_Slice_Granularity 52
#define VMM__Set_Time_Slice_Granularity 53
#define VMM__Get_Time_Slice_Info 54
#define VMM__Adjust_Execution_Time 55
#define VMM__Release_Time_Slice 56
#define VMM__Wake_Up_VM 57
#define VMM__Call_When_Idle 58

#define VMM__Get_Next_VM_Handle 59

//   Time-out and system timer services

#define VMM__Set_Global_Time_Out 60
#define VMM__Set_VM_Time_Out 61
#define VMM__Cancel_Time_Out 62
#define VMM__Get_System_Time 63
#define VMM__Get_VM_Exec_Time 64

#define VMM__Hook_V86_Int_Chain 65
#define VMM__Get_V86_Int_Vector 66
#define VMM__Set_V86_Int_Vector 67
#define VMM__Get_PM_Int_Vector 68
#define VMM__Set_PM_Int_Vector 69

#define VMM__Simulate_Int 70
#define VMM__Simulate_Iret 71
#define VMM__Simulate_Far_Call 72
#define VMM__Simulate_Far_Jmp 73
#define VMM__Simulate_Far_Ret 74
#define VMM__Simulate_Far_Ret_N 75
#define VMM__Build_Int_Stack_Frame 76

#define VMM__Simulate_Push 77
#define VMM__Simulate_Pop 78

// Heap Manager

#define VMM___HeapAllocate 79
#define VMM___HeapReAllocate 80
#define VMM___HeapFree 81
#define VMM___HeapGetSize 82

/*ENDMACROS*/

/****************************************************
 *
 *   Flags for heap allocator calls
 *
 *   NOTE: HIGH 8 BITS (bits 24-31) are reserved
 *
 ***************************************************/

#define HEAPZEROINIT    0x00000001
#define HEAPZEROREINIT  0x00000002
#define HEAPNOCOPY  0x00000004
#define HEAPLOCKEDIFDP  0x00000100
#define HEAPSWAP    0x00000200
#define HEAPINIT        0x00000400
#define HEAPCLEAN   0x00000800

// Page Manager

/*MACROS*/
#define VMM___PageAllocate 83
#define VMM___PageReAllocate 84
#define VMM___PageFree 85
#define VMM___PageLock 86
#define VMM___PageUnLock 87
#define VMM___PageGetSizeAddr 88
#define VMM___PageGetAllocInfo 89
#define VMM___GetFreePageCount 90
#define VMM___GetSysPageCount 91
#define VMM___GetVMPgCount 92
#define VMM___MapIntoV86 93
#define VMM___PhysIntoV86 94
#define VMM___TestGlobalV86Mem 95
#define VMM___ModifyPageBits 96
#define VMM___CopyPageTable 97
#define VMM___LinMapIntoV86 98
#define VMM___LinPageLock 99
#define VMM___LinPageUnLock 100
#define VMM___SetResetV86Pageable 101
#define VMM___GetV86PageableArray 102
#define VMM___PageCheckLinRange 103
#define VMM___PageOutDirtyPages 104
#define VMM___PageDiscardPages 105
/*ENDMACROS*/

/****************************************************
 *
 *  Flags for other page allocator calls
 *
 *  NOTE: HIGH 8 BITS (bits 24-31) are reserved
 *
 ***************************************************/

#define PAGEZEROINIT        0x00000001
#define PAGEUSEALIGN        0x00000002
#define PAGECONTIG      0x00000004
#define PAGEFIXED       0x00000008
#define PAGEDEBUGNULFAULT   0x00000010
#define PAGEZEROREINIT      0x00000020
#define PAGENOCOPY      0x00000040
#define PAGELOCKED      0x00000080
#define PAGELOCKEDIFDP      0x00000100
#define PAGESETV86PAGEABLE  0x00000200
#define PAGECLEARV86PAGEABLE    0x00000400
#define PAGESETV86INTSLOCKED    0x00000800
#define PAGECLEARV86INTSLOCKED  0x00001000
#define PAGEMARKPAGEOUT     0x00002000
#define PAGEPDPSETBASE      0x00004000
#define PAGEPDPCLEARBASE    0x00008000
#define PAGEDISCARD     0x00010000
#define PAGEPDPQUERYDIRTY   0x00020000
#define PAGEMAPFREEPHYSREG  0x00040000
#define PAGENOMOVE      0x10000000
#define PAGEMAPGLOBAL       0x40000000
#define PAGEMARKDIRTY       0x80000000

/****************************************************
 *
 *      Flags for _PhysIntoV86,
 *      _MapIntoV86, and _LinMapIntoV86
 *
 ***************************************************/

#define MAPV86_IGNOREWRAP       0x00000001


// Informational services

/*MACROS*/
#define VMM___GetNulPageHandle 106
#define VMM___GetFirstV86Page 107
#define VMM___MapPhysToLinear 108
#define VMM___GetAppFlatDSAlias 109
#define VMM___SelectorMapFlat 110
#define VMM___GetDemandPageInfo 111
#define VMM___GetSetPageOutCount 112
/*ENDMACROS*/

/*
 *  Flags bits for _GetSetPageOutCount
 */
#define GSPOC_F_GET 0x00000001

// Device VM page manager

/*MACROS*/
#define VMM__Hook_V86_Page 113
#define VMM___Assign_Device_V86_Pages 114
#define VMM___DeAssign_Device_V86_Pages 115
#define VMM___Get_Device_V86_Pages_Array 116
#define VMM__MMGR_SetNULPageAddr 117

// GDT/LDT management

#define VMM___Allocate_GDT_Selector 118
#define VMM___Free_GDT_Selector 119
#define VMM___Allocate_LDT_Selector 120
#define VMM___Free_LDT_Selector 121
#define VMM___BuildDescriptorDWORDs 122
#define VMM___GetDescriptor 123
#define VMM___SetDescriptor 124
/*ENDMACROS*/

/*
*   Flag equates for _Allocate_GDT_Selector
*/
#define ALLOCFROMEND    0x40000000


/*
 *  Flag equates for _BuildDescriptorDWORDs
 */
#define BDDEXPLICITDPL	0x00000001

/*
 *  Flag equates for _Allocate_LDT_Selector
 */
#define ALDTSPECSEL 0x00000001

/*MACROS*/
#define VMM___MMGR_Toggle_HMA 125
/*ENDMACROS*/

/*
 *  Flag equates for _MMGR_Toggle_HMA
 */
#define MMGRHMAPHYSICAL 0x00000001
#define MMGRHMAENABLE	0x00000002
#define MMGRHMADISABLE	0x00000004
#define MMGRHMAQUERY	0x00000008

/*MACROS*/
#define VMM__Get_Fault_Hook_Addrs 126
#define VMM__Hook_V86_Fault 127
#define VMM__Hook_PM_Fault 128
#define VMM__Hook_VMM_Fault 129
#define VMM__Begin_Nest_V86_Exec 130
#define VMM__Begin_Nest_Exec 131
#define VMM__Exec_Int 132
#define VMM__Resume_Exec 133
#define VMM__End_Nest_Exec 134

#define VMM__Allocate_PM_App_CB_Area 135
#define VMM__Get_Cur_PM_App_CB 136
#define VMM__Set_V86_Exec_Mode 137
#define VMM__Set_PM_Exec_Mode 138

#define VMM__Begin_Use_Locked_PM_Stack 139
#define VMM__End_Use_Locked_PM_Stack 149

#define VMM__Save_Client_State 141
#define VMM__Restore_Client_State 142

#define VMM__Exec_VxD_Int 143

#define VMM__Hook_Device_Service 144
#define VMM__Hook_Device_V86_API 145
#define VMM__Hook_Device_PM_API 146

#define VMM__System_Control 147
//   I/O and software interrupt hooks

#define VMM__Simulate_IO 148
#define VMM__Install_Mult_IO_Handlers 149
#define VMM__Install_IO_Handler 150
#define VMM__Enable_Global_Trapping 151
#define VMM__Enable_Local_Trapping 152
#define VMM__Disable_Global_Trapping 153
#define VMM__Disable_Local_Trapping 154

//   Linked List Abstract Data Type Services

#define VMM__List_Create 155
#define VMM__List_Destroy 156
#define VMM__List_Allocate 157
#define VMM__List_Attach 158
#define VMM__List_Attach_Tail 159
#define VMM__List_Insert 160
#define VMM__List_Remove 161
#define VMM__List_Deallocate 162
#define VMM__List_Get_First 163
#define VMM__List_Get_Next 164
#define VMM__List_Remove_First 165
/*ENDMACROS*/

/*
 *   Flags used by List_Create
 */
#define LF_ASYNC_BIT	    0
#define LF_ASYNC	(1 << LF_ASYNC_BIT)
#define LF_USE_HEAP_BIT     1
#define LF_USE_HEAP	(1 << LF_USE_HEAP_BIT)
#define LF_ALLOC_ERROR_BIT  2
#define LF_ALLOC_ERROR	    (1 << LF_ALLOC_ERROR_BIT)
/*
 * Swappable lists must use the heap.
 */
#define LF_SWAP 	(LF_USE_HEAP + (1 << 3))

/******************************************************************************
 *  I N I T I A L I Z A T I O N   P R O C E D U R E S
 ******************************************************************************/

// Instance data manager

/*MACROS*/
#define VMM___AddInstanceItem 166

// System structure data manager

#define VMM___Allocate_Device_CB_Area 167
#define VMM___Allocate_Global_V86_Data_Area 168
#define VMM___Allocate_Temp_V86_Data_Area 169
#define VMM___Free_Temp_V86_Data_Area 170
/*ENDMACROS*/

#pragma pack(push)
#pragma pack(1)
typedef struct tagCRS_32
{
   DWORD  Client_EDI ;
   DWORD  Client_ESI ;
   DWORD  Client_EBP ;
   DWORD  dwReserved_1 ;          // ESP at pushall
   DWORD  Client_EBX ;
   DWORD  Client_EDX ;
   DWORD  Client_ECX ;
   DWORD  Client_EAX ;
   DWORD  Client_Error ;          // DWORD error code
   DWORD  Client_EIP ;
   WORD   Client_CS ;
   WORD   wReserved_2 ;           // (padding)
   DWORD  Client_EFlags ;
   DWORD  Client_ESP ;
   WORD   Client_SS ;
   WORD   wReserved_3 ;           // (padding)
   WORD   Client_ES ;
   WORD   WReserved_4 ;           // (padding)
   WORD   Client_DS ;
   WORD   wReserved_5 ;           // (padding)
   WORD   Client_FS ;
   WORD   wReserved_6 ;           // (padding)
   WORD   Client_GS ;
   WORD   wReserved_7 ;           // (padding)

   DWORD  Client_Alt_EIP ;
   WORD   Client_Alt_CS ;
   WORD   wReserved_8 ;           // (padding)
   DWORD  Client_Alt_EFlags ;
   DWORD  Client_Alt_ESP ;
   WORD   Client_Alt_SS ;
   WORD   wReserved_9 ;           // (padding)
   WORD   Client_Alt_ES ;
   WORD   WReserved_10 ;          // (padding)
   WORD   Client_Alt_DS ;
   WORD   wReserved_11 ;          // (padding)
   WORD   Client_Alt_FS ;
   WORD   wReserved_12 ;          // (padding)
   WORD   Client_Alt_GS ;
   WORD   wReserved_13 ;          // (padding)

} CRS_32, *PCRS_32;
#pragma pack(pop)

/******************************************************************************
 *              PAGE TABLE EQUATES
 *****************************************************************************/

#define P_SIZE      0x1000      /* page size */

/******************************************************************************
 *
 *              PAGE TABLE ENTRY BITS
 *
 *****************************************************************************/

#define P_PRESBIT   0
#define P_PRES      (1 << P_PRESBIT)
#define P_WRITEBIT  1
#define P_WRITE     (1 << P_WRITEBIT)
#define P_USERBIT   2
#define P_USER      (1 << P_USERBIT)
#define P_ACCBIT    5
#define P_ACC       (1 << P_ACCBIT)
#define P_DIRTYBIT  6
#define P_DIRTY     (1 << P_DIRTYBIT)

#define P_AVAIL     (P_PRES+P_WRITE+P_USER) /* avail to user & present */

/****************************************************
 *
 *  Page types for page allocator calls
 *
 ***************************************************/

#define PG_VM       0
#define PG_SYS      1
#define PG_RESERVED1    2
#define PG_PRIVATE  3
#define PG_RESERVED2    4
#define PG_RELOCK   5       /* PRIVATE to MMGR */
#define PG_INSTANCE 6
#define PG_HOOKED   7
#define PG_IGNORE   0xFFFFFFFF

/****************************************************
 *
 *  Definitions for the access byte in a descriptor
 *
 ***************************************************/

/*
 *  Following fields are common to segment and control descriptors
 */
#define D_PRES      0x080       /* present in memory */
#define D_NOTPRES   0           /* not present in memory */

#define D_DPL0      0           /* Ring 0 */
#define D_DPL1      0x020       /* Ring 1 */
#define D_DPL2      0x040       /* Ring 2 */
#define D_DPL3      0x060       /* Ring 3 */

#define D_SEG       0x010       /* Segment descriptor */
#define D_CTRL      0           /* Control descriptor */

#define D_GRAN_BYTE 0x000       /* Segment length is byte granular */
#define D_GRAN_PAGE 0x080       /* Segment length is page granular */
#define D_DEF16     0x000       /* Default operation size is 16 bits */
#define D_DEF32     0x040       /* Default operation size is 32 bits */


/*
 *  Following fields are specific to segment descriptors
 */
#define D_CODE      0x08        /* code */
#define D_DATA      0           /* data */

#define D_X         0           /* if code, exec only */
#define D_RX        0x02        /* if code, readable */
#define D_C         0x04        /* if code, conforming */

#define D_R         0           /* if data, read only */
#define D_W         0x02        /* if data, writable */
#define D_ED        0x04        /* if data, expand down */

#define D_ACCESSED  1           /* segment accessed bit */


/* contains information that an aplication passed to VXD by calling DeviceIoControl function */
struct DIOCParams
{
	DWORD Intrenal1;
	DWORD VMHandle;
	DWORD Internal2;
	DWORD dwIoControlCode;
	DWORD lpInBuffer;
	DWORD cbInBuffer;
	DWORD lpOutBuffer;
	DWORD cbOutBuffer;
	DWORD lpcbBytesReturned;
	DWORD lpOverlapped;
	DWORD hDevice;
	DWORD tagProcess;
};

/* vWin32 communicates with Vxds on behalf of Win32 apps thru this mechanism. */
#define W32_DEVICEIOCONTROL 0x0023

/* sub-functions */
#define DIOC_GETVERSION     0x0
#define DIOC_OPEN       DIOC_GETVERSION
#define DIOC_CLOSEHANDLE    -1

#endif /* __VMM_H__INCLUDED__ */
