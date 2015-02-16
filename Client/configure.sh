#! /bin/bash
echo -en  "all: $1 \n\n$1: " > "Makefile"
for f in *.c
do
  echo -en "`basename $f .c`.o " >> "Makefile"
done
echo -en "\n\tgcc -Wall " >> "Makefile"
for f in *.c
do
echo -en "`basename $f .c`.o " >> "Makefile";
done
echo " -o $1" >> "Makefile";
for f in *.c
do
  echo `gcc -MM $f` >> "Makefile"
  echo -e "\tgcc -c -Wall $f -o `basename $f .c`.o" >> "Makefile"
done
echo -e "\nclean:" >> "Makefile"
echo -e "\trm *.o" >> "Makefile"
echo -e "\trm $1" >> "Makefile"
