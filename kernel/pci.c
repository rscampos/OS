#include "common.h"
#include "isr.h"

u32int pci_config(u8int bus, u8int device, u8int function, u8int offset){
        u32int address;
        u32int temp;
        address = 0x80000000 | (bus << 16) | (device << 11) | (function << 8) | (offset & 0xfc);
        outl(0xCF8,address);
        temp = inl(0xCFC);
        return temp;
}


void init_pci(){
        u32int temp;
        u8int bus=0, dev=0, func=0, off=0;
        for(dev=0;dev<32;dev++){
                for(func=0;func<8;func++){
                        temp = pci_config(bus,dev,func,off);
                        if(temp != 0xFFFFFFFF){
                                printf("%d/%d/%d\n", bus, dev, func);
                                printf("\t0x%x", off);
                                temp = pci_config(bus,dev,func,4);
                                printf("\t0x%x", temp);
                                temp = pci_config(bus,dev,func,8);
                                printf("\t0x%x", temp);
                                temp = pci_config(bus,dev,func,12);
                                printf("\t0x%x\n", temp);

                                temp = pci_config(bus,dev,func,16);
                                printf("\t0x%x", temp);
                                temp = pci_config(bus,dev,func,20);
                                printf("\t0x%x", temp);
                                temp = pci_config(bus,dev,func,24);
                                printf("\t0x%x", temp);
                                temp = pci_config(bus,dev,func,28);
                                printf("\t0x%x\n", temp);
                      }
                }
        }
}
