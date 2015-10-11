#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLENGTH 200
typedef struct Line
{
	int lineNumber;
	char lineContent[200];
	struct Line * next;
}Line;
main()
{
	FILE *fp;
	errno_t err;
	int i=0, j=0, k;
	char str[MAXLENGTH];
	Line * lp = NULL, *p = NULL;
	char filename[] = "D:\\test.c";
	// Open for read (will fail if file "D:\\test.c" does not exist).
	err = fopen_s(&fp, filename, "r");
	if (err == 0)
	{
		printf("The file 'test.c' was opened.\n");
	}
	else
	{
		printf("The file 'test.c' was not opened.\n");
	}
	lp = (Line *)malloc(sizeof(Line));
	p = lp;
	//
	while (!feof(fp))
	{
		i++;
		fgets(str, MAXLENGTH, fp);
		strcpy_s(p->lineContent, MAXLENGTH,str);
		p->lineNumber = i;
		p->next = (Line *)malloc(sizeof(Line));
		p = p->next;
	}
	free(p);
	p = lp;
	printf("Please comform the content of the file.\n");
	for (j = 0; j < i; j++)
	{
		printf("%d   %s", p->lineNumber, p->lineContent);
		p = p->next;
	}
	printf("\n");
	// Close fp if it is not NULL.
	if (fp)
	{
		err = fclose(fp);
		if (err == 0)
		{
			printf("The file 'test.c' was closed.\n");
		}
		else
		{
			printf("The file 'test.c' was not closed.\n");
		}
	}
	getchar();
}