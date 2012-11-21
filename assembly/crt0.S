/*

    LEON2/3 LIBIO low-level routines 
    Written by Jiri Gaisler.
    Copyright (C) 2004  Gaisler Research AB

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

	.text
	.global _start, main, _end

_start:
	! set stack pointer dependent on pid
	rd     %asr17,%g5
	srl    %g5,28,%g5 ! g5 is the pid
	sethi %hi(-1024000), %g1 ! 1M 
	smul %g5, %g1, %g1
	save	%sp, %g1, %sp ! offset stack by pid * g5
#ifndef _FLAT
	save	%sp, -64, %sp
#else
	add	%sp, -72, %sp
	st	%o7, [%sp+64]
#endif

	! if pid == 0 then do this, otherwise call main
	! stops other cpus resetting .bss section
	cmp %g5, 0
	bne jumptomain

	/* clear the bss */
        sethi %hi(__bss_start),%g2
        or    %g2,%lo(__bss_start),%g2  ! g2 = start of bss
        sethi %hi(_end),%g3
        or    %g3,%lo(_end),%g3         ! g3 = end of bss
        mov   %g0,%g1                   ! so std has two zeros
	sub   %g3, %g2, %g3
zerobss:
	subcc  %g3, 8, %g3
	bge,a   zerobss
	std    %g0,[%g2+%g3]

	set    _end, %o0
	st     %g0,[%o0]

	call	bdinit2
	 nop

	call	prelibchook
	 nop

	call	_call_initcalls        /* atexit uses __atexit lock */
	 nop
	
	set	_fini, %o0
	call	atexit, 1
	 nop
	
	call	_init
	 nop
jumptomain:
	call    main
         nop
	call _exit
	nop
#ifndef _FLAT
	ret
	restore
#else
	ld	[%sp+64], %o7
	retl
	add	%sp, 72, %sp
#endif

        .seg    "data"
        .global .bdata
.bdata:
        .align  8
        .global _environ
_environ:
        .word   1
 


