#include <stdio.h>

#include "adt/Arena.hh"
#include "adt/Array.hh"
#include "adt/String.hh"
#include "adt/HashMap.hh"

int
main()
{
    adt::Array<adt::StringView> aS;
    aS.push("what");

    adt::String kekw = "kekw";
    adt::String kekw2 = kekw;
    printf("kekw2: '%s'\n", kekw2.pData);

    aS.push(kekw);

    adt::HashMap<adt::StringView> hmS{};
    hmS.insert(kekw);
    auto fs = hmS.search("kekw");
    if (fs.pData)
        printf("fs: '%s'\n", fs.pData->pData);
    else
        printf("fs: not found\n");

    printf("aS.back: '%s'\n", aS.back().pData);

    aS.free();
    hmS.free();
    kekw.free();
    kekw2.free();
}
