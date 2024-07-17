#include <stdio.h>

#include "adt/arena.hh"
#include "adt/array.hh"

int
main()
{
    adt::Arena a(50);

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
    /*printf("od: %zu\n", od);*/

    auto pAData = ARENA_NODE_GET_FROM_DATA(kekw);
    printf("pAData.size: %zu\n", pAData->size);

    adt::Array<int, adt::Arena> arr(16, a);
    arr.push(1);
    arr.push(2);
    arr.push(3);

    printf("back(): %d\n", *arr.back());
    printf("arr[1]: %d\n", arr[1]);
}
