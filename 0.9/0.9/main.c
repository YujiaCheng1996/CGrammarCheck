#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINELENGTH 200
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
	int fileLineNumber = 0, lineLength = 0;
	int i = 0, j = 0, k = 0, t = 0, q = 0;
	int braceCount = 0;
	int parenthesisCount = 0, squareBracketCount = 0, angleBracketCount = 0, singleQuotationMarkCount = 0, doubleQuotationMarkCount = 0;
	int wrongNumber = 0;
	int multilineCommentsFlag = 0, multilineCommentsLineFlag = 0;
	char str[MAXLINELENGTH];
	Line * lp = NULL, *p = NULL;
	char filename[] = "D:\\test.c";
loop:;// Open for read (will fail if file "D:\\test.c" does not exist).
	err = fopen_s(&fp, filename, "r");
	if (err == 0)
	{
		printf("The file 'test.c' was opened.\n");
	}
	else
	{
		printf("The file 'test.c' was not opened.\n");
		goto loop;
	}
	//Create a linked list to store each line and line number.
	lp = (Line *)malloc(sizeof(Line));
	p = lp;
	while (!feof(fp))
	{
		fileLineNumber++;
		fgets(str, MAXLINELENGTH, fp);
		if (str[strlen(str) - 1] == '\n')
		{
			str[strlen(str) - 1] = '\0';
		}
		strcpy_s(p->lineContent, MAXLINELENGTH, str);
		p->lineNumber = fileLineNumber;
		p->next = (Line *)malloc(sizeof(Line));
		p = p->next;
	}
	free(p);
	//Put out the file with line number attached.
	p = lp;
	printf("Please comform the content of the file.\n");
	for (i = 0; i < fileLineNumber; i++)
	{
		printf("%4d   %s\n", p->lineNumber, p->lineContent);
		p = p->next;
	}
	//Let user conform the file(will reload if users does not press y ).
	printf("Press y to conform the file or any other key to load another file:");
	if (getchar() == 'y')
	{
		getchar();
		printf("The file has been conformed.\n");
	}
	else {
		getchar();
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
		goto loop;
	}
	//Delete all the comments.
	printf("Does the file have comments?\nPress y to conform or any other key to save the process time.:");
	if (getchar() != 'y')
	{
		getchar();
	}
	else {
		getchar();
		printf("The file after the commets deleted are listed below.\n");
		p = lp;
		for (i = 0; i < fileLineNumber; i++)
		{
			lineLength = strlen(p->lineContent);
			for (j = 0; j < lineLength; j++)
			{
				if (multilineCommentsFlag)
				{
					if (p->lineContent[j] == '*' && p->lineContent[j + 1] == '/')
					{
						if (multilineCommentsLineFlag == p->lineNumber)
						{
							for (k = j + 2, t = multilineCommentsFlag - 1; k < lineLength + 1; k++)
							{
								p->lineContent[t++] = p->lineContent[k];
							}
							j= multilineCommentsFlag - 1;
						}
						else
						{
							for (k = j + 2, t = 0; k < lineLength + 1; k++)
							{
								p->lineContent[t++] = p->lineContent[k];
							}
						}
						multilineCommentsFlag = 0;
					}
					if (p->lineContent[j + 1] == '\0')
					{
						if (multilineCommentsLineFlag == p->lineNumber)
						{
							p->lineContent[multilineCommentsFlag - 1] = '\0';
						}
						else
						{
							p->lineContent[0] = '\0';
						}
					}
				}
				else if (p->lineContent[j] == '/')
				{
					if (p->lineContent[j + 1] == '/')
					{
						p->lineContent[j] = '\0';
						break;
					}
					else if (p->lineContent[j + 1] == '*')
					{
						multilineCommentsFlag = j + 1;
						multilineCommentsLineFlag = p->lineNumber;
					}
				}
			}
			printf("%4d   %s\n", p->lineNumber, p->lineContent);
			p = p->next;
		}
	}
	//Check the grammar mistakes.
	p = lp;
	for (i = 0; i < fileLineNumber; i++)
	{
		strcpy_s(str, MAXLINELENGTH, p->lineContent);
		lineLength = strlen(str);
		parenthesisCount = squareBracketCount = angleBracketCount = singleQuotationMarkCount = doubleQuotationMarkCount = 0;
		for (j = 0; j < lineLength; j++)
		{
			switch ((int)str[j])
			{
			case '{':braceCount++; break;
			case '}':braceCount--; break;
			case '(':parenthesisCount++; break;
			case ')':if (parenthesisCount < 1) { printf("Missing \'(\' before \')\' in line %d.\n", i + 1); wrongNumber++; }parenthesisCount--; break;
			case '[':squareBracketCount++; break;
			case ']':if (squareBracketCount < 1) { printf("Missing \'[\' before \']\' in line %d.\n", i + 1); wrongNumber++; }squareBracketCount--; break;
			case '<':angleBracketCount++; break;
			case '>':if (angleBracketCount < 1) { printf("Missing \'[\' before \']\' in line %d.\n", i + 1); wrongNumber++; }angleBracketCount--; break;
			case '\'':singleQuotationMarkCount++; break;
			case '\"':doubleQuotationMarkCount++; break;
			}
		}
		if (parenthesisCount)
		{
			printf("Missing \')\' in line %d.\n", i + 1);
			wrongNumber++;
		}
		if (squareBracketCount)
		{
			printf("Missing \']\' in line %d.\n", i + 1);
			wrongNumber++;
		}
		if (angleBracketCount)
		{
			printf("Missing \'>\' in line %d.\n", i + 1);
			wrongNumber++;
		}
		if (singleQuotationMarkCount % 2)
		{
			printf("Missing \'\'\' in line %d.\n", i + 1);
			wrongNumber++;
		}
		if (doubleQuotationMarkCount % 2)
		{
			printf("Missing \'\"\' in line %d.\n", i + 1);
			wrongNumber++;
		}
		p = p->next;
	}
	if (braceCount > 0)
	{
		printf("Missing %d \'}\' in total.\n", braceCount);
		wrongNumber += braceCount;
	}
	else if (braceCount < 0)
	{
		printf("Missing %d \'{\' in total.\n", braceCount*-1);
		wrongNumber -= braceCount;
	}
	printf("%d wrong(s) in total.\nMission completed.\n", wrongNumber);
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