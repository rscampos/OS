;*********************************************
;	common.inc
;	- common variables
;*********************************************

%define         MULTIBOOT	0x8000	        ; store multiboot infose
%define         TEMPMEM  	0x9500	        ; for Root Directory Table (segment)
%define         KERNELSTART	0x9600	        ; place to copy the Kernel before PMode


;-----------------------------------------------;
; The Kernel was compiled and linked at 1MB, so ;
; the base is going to be at 1MB                ;
;-----------------------------------------------;
%define		IMAGEPMODE	0x100000 
