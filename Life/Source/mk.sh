if test -d ~/bin
then
    echo "~/bin already exists"
else
    echo "creating ~/bin"
    mkdir ~/bin
fi
if test -f info2.cpp
then
   rm info2.cpp
fi
rm regexp/*.o
rm *.o
rm wild_life


cd regexp
make >djd3 2>djd4
cd ..
$EDITOR regexp/djd3 regexp/djd4
make >djd1 2>djd2
$EDITOR djd1 djd2
if test -f wild_life.exe
then	
    cp wild_life.exe ~/bin
fi
if test -f wild_life
then
    cp wild_life ~/bin
fi
if test -f wild_life_X++
then
    cp wild_life_X++ ~/bin
fi

