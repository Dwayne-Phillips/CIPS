
#  11 February 2016

CC=gcc
CFLAGS=-I.
DEPS=cips.h

hello0: hello0.c
	gcc -o hello0 hello0.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

HISTEQOBJS=histeq.o imageio.o hist.o
histeq: $(HISTEQOBJS)
		$(CC) -o $@ $^ $(CFLAGS)


HIMAGEOBJS=himage.o imageio.o hist.o
himage: $(HIMAGEOBJS)
	$(CC) -o $@ $^ $(CFLAGS)


FINDTOBJS=findt.o imageio.o tiffs.o
findt: $(FINDTOBJS)
			$(CC) -o $@ $^ $(CFLAGS)

HEADEROBJS=header.o imageio.o
header: $(HEADEROBJS)
	$(CC) -o $@ $^ $(CFLAGS)


HALFTONEOBJS=halftone.o ht.o imageio.o
halftone: $(HALFTONEOBJS)
	$(CC) -o $@ $^ $(CFLAGS)


HIDETOBJS=hidet.o imageio.o
hidet: $(HIDETOBJS)
		$(CC) -o $@ $^ $(CFLAGS)

GEOMETRYOBJS=geometry.o geosubs.o imageio.o
geometry: $(GEOMETRYOBJS)
						$(CC) -o $@ $^ $(CFLAGS)


FLIPOBJS=flip.o imageio.o
flip: $(FLIPOBJS)
				$(CC) -o $@ $^ $(CFLAGS)


BMP2TIFFOBJS=bmp2tif.o imageio.o
bmp2tif: $(BMP2TIFFOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

BMPedgeOBJS=bmpedge.o imageio.o
bmpedge: $(BMPedgeOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

DUMPBOBJS=dumpb.o imageio.o
dumpb: $(DUMPBOBJS)
		$(CC) -o $@ $^ $(CFLAGS)


DUMPIOBJS=dumpi.o imageio.o
dumpi: $(DUMPIOBJS)
			$(CC) -o $@ $^ $(CFLAGS)

CREATOBJS=create.o imageio.o
create: $(CREATOBJS)
	$(CC) -o $@ $^ $(CFLAGS)


STRETCHOBJS=stretch.o geosubs.o imageio.o
stretch: $(STRETCHOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MAINOVEROBJS=mainover.o overlay.o imageio.o
mainover: $(MAINOVEROBJS)
	$(CC) -o $@ $^ $(CFLAGS)

BOOLEANOBJS=boolean.o boole.o imageio.o
boolean: $(BOOLEANOBJS)
	$(CC) -o $@ $^ $(CFLAGS)


MAINLOBJS=mainl.o lambert.o imageio.o
mainl: $(MAINLOBJS)
		$(CC) -o $@ $^ $(CFLAGS)

MAINOVEROBJS=mainover.o overlay.o imageio.o
mainover: $(MAINOVEROBJS)
	$(CC) -o $@ $^ $(CFLAGS)


CIPSCATOBJS=cipscat.o
cipscat: $(CIPSCATOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

ISOOBJS=iso.o imageio.o
iso: $(ISOOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

ISO2OBJS=iso2.o imageio.o
iso2: $(ISO2OBJS)
		$(CC) -o $@ $^ $(CFLAGS)

SHOWIOBJS=showi.o imageio.o
showi: $(SHOWIOBJS)
	$(CC) -o $@ $^ $(CFLAGS)


INVERTOBJS=invert.o imageio.o
invert: $(INVERTOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MAINASOBJS=mainas.o imageio.o addsub.o
mainas: $(MAINASOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MAINCPOBJS=maincp.o imageio.o cutp.o
maincp: $(MAINCPOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MAINSEGOBJS=mainseg.o hist.o imageio.o segment.o
mainseg: $(MAINSEGOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MAIN2SEGOBJS=main2seg.o hist.o imageio.o segment.o \
             edge2.o edge3.o edge.o filter.o \
             segment2.o utility.o
main2seg: $(MAIN2SEGOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MAINSKOBJS=mainsk.o imageio.o skeleton.o ed.o utility.o
mainsk: $(MAINSKOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MEDGEOBJS=medge.o imageio.o edges.o utility.o
medge: $(MEDGEOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

MFILTEROBJS=mfilter.o imageio.o filter.o utility.o
mfilter: $(MFILTEROBJS)
	$(CC) -o $@ $^ $(CFLAGS)

SIDEOBS=imageio.o hist.o side.o
side: $(SIDEOBS)
	$(CC) -o $@ $^ $(CFLAGS)

####################### TEST PLATFORM ###############

HELLOOBJS=hello.o hello2.o hello3.o

hello: $(HELLOOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

TESTBEDOBJS=testbed.o imageio.o hist.o
testbed: $(TESTBEDOBJS)
	$(CC) -o $@ $^ $(CFLAGS)

TESTSEGOBJS=testseg.o imageio.o segment.o hist.o
testseg: $(TESTSEGOBJS)
	$(CC) -o $@ $^ $(CFLAGS)


IIIOBJS=iii.o imageio.o segment.o hist.o
iii: $(IIIOBJS)
	$(CC) -o $@ $^ $(CFLAGS)
