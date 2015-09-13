;*********************************************
;	common.inc
;	- common variables
;*********************************************

%define         MULTIBOOT	0x8000	; store multiboot infose
%define         TEMPMEM  	0x9500	; for Root Directory Table (segment)
%define         KERNELSTART	0x9600	; place to copy the Kernel before PMode
%define		IMAGEPMODE	0x100000; address of the kernel in PMode
