// https://medium.com/@dstindiess/overwriting-data-function-pointers-use-after-free-d47a1c10cdd4
// https://learn.snyk.io/lesson/use-after-free/
// https://fuzzysecurity.com/tutorials/expDev/11.html

////////////////////////////////////////////////////////////////////////////////////
// cl /c /GS- UseAfterFree.cpp
// link UseAfterFree.obj /NXCOMPAT:NO /DYNAMICBASE:NO
////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <iostream>


struct item_a
{
	uint64_t data;
	void (*fp) (uint64_t);
	char b[4096];
};

struct item_b
{
	void (*fp) (uint64_t);
	uint64_t data;
	char b[4096];
};



void win()
{
	puts("You win");
	exit(1);
}

void print_data(uint64_t data)
{
	printf("Data: %llu\n", data);
}

item_a *create_item_a(uint64_t data)
{
	item_a *item = (item_a*)malloc(sizeof(*item));
	item->data = data;
	item->fp = print_data;

	return item;
}

item_b *create_item_b(uint64_t data)
{
	item_b *item = (item_b*)malloc(sizeof(*item));
	item->data = data;
	item->fp = print_data;

	return item;
}

void menu()
{
	puts("[1] create a item_a");
	puts("[2] create a item_b");
	puts("[3] free a item_a");
	puts("[4] free a item_b");
	puts("[5] print item_a data");
	puts("[6] print item_b data");
}

int main()
{
	item_a* aItem = NULL;
	item_b* bItem = NULL;
	char buff[1024] = { 0 };
	uint64_t choice = 0;

	printf("%llu - %llu\n", sizeof(item_a), sizeof(item_b));

	menu();
	for (;;)
	{
		printf("Win: %p\n", win);
		printf("A: %p\n", aItem);
		printf("B: %p\n", bItem);
		memset(buff, 0, sizeof(buff));
		printf("Input: ");
		fgets(buff, sizeof(buff), stdin);
		choice = strtol(buff, NULL, 10);

		switch (choice)
		{
		case 1:
			printf("Data: ");
			fgets(buff, sizeof(buff), stdin);
			choice = strtol(buff, NULL, 10);
			aItem = create_item_a(choice);
			break;
		case 2:
			printf("Data: ");
			fgets(buff, sizeof(buff), stdin);
			choice = strtoll(buff, NULL, 10);
			bItem = create_item_b(choice);
			break;
		case 3:
			free(aItem);
			break;
		case 4:
			free(bItem);
			break;
		case 5:
			aItem->fp(aItem->data);
			break;
		case 6:
			bItem->fp(bItem->data);
			break;
		}
	}


}