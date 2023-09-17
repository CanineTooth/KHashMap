#pragma once
#include <ntifs.h>

typedef struct _HASH_DATA
{
	ULONG64 hash;
	ULONG64 data;
	// add your data
}HASH_DATA, * PHASH_DATA;

VOID InitHashMap();

HASH_DATA* InsertElement(HASH_DATA* Data);

BOOLEAN DeleteElememt(ULONG64 Hash);

HASH_DATA* LookupElementWithLock(ULONG64 Hash);

HASH_DATA* LookupElementWithoutLock(ULONG64 Hash);

ULONG NumberOfElement();

HASH_DATA* GetElementByIndex(ULONG Index);

VOID EmptyElement();