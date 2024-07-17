#include <stdio.h>

#include "adt/Arena.hh"
#include "adt/Array.hh"
#include "adt/String.hh"
#include "adt/HashMap.hh"

int
main()
{
    adt::Array<adt::StringView<char>> aS;
    aS.push("what");

    adt::String kekw = "kekw";
    adt::String kekw2 = kekw;

    adt::HashMap<adt::String<char>> hmS{};

    printf("aS.back: '%s'\n", aS.back()->pData);

    aS.free();
    hmS.free();
    kekw.free();
    kekw2.free();
}
