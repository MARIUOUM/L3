JAVAC=javac
SOURCES = $(wildcard *.java)
CLASSES = $(SOURCES:.java=.class)
PARAM = -Xlint
MAIN = MainJeuDeLaVie
MAINIG = MainIGJeuDeLaVie
JVM = java

all: $(CLASSES)

clean :
	rm -f *.class *~

%.class : %.java
	$(JAVAC) -Xlint $<

run: $(MAIN).class
	$(JVM) $(MAIN)

window: $(MAINIG).class
	$(JVM) $(MAINIG)

