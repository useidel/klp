klp - live-patching
============

A different sample for the Linux Kernel live patching 

The instructions in OPROC should allow a start

You need a 4.x kernel ... an RC release of 4.0.0 is good enough.

The kernel configuration must have the following options set

 CONFIG_HAVE_LIVEPATCH=y
 CONFIG_LIVEPATCH=y
 CONFIG_SAMPLE_LIVEPATCH=m



