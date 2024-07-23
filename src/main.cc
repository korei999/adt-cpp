#include <stdio.h>

#include "adt/Arena.hh"
#include "adt/String.hh"
#include "adt/HashMap.hh"

int
main()
{
    adt::Arena arena(adt::SIZE_1M);

    auto s0 = adt::makeString(&arena, "what");
    auto s1 = adt::makeString(&arena, "tahw");

    adt::HashMap<adt::String> hm0(&arena, adt::SIZE_1K);
    hm0.insert(s0);
    hm0.insert(s1);
    hm0.insert("yes");

    auto f0 = hm0.search("what");
    if (f0.pData) printf("f0: '%.*s'\n", f0.pData->size, f0.pData->pData);
    else printf("f0: '%s'\n", "not found");

    auto f1 = hm0.search("no");
    if (f1.pData) printf("f1: '%.*s'\n", f1.pData->size, f1.pData->pData);
    else printf("f1: '%s'\n", "not found");

    arena.freeAll();
    arena.destroy();
}
