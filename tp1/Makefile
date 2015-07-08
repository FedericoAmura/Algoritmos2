CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g
EXEC=tp1
OBJ=lista.o comandos.o
PADRON=95202
ARCHIVOS=*.h *.c #Tipos de archivos que se van a agregar
CUATRIMESTRE=2013-2C
ENTREGA=TP1
ENCODING=ISO-8859-1
OUTPUTFILE=out.ps #archivo intermedio
FORMATO=landscape #portrait o landscape (vertical u horizontal)
COLUMNAS=1 #paginas por hoja (en columnas)
NUMEROS_LINEA=2 #cada cuantas lineas se imprime el numero de linea
ARCHIVO_ENTREGA=$(PADRON)-$(ENTREGA)
NOMBRE_ZIP=$(ARCHIVO_ENTREGA).zip
NOMBRE_PDF= $(ARCHIVO_ENTREGA).pdf
ENCABEZADO="[75.41] Algoritmos y Programacion II"
PIE="Padrón $(PADRON) (curso $(CUATRIMESTRE)) Entrega: $(ENTREGA)"

all: $(EXEC)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(EXEC): $(OBJ)  tp1.c
	$(CC) $(CFLAGS) $(OBJ) tp1.c -o $(EXEC)

clean:
	rm $(OBJ) $(EXEC)

pdf: clean
	a2ps $(ARCHIVOS) -Av --header=$(ENCABEZADO) --footer=$(PIE) --line-numbers=$(NUMEROS_LINEA) --borders=yes --columns=$(COLUMNAS)  --$(FORMATO) --output=$(OUTPUTFILE) --encoding=$(ENCODING) --tabsize=4 --major=columns --toc  | ps2pdf $(OUTPUTFILE) $(NOMBRE_PDF)
	rm *.ps #elimino los archivos temporales
 
entrega: pdf
	zip $(NOMBRE_ZIP) $(ARCHIVOS) *.pdf
 
clean_entrega: clean
	rm *.zip *.pdf
