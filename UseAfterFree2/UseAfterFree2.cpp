// https://medium.com/@dstindiess/overwriting-data-function-pointers-use-after-free-d47a1c10cdd4
// https://learn.snyk.io/lesson/use-after-free/
// https://fuzzysecurity.com/tutorials/expDev/11.html

////////////////////////////////////////////////////////////////////////////////////
// cl /c /GS- UseAfterFree2.cpp
// link UseAfterFree2.obj /NXCOMPAT:NO /DYNAMICBASE:NO
////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <cstdlib>
#include <iostream>


void printa(uintptr_t* data)
{
	printf("item_a data: %llu\n", *data);
}

void printb(uintptr_t data)
{
	printf("item_b data: %llu\n", data);
}

struct item_a
{
	uintptr_t* data;
	bool canWin;
	void (*print)(uintptr_t* data);
	char buff[4096];
};

struct item_b
{
	uintptr_t data;
	bool canWin;
	void (*print)(uintptr_t data);
	char buff[4096];
};


item_a* init_item_a(uintptr_t initial)
{
	item_a* itemA = (item_a*)malloc(sizeof(*itemA));
	if (!itemA)
	{
		return NULL;
	}

	itemA->data = (uintptr_t*)malloc(sizeof(itemA->data));
	if (!itemA->data)
	{
		return NULL;
	}

	*(uint64_t*)itemA->data = initial;

	itemA->print = printa;
	itemA->canWin = false;
	return itemA;
}

void print_res(item_a* itemA)
{
	printf("item_a: %llu\n", *(uint64_t*)itemA->data);
}

void set_res(item_a* itemA, uintptr_t val)
{
	*(uint64_t*)itemA->data = val;
}

uintptr_t get_choice()
{
	char buff[1024] = { 0 };
	uintptr_t choice = 0;
	printf("Input: ");
	fgets(buff, sizeof(buff), stdin);
	choice = strtoull(buff, NULL, 10);

	return choice;
}


int main()
{
	item_a* itemA = NULL;
	item_b* itemB = NULL;
	uintptr_t choice = 0;
	bool done = false;


	void (*win)(void) = NULL;

	// 0x00000001400011fa : jmp rax

	for (;;)
	{
		printf("item_a: %p\n", itemA);
		printf("item_b: %p\n", itemB);
		puts("[1] alloc item_a");
		puts("[2] set item_a->data");
		puts("[3] print item_a->data");
		puts("[4] free item_a");
		puts("[5] alloc item_b");
		puts("[6] set item_b.data");
		puts("[7] print item_b.data");
		puts("[8] free item_b");
		puts("[9] win");

		choice = get_choice();
		switch (choice)
		{
		case 1:
			itemA = init_item_a(0);
			break;
		case 2:
			choice = get_choice();
			*itemA->data = choice;
			break;
		case 3:
			itemA->print(itemA->data);
			Sleep(1000);
			break;
		case 4:
			free(itemA->data);
			free(itemA);
			break;
		case 5:
			itemB = (item_b*)malloc(sizeof(*itemB));
			itemB->data = 0;
			itemB->print = printb;
			itemB->canWin = false;
			break;
		case 6:
			choice = get_choice();
			itemB->data = choice;
			break;
		case 7:
			itemB->print(itemB->data);
			Sleep(1000);
		case 8:
			free(itemB);
			break;
		case 9:
			if (itemB && itemB->canWin)
			{
				for (int i = 0; i < 100; i++)
				{
					puts("You win!");
				}
				Sleep(1000);
				return 0;
			}
			else
			{
				puts("Uh-oh, you haven't won yet");
				Sleep(1000);
			}
			break;

		default:
			puts("Invalid Option");
			Sleep(1000);
			break;
		}

		
	}

}