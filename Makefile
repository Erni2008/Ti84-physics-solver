NAME = PHYSOLVE
DESCRIPTION = "Physics Formula Solver"
COMPRESSED = YES

CFLAGS = -Wall -Wextra -Oz -Iinclude
LDFLAGS = -lm

include $(shell cedev-config --makefile)
