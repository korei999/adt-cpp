#include <stdio.h>

#include "adt/Arena.hh"
#include "adt/Array.hh"
#include "adt/String.hh"
#include "adt/HashMap.hh"

int
main()
{
    adt::Arena a(adt::SIZE_1M);

    char* kekw = (char*)a.alloc(5, 1);
    strncpy(kekw, "kekw", 5);

    char* next = (char*)a.alloc(5, 1);
    strncpy(next, "next", 5);

    char* next2 = (char*)a.alloc(6, 1);
    strncpy(next2, "next2", 6);

    char* next3 = (char*)a.alloc(6, 1);
    strncpy(next3, "next3", 6);

    next = (char*)a.realloc(next, 7);

    printf("kekw: '%s'\n", kekw);
    printf("next: '%s'\n", next);
    printf("next2: '%s'\n", next2);
    printf("next3: '%s'\n", next3);

    printf("next - kekw = %zu\n", next - kekw);

    auto od = offsetof(adt::ArenaNode, pData);
    printf("od: %zu\n", od);

    auto pAData = ARENA_NODE_GET_FROM_DATA(kekw);
    printf("pAData.size: %zu\n", pAData->size);

    adt::Array<int> arr(4);
    arr.push(1);
    arr.push(2);
    arr.push(3);
    arr.push(4);
    arr.push(5);

    printf("back(): %d\n", *arr.back());
    printf("arr[1]: %d\n", arr[1]);

    adt::String ss = "ss";
    adt::String ssss = ss + ss;
    printf("ss: '%s'\n", ss.pData);
    printf("ssss: '%s'\n", ssss.pData);

    adt::String ssa("ssa", a);
    adt::String asa("asa", a);
    printf("ssa: '%s'\n", ssa.pData);
    ssa += asa;
    printf("ssa: '%s'\n", ssa.pData);

    adt::String add = "add";
    adt::String string = "string";
    add += string;
    printf("add: '%s'\n", add.pData);

    adt::HashMap<int> hm0{adt::SIZE_MIN, adt::HASHMAP_DEFAULT_LOAD_FACTOR};
    hm0.insert(1);
    hm0.insert(2);
    hm0.insert(55);
    printf("loadFactor: %lf\n", hm0.loadFactor());

    auto f5 = hm0.search(55);
    if (f5.pData) printf("f5: %d\n", *f5.pData);
    else printf("f5: not found\n");

    auto f2 = hm0.search(2);
    if (f2.pData) printf("f2: %d\n", *f2.pData);
    else printf("f2: not found\n");

    adt::String stry1 = "stry1";
    adt::String stry2 = "stry2";
    adt::String stry3 = stry1;

    stry3 = stry2;
    printf("stry3: '%s'\n", stry3.pData);

    adt::HashMap<adt::String<char>> hm1{};
    hm1.insert("what");
    /*hm1.insert("is");*/

    auto fwhat = hm1.search("what");
    if (fwhat.pData) printf("fwhat: '%s'\n", fwhat.pData->pData);
    else printf("fwhat: not found\n");

}
