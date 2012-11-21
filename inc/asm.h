/* Assembly for get_pid function */
asm(
    ".text\n"
    ".align 4\n"
    ".global get_pid\n"
    "get_pid:\n"
    "        mov  %asr17, %o0\n"
    "        srl  %o0, 28, %o0\n"
    "        retl\n"
    "        and %o0, 0xf, %o0\n"
    );
