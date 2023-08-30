#include <Windows.h>
#include <stdio.h>
/// <summary>
/// Demo Process for unit testing.  Takes 1 argument. Sleeps 
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[])
{
	if (sizeof(VOID*) == 4)
	{
		fprintf(stdout, "x86 code");
	}
	else
	{
		fprintf(stdout, "x64 code");
	}
	if (argc <= 1)
	{
		fprintf(stdout, "Sleeping for 4 seconds\r\n");
		Sleep(4000);
		return 4000;
	}
	else
	{
		int result = atoi(argv[1]);
		if (result < 100)
			result = 100;
		if (result > 5000)
			result = 1000;
		fprintf(stdout, "Sleeping for %d milliseconds", result);
		Sleep(result);
		return result;
	}
}