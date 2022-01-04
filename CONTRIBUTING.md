Contributing
=============

Please make sure to retain IBM 5150 Compatibility when Contributing to this Project.

**This includes:**
- Maintaining a decent speed on an 8088/8086-like CPU
- Maintaining the CGA output
- Keeping the RAM usage as low as possible
  - never exceed 64kB of RAM!
- Keeping the Binary Filesize as low as possible
  - never exceed 160kB of Storage, to maintain Compatibility with PC-DOS 1.0!

If anything can be reworked to be in x86 Assembly, this is permitted to, so long as the majority of the code remains in C. 
This mostly refers to screen-drawing routines and certain mathmatical routines, otherwise the rest is to be kept as C.
