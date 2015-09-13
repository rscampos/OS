
#include "common.h"

/* Model-specific registers for the i386 family */
#define MSR_IA32_FEATURE_CONTROL        0x03a
#define MSR_BIOS_UPDT_TRIG              0x079
#define MSR_BIOS_SIGN                   0x08b
#define MSR_APICBASE                    0x01b

/* MSR_IA32_FEATURE_CONTROL related */
#define IA32_FEATURE_CONTROL_VMX_EN     0x04    /* enable VMX outside SMX */


/* VMX MSRs */
#define MSR_VMX_BASIC                   0x480
#define MSR_VMX_PINBASED_CTLS           0x481
#define MSR_VMX_PROCBASED_CTLS          0x482
#define MSR_VMX_EXIT_CTLS               0x483
#define MSR_VMX_ENTRY_CTLS              0x484
#define MSR_VMX_CR0_FIXED0              0x486
#define MSR_VMX_CR0_FIXED1              0x487
#define MSR_VMX_CR4_FIXED0              0x488
#define MSR_VMX_CR4_FIXED1              0x489
#define MSR_VMX_PROCBASED_CTLS2         0x48b
#define MSR_VMX_EPT_VPID_CAP            0x48c
#define MSR_VMX_TRUE_PINBASED_CTLS      0x48d
#define MSR_VMX_TRUE_PROCBASED_CTLS     0x48e
#define MSR_VMX_TRUE_EXIT_CTLS          0x48f
#define MSR_VMX_TRUE_ENTRY_CTLS         0x490

/*
 *  * Bits in PPro special registers
 *   */
#define CR4_VME 0x00000001      /* Virtual 8086 mode extensions */
#define CR4_PVI 0x00000002      /* Protected-mode virtual interrupts */
#define CR4_TSD 0x00000004      /* Time stamp disable */
#define CR4_DE  0x00000008      /* Debugging extensions */
#define CR4_PSE 0x00000010      /* Page size extensions */
#define CR4_PAE 0x00000020      /* Physical address extension */
#define CR4_MCE 0x00000040      /* Machine check enable */
#define CR4_PGE 0x00000080      /* Page global enable */
#define CR4_PCE 0x00000100      /* Performance monitoring counter enable */
#define CR4_FXSR 0x00000200     /* Fast FPU save/restore used by OS */
#define CR4_XMM 0x00000400      /* enable SIMD/MMX2 to use except 16 */
#define CR4_VMXE 0x00002000     /* enable VMX operation (Intel-specific) */
#define CR4_FSGSBASE 0x00010000 /* Enable FS/GS BASE accessing instructions */
#define CR4_PCIDE 0x00020000    /* Enable Context ID */
#define CR4_XSAVE 0x00040000    /* XSETBV/XGETBV */
#define CR4_SMEP 0x00100000     /* Supervisor-Mode Execution Prevention */

enum {
        CPUID_FEAT_ECX_SSE3         = 1 << 0, 
        CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
        CPUID_FEAT_ECX_DTES64       = 1 << 2,
        CPUID_FEAT_ECX_MONITOR      = 1 << 3,  
        CPUID_FEAT_ECX_DS_CPL       = 1 << 4,  
        CPUID_FEAT_ECX_VMX          = 1 << 5,  
        CPUID_FEAT_ECX_SMX          = 1 << 6,  
        CPUID_FEAT_ECX_EST          = 1 << 7,  
        CPUID_FEAT_ECX_TM2          = 1 << 8,  
        CPUID_FEAT_ECX_SSSE3        = 1 << 9,  
        CPUID_FEAT_ECX_CID          = 1 << 10,
        CPUID_FEAT_ECX_FMA          = 1 << 12,
        CPUID_FEAT_ECX_SDBG         = 1 << 11,
        CPUID_FEAT_ECX_CX16         = 1 << 13, 
        CPUID_FEAT_ECX_ETPRD        = 1 << 14, 
        CPUID_FEAT_ECX_PDCM         = 1 << 15, 
        CPUID_FEAT_ECX_PCID         = 1 << 17, 
        CPUID_FEAT_ECX_DCA          = 1 << 18, 
        CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
        CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
        CPUID_FEAT_ECX_x2APIC       = 1 << 21, 
        CPUID_FEAT_ECX_MOVBE        = 1 << 22, 
        CPUID_FEAT_ECX_POPCNT       = 1 << 23, 
        CPUID_FEAT_ECX_TSC_DEADLINE = 1 << 24, 
        CPUID_FEAT_ECX_AES          = 1 << 25, 
        CPUID_FEAT_ECX_XSAVE        = 1 << 26, 
        CPUID_FEAT_ECX_OSXSAVE      = 1 << 27, 
        CPUID_FEAT_ECX_AVX          = 1 << 28,
        CPUID_FEAT_ECX_F16C         = 1 << 29,
        CPUID_FEAT_ECX_RDRND        = 1 << 30,
        CPUID_FEAT_ECX_HYPERVISOR   = 1 << 31,

        CPUID_FEAT_EDX_FPU          = 1 << 0,  
        CPUID_FEAT_EDX_VME          = 1 << 1,  
        CPUID_FEAT_EDX_DE           = 1 << 2,  
        CPUID_FEAT_EDX_PSE          = 1 << 3,  
        CPUID_FEAT_EDX_TSC          = 1 << 4,  
        CPUID_FEAT_EDX_MSR          = 1 << 5,  
        CPUID_FEAT_EDX_PAE          = 1 << 6,  
        CPUID_FEAT_EDX_MCE          = 1 << 7,  
        CPUID_FEAT_EDX_CX8          = 1 << 8,  
        CPUID_FEAT_EDX_APIC         = 1 << 9,  
        CPUID_FEAT_EDX_SEP          = 1 << 11, 
        CPUID_FEAT_EDX_MTRR         = 1 << 12, 
        CPUID_FEAT_EDX_PGE          = 1 << 13, 
        CPUID_FEAT_EDX_MCA          = 1 << 14, 
        CPUID_FEAT_EDX_CMOV         = 1 << 15, 
        CPUID_FEAT_EDX_PAT          = 1 << 16, 
        CPUID_FEAT_EDX_PSE36        = 1 << 17, 
        CPUID_FEAT_EDX_PSN          = 1 << 18, 
        CPUID_FEAT_EDX_CLF          = 1 << 19, 
        CPUID_FEAT_EDX_DTES         = 1 << 21, 
        CPUID_FEAT_EDX_NX           = 1 << 20, 
        CPUID_FEAT_EDX_ACPI         = 1 << 22, 
        CPUID_FEAT_EDX_MMX          = 1 << 23, 
        CPUID_FEAT_EDX_FXSR         = 1 << 24, 
        CPUID_FEAT_EDX_SSE          = 1 << 25, 
        CPUID_FEAT_EDX_SSE2         = 1 << 26, 
        CPUID_FEAT_EDX_SS           = 1 << 27, 
        CPUID_FEAT_EDX_HTT          = 1 << 28, 
        CPUID_FEAT_EDX_TM1          = 1 << 29, 
        CPUID_FEAT_EDX_IA64         = 1 << 30,
        CPUID_FEAT_EDX_PBE          = 1 << 31
};

void hardware_cpuid();
