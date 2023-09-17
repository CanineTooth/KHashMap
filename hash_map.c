#include "hash_map.h"

#define AVL_HASH_TAG 'hsah'

FAST_MUTEX m_hash_mutex = { 0 };
RTL_AVL_TABLE m_avl_table = { 0 };

RTL_GENERIC_COMPARE_RESULTS CompareHashRoutine(
	_In_ struct _RTL_AVL_TABLE* Table,
	_In_ PVOID FirstStruct,
	_In_ PVOID SecondStruct
)
{
	UNREFERENCED_PARAMETER(Table);
	HASH_DATA* p1 = (HASH_DATA*)FirstStruct;
	HASH_DATA* p2 = (HASH_DATA*)SecondStruct;
	if (p1->hash == p2->hash)
	{
		return GenericEqual;
	}
	return p1->hash > p2->hash ? GenericGreaterThan : GenericLessThan;
}

PVOID AllocateHashRoutine(
	_In_ struct _RTL_AVL_TABLE* Table,
	_In_ CLONG ByteSize
)
{
	UNREFERENCED_PARAMETER(Table);
	return ExAllocatePoolWithTag(PagedPool, ByteSize, AVL_HASH_TAG);
}

VOID FreeHashRoutine(
	_In_ struct _RTL_AVL_TABLE* Table,
	PVOID Buffer
)
{
	UNREFERENCED_PARAMETER(Table);
	ExFreePoolWithTag(Buffer, AVL_HASH_TAG);
}

VOID InitHashMap()
{
	ExInitializeFastMutex(&m_hash_mutex);
	RtlInitializeGenericTableAvl(&m_avl_table,
		CompareHashRoutine,
		AllocateHashRoutine,
		FreeHashRoutine,
		NULL);
}

BOOLEAN DeleteElememt(ULONG64 Hash)
{
	ExAcquireFastMutex(&m_hash_mutex);
	
	ULONG64 buffer = Hash;
	BOOLEAN result = RtlDeleteElementGenericTableAvl(&m_avl_table, &buffer);

	ExReleaseFastMutex(&m_hash_mutex);
	return result;
}

HASH_DATA* InsertElement(HASH_DATA* Data)
{
	ExAcquireFastMutex(&m_hash_mutex);

	PVOID result = RtlInsertElementGenericTableAvl(&m_avl_table,
		Data,
		sizeof(HASH_DATA),
		NULL);

	ExReleaseFastMutex(&m_hash_mutex);
	return result;
}

HASH_DATA* LookupElementWithLock(ULONG64 Hash)
{
	ExAcquireFastMutex(&m_hash_mutex);

	ULONG64 buffer = Hash;
	PVOID result = RtlLookupElementGenericTableAvl(&m_avl_table, &buffer);

	ExReleaseFastMutex(&m_hash_mutex);
	return result;
}

HASH_DATA* LookupElementWithoutLock(ULONG64 Hash)
{
	ULONG64 buffer = Hash;
	PVOID result = RtlLookupElementGenericTableAvl(&m_avl_table, &buffer);
	return result;
}

ULONG NumberOfElement()
{
	ExAcquireFastMutex(&m_hash_mutex);

	ULONG result = RtlNumberGenericTableElementsAvl(&m_avl_table);

	ExReleaseFastMutex(&m_hash_mutex);
	return result;
}

HASH_DATA* GetElementByIndex(ULONG Index)
{
	ExAcquireFastMutex(&m_hash_mutex);

	PVOID result = RtlGetElementGenericTableAvl(&m_avl_table, Index);

	ExReleaseFastMutex(&m_hash_mutex);
	return result;
}

VOID EmptyElement()
{
	ExAcquireFastMutex(&m_hash_mutex);

	PVOID p;
	while ((p = RtlGetElementGenericTableAvl(&m_avl_table, 0)) != NULL)
	{
		RtlDeleteElementGenericTableAvl(&m_avl_table, p);
	}
	ExReleaseFastMutex(&m_hash_mutex);
}

VOID EnumerateElement()
{
	ExAcquireFastMutex(&m_hash_mutex);
	for (PVOID p = RtlEnumerateGenericTableAvl(&m_avl_table, TRUE);
		p != NULL;
		p = RtlEnumerateGenericTableAvl(&m_avl_table, FALSE))
	{
		// do something
	}
	ExReleaseFastMutex(&m_hash_mutex);
}