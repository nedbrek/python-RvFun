#!/usr/bin/env python3

import rvfun
rv = rvfun.rvfun()
rv.set_elf("unix.exe")
rv.finalize()
rv.run(100)

