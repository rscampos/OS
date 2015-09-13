#include "common.h"
#include "isr.h"
#include "hardware.h"

u32int rcr4(){
        u32int data;
        asm volatile(   "movl %%cr4,%0;" : "=r" ( data ));
        return data;
}

void load_cr4(u32int data){
        asm volatile(   "movl %0,%%cr4;" :: "r" ( data ));
}

void exec_cpuid(u32int *reg_eax, u32int *reg_ebx, u32int *reg_ecx, u32int *reg_edx){
        asm volatile(   "cpuid;"
                        : "=a" ( *reg_eax ), "=b" ( *reg_ebx ), "=c" ( *reg_ecx), "=d" ( *reg_edx )       /* output */
                        : "a" ( *reg_eax )    /* input */
                //: "%eax" 
        );
}

u32int rdmsr(u32int msr){
        u32int low;
        asm volatile(   "rdmsr;"
                        : "=a" ( low )
                        : "c" ( msr )
                        : "edx"
        );
        return low;
}


void get_features(u32int reg_ecx, u32int reg_edx){
        printf("Proc flags:");

        /* reg ecx */
        if(reg_ecx & CPUID_FEAT_ECX_VMX) printf(" vmx");
        if(reg_ecx & CPUID_FEAT_ECX_VMX) printf(" vme");
        if(reg_ecx & CPUID_FEAT_ECX_SSE3) printf(" sse3");
        if(reg_ecx & CPUID_FEAT_ECX_AES) printf(" aes");
        if(reg_ecx & CPUID_FEAT_ECX_HYPERVISOR) printf(" hypervisor");
        
        /* reg edx */
        if(reg_edx & CPUID_FEAT_EDX_FPU) printf(" fpu");
        if(reg_edx & CPUID_FEAT_EDX_VME) printf(" vme");
        if(reg_edx & CPUID_FEAT_EDX_MSR) printf(" msr");
        if(reg_edx & CPUID_FEAT_EDX_PAE) printf(" pae");
        if(reg_edx & CPUID_FEAT_EDX_APIC) printf(" apic");
        if(reg_edx & CPUID_FEAT_EDX_NX) printf(" nx");
        if(reg_edx & CPUID_FEAT_EDX_MMX) printf(" mmx");
        if(reg_edx & CPUID_FEAT_EDX_HTT) printf(" htt");
        if(reg_edx & CPUID_FEAT_EDX_IA64) printf(" ia64");
        
        printf("\n");
}

void hardware_cpuid(){
        u32int reg_eax=0x0;
        u32int reg_ebx=0x0;
        u32int reg_ecx=0x0;
        u32int reg_edx=0x0;
        u32int msr, basic, cr0_fixed0, cr4_fixed0, cr4;
        int i=0;

        printf("Getting information about the CPU:\n");

        /* Get Vendor ID */
        //reg_eax=0x0;
        //exec_cpuid(&reg_eax, &reg_ebx, &reg_ecx, &reg_edx);
        //printf("EAX:%x string:%x%x%x \n", reg_eax, reg_ebx, reg_edx, reg_ecx);

        /* Processor Info and Feature Bits */
        reg_eax = 0x1;
        exec_cpuid(&reg_eax, &reg_ebx, &reg_ecx, &reg_edx);
        printf("EAX:%x EBX:%x ECX:%x EDX:%x \n", reg_eax, reg_ebx, reg_ecx, reg_edx);
        get_features(reg_ecx, reg_edx);
        msr = rdmsr(MSR_IA32_FEATURE_CONTROL);
        
        if (!(msr & IA32_FEATURE_CONTROL_VMX_EN))
                printf("(VMX disabled in BIOS)\n");
                
        basic = rdmsr(MSR_VMX_BASIC);
        cr0_fixed0 = rdmsr(MSR_VMX_CR0_FIXED0);
        cr4_fixed0 = rdmsr(MSR_VMX_CR4_FIXED0);
        cr4 = rcr4();
        load_cr4(cr4 | CR4_VMXE); 
        cr4 = rcr4();
        printf("CR4:0x%x\n",cr4); 
        printf("APIC Base:0x%x\n",rdmsr(MSR_APICBASE)); 
        printf("MSR:0x%x BASIC:0x%x CR0:0x%x CR4:0x%x\n",msr, basic, cr0_fixed0, cr4_fixed0);
}
