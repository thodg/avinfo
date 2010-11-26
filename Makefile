##
##  avinfo  -  audio and video streams info
##
##  Copyright 2010 Thomas de Grivel <billitch@gmail.com>
##
##  Permission to use, copy, modify, and distribute this software for any
##  purpose with or without fee is hereby granted, provided that the above
##  copyright notice and this permission notice appear in all copies.
## 
##  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
##  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
##  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
##  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
##  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
##  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
##  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
##

PREFIX := /usr/local
CC := gcc
CFLAGS += -pthread
CPPFLAGS += -I${PREFIX}/include
LDFLAGS += -L${PREFIX}/lib

PROGRAM = avinfo
SRC = avinfo.c
LIBS = -lavformat -lavcodec -lz

all: ${PROGRAM}

clean:
	rm -f ${PROGRAM}

install: ${PROGRAM}
	install -m 755 ${PROGRAM} ${PREFIX}/bin

${PROGRAM}: ${SRC}
	${CC} ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} ${SRC} ${LIBS} -o ${PROGRAM}
