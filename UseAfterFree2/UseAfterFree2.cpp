// https://medium.com/@dstindiess/overwriting-data-function-pointers-use-after-free-d47a1c10cdd4
// https://learn.snyk.io/lesson/use-after-free/
// https://fuzzysecurity.com/tutorials/expDev/11.html

////////////////////////////////////////////////////////////////////////////////////
// cl /c /GS- UseAfterFree2.cpp
// link UseAfterFree2.obj /NXCOMPAT:NO /DYNAMICBASE:NO
////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

struct Result
{
	uintptr_t data;
	char buff[4096];
};

struct ToMath
{
	uintptr_t data;
	char buff[4096];
};


Result* init_result(uintptr_t initial)
{
	Result* res = (Result*)malloc(sizeof(*res));
	if (!res)
	{
		return NULL;
	}

	res->data = (uintptr_t)malloc(sizeof(uintptr_t));
	if (!res->data)
	{
		return NULL;
	}

	*(uint64_t*)res->data = initial;

	return res;
}

void print_res(Result* res)
{
	printf("Res: %llu\n", *(uint64_t*)res->data);
}

void set_res(Result* res, uintptr_t val)
{
	*(uint64_t*)res->data = val;
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

void menu1()
{
	puts("[1] new math");
	puts("[2] print result");
	puts("[3] clear result");
}

void menu2()
{
	puts("[1] set val");
	puts("[2] print val");
	puts("[3] add");
	puts("[4] sub");
	puts("[5] clear val");
	puts("[6] back");
}

/*

Alloc res
free res
 - set val
 - math res
print res

*/


int main()
{
	Result* res = NULL;
	ToMath* val = NULL;
	uintptr_t choice = 0;
	bool done = false;


	

	for (;;)
	{
		printf("Result: %p\n", res);
		printf("Val: %p\n", val);
		menu1();

		choice = get_choice();

		switch (choice)
		{
		case 1:
			puts("here");
			done = false;
			//// Menu2 ////
			for (;!done;)
			{
				menu2();

				choice = get_choice();

				switch (choice)
				{
				case 1:
					choice = get_choice();

					val = (ToMath*)malloc(sizeof(*val));
					if (!val)
					{
						exit(1);
					}

					val->data = choice;
					break;
				case 2:
					printf("Val: %llu\n", val->data);
					break;
				case 3:
					*(uint64_t*)res->data += val->data;
					break;
				case 4:
					*(uint64_t*)res->data -= val->data;
					break;
				case 5:
					free(val);
					break;
				case 6:
					done = true;
					break;
				}
			}
			//////////////

			break;
		case 2:
			print_res(res);
			break;
		case 3:
			free(res);
			break;
		default:
			puts("invalid input");

		}
	}


	/*for (;;)
	{
		printf("Result: %p\n", res);
		printf("Val: %p\n", val);
		puts("[1] alloc res");
		puts("[2] alloc val");
		puts("[3] free res");
		puts("[4] free val");

		choice = get_choice();
		switch (choice)
		{
		case 1:
			res = init_result(1);
			break;
		case 2:
			val = (ToMath*)malloc(sizeof(*val));
			break;
		case 3:
			free(res);
			break;
		case 4:
			free(val);
			break;
		}

	}*/



}