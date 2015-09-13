;*********************************************
;	fat12.inc
;	- FAT12 routines
;	Operating Systems Development Tutorial
;*********************************************

 
bits	16		; We are still in 16 bit Real Mode

bpbOEM:			DB "My OS   "
bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	DB 2
bpbRootEntries: 	DW 224
bpbTotalSectors: 	DW 2880
bpbMedia: 	        DB 0xF8
bpbSectorsPerFAT: 	DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors:	DD 0
bpbTotalSectorsBig:	DD 0
bsDriveNumber:		DB 0
bsUnused:		DB 0
bsExtBootSignature:	DB 0x29
bsSerialNumber:		DD 0xa0a1a2a3
bsVolumeLabel:		DB "MOS FLOPPY "
bsFileSystem:		DB "FAT12   "

;----------------------------------------------------------
; functions
;----------------------------------------------------------

%include "common.inc"    ; basic input/output

;----------------------------------------------------------
; LBACHS - cl (LBA - number of sectors)
;----------------------------------------------------------
LBACHS:
        ; getting the sector (=> absolutesector)
        mov     al, cl
        xor     dx, dx
        div     WORD[bpbSectorsPerTrack]
        inc     dl
        mov     BYTE[absolutesector], dl

        ; getting the head (=> absolutehead)
        xor     dx, dx
        div     WORD[bpbHeadsPerCylinder]
        mov     BYTE[absolutehead], dl

        ; getting the track (=> absolute track)
	mov     BYTE[absolutetrack], al
        ret
;----------------------------------------------------------
; Read sectors - need to pass: bx (place to load in memory 0x07c0:bx), sectorsoffset and sectorstoread
;----------------------------------------------------------
READ_SECTORS:
        ; reset the disk
        .RESET:
                mov     ah, 0   			; reset floppy disk function
                mov     dl, 0   			; drive 0 is floppy drive
                int     0x13    			; call BIOS
                jc      .RESET  			; If Carry Flag (CF) is set, there was an error. Try resetting again

        ; converting LBA to CHS
        mov     cl, BYTE[sectorsoffset] 		; offset sectos untill to the wantend chunck of sectors(LBA)
        call LBACHS                             	; convert LBA to CHS

        ; load from disk to the memory
        ; Put the sector into es:bx
        .READ:
                mov     ah, 0x02                        ; int 0x13 function 0x02 = "read"
                mov     al, BYTE[sectorstoread]		; how many sectors to read (load to memory)
                mov     ch, BYTE[absolutetrack] 	; absolute track of CHS
                mov     cl, BYTE[absolutesector]        ; absolute sector of CHS
                mov     dh, BYTE[absolutehead]          ; absolute head of CHS
                mov     dl, 0x00                        ; Drive number. floppy drive = 0.
                int     0x13                    	; call BIOS - Read the sector
                jc      .READ				; If Carry Flag (CF) is set, there was an error.
                ret
;----------------------------------------------------------
; load the root directory table (RDT)
;----------------------------------------------------------
LOAD_ROOT:
        mov     al, BYTE[sizerootdir]		; size of root dir
	mov	BYTE[sectorstoread], al		; how many sector to be read?
        mov     al, BYTE[offsetrootdir]		; offset of RDT
        mov     BYTE[sectorsoffset], al		; how many sector of offset?

        ; put the root directory at an address
        mov     bx, TEMPMEM			; the address to load
        call    READ_SECTORS			; read the RDT to find the file (stage2)
        ret
;----------------------------------------------------------
; get the next cluster - need to pass: dx - cluster number
;----------------------------------------------------------
; each entry is 12-bit longe

GET_NEXT_CLUSTER:		
		mov	ax, dx
		mov	cx, 0x2
		xor	dx, dx
		div	cx
		mov	cx, 0x3
		mov	bx, dx
		mul	cx
		xor	cx, cx
		cmp	cx, bx
		je .DONTADD
		add	ax, 0x1
	.DONTADD:
		; ax store the offset in FAT
		mov	dx, TEMPMEM		; get the address of FAT
		add	dx, ax
		mov	di, dx
		mov	dx, [di]
		xor	cx, cx
		cmp	cx, bx			; bx - remiender
		je .CASE2
		.CASE1:
			shr     dx, 0x4
			ret
		.CASE2:
			mov	cx, 0x0FFF	; is it the last cluster?
			and	dx, cx
			ret

;----------------------------------------------------------
; Load all the cluster of the image to memory
;----------------------------------------------------------
LOAD_IMAGE:
	mov	dx, WORD[startcluster]			; start cluster of the image
	.LOAD_CLUSTER:
		mov	WORD[nextcluster], dx		; get the cluster number to do some operations
		mov	cx, dx				; for decrement
		dec	cx				; decrease by 1
		dec	cx				; decrease by 1 again
		xor	ax, ax
		mov	al, BYTE[offsetdataregion]	; sector's offset to data region
		add	ax, cx				; offset data region + position of the cluster
		mov	BYTE[sectorsoffset], al		; how many sector's offset ?
		mov	BYTE[sectorstoread], 0x1	; one sector per time - we are loading the image
		mov	ax, WORD[countersectors]	; the counter for address
		inc	ax				; increment the counter for address
		mov	WORD[countersectors], ax	; store counter for address
		mov	bx, WORD[bpbBytesPerSector]	; how many bytes per sector
		mul	bx				; 
		mov     bx, KERNELSTART-0x200		; the base address - we are loading the image to KERNELSTART
		add	bx, ax				; memory position to put the sector
		CALL	READ_SECTORS			; load the part of the image to memory
		mov	dx, WORD[nextcluster]		; get the last cluster
		CALL	GET_NEXT_CLUSTER
		mov	cx, 0x0FFF			; last cluster
		cmp	cx, dx
		jne	.LOAD_CLUSTER			; if not equal, get next
		ret

;----------------------------------------------------------
; pass the start cluster 
;----------------------------------------------------------
LOAD_FAT:
	mov	ax, WORD[bpbSectorsPerFAT]	; ax - bpbSectorsPerFAT sectors
	mov	BYTE[sectorstoread], al		; how many sector to be read?
	mov	ax, WORD[bpbReservedSectors]	; Offset to the FAT
	mov	BYTE[sectorsoffset], al		; how many bytes of offset?
        mov     bx, TEMPMEM			; the address to load the FAT
	call	READ_SECTORS			; load the FAT to memory
	ret

;----------------------------------------------------------
; get the cluster of the entry RDT - ax
;----------------------------------------------------------
GET_CLUSTER:
	mov	di, ax				; put in di for get the value from address
	mov	ax, [di + 0x001A]   		; 26B of offset for get the cluster number
	mov	WORD[startcluster], ax		; save the first cluster of the file
	ret	
;----------------------------------------------------------
; search the stage2 in the root directory
;----------------------------------------------------------
FIND:
        mov     cx, WORD[bpbRootEntries]	; number of entrys in RDT
        mov     ax, TEMPMEM			; address of RDT in memory

        .TRY_AGAIN:
                mov     di, ax          	; es:di - RDT
                mov     si, fileimg	  	; ds:si - file to be found
                push    cx			; salve the number of entrys in RDT
                mov     cx, 0xb         	; cx is used by rep. repet cx times
                cld                     	; clear the direction
                repe	cmpsb			; cmps byte es:di ds:si
                je	.FOUND			; if the file was found - jump to .FOUND
		pop     cx			; get the number of entrys in RDT
                dec     cx			; decrease by 1 the number of entrys in RDT 
                cmp     cx, 0x0			; couldn't find the file
                je	.NOT_FOUND		; if true, just stop
                add     ax, 0x20		; size of each entry in root directory
                jmp	.TRY_AGAIN		; try another file
        .FOUND:
		pop     cx			; restore de stack for the ret
                call	GET_CLUSTER		; the file was found - get the clusters
		ret
        .NOT_FOUND:
                ret
	
;----------------------------------------------------------
; some variables used
;----------------------------------------------------------

sectorsoffset   	DB      0x00
sectorstoread		DB      0x00
sizerootdir		DB	0x00
offsetrootdir		DB	0x00
absolutesector  	DB      0x00
absolutetrack   	DB      0x00
absolutehead    	DB      0x00
startcluster		DW	0x00
nextcluster		DW	0x00
offsetdataregion	DB	0x00
countersectors		DW	0x00
