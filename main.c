#include <stdio.h>
#include <string.h>
#include <ctype.h>

FILE *fr;
FILE *fw;

char lines[100][40];
char dt[100][10];
char vn[100][32];
char temp[32];

void split(char *str, int i){
	char * input;
	input = strtok(str," ");
	sprintf(dt[i],"%s",input);
	input = strtok(NULL," ");
	sprintf(vn[i],"%s", input);
}

char * proper(char *str){
	sprintf(temp,"%s",str);
	temp[0] = toupper(temp[0]);
	return temp;
}

char * toupperString(char *str){
	char *ptr;
	sprintf(temp,"%s",str);
	ptr = temp;
	while(*ptr)
	{
		*ptr = toupper(*ptr);
		ptr++;
	}
	return temp;
}


int main(int argc, char *argv[])
{
	int i,linesread;
	char *input;
	int len;


	if(argc == 1)
		printf("Usage: ./qtclass-gen [input_file]\r\n");
	else
	{
		input = argv[1];
		printf("Opening file: %s\r\n",argv[1]);
		fr = fopen(input,"rt");
		if(fr == NULL)
		{
			printf("File can't be read.\r\n");
			return 0;
		}
		sprintf(temp,"%s.h",input);
		//input = argv[2];
		printf("Creating file: %s\r\n",temp);
		fw = fopen(temp,"w");
		linesread = 0;

		while(fgets(lines[linesread],80,fr) != NULL)
		{	
			// remove newline
			len = strlen(lines[linesread]);
			input = &lines[linesread][len - 1];
			memset(input,0,2);
			linesread++;
		}
		printf("Lines read: %d\r\n",linesread);
		for(i = 0; i < linesread; i++)
		{
			split(lines[i],i);
		}

		printf("Writing to file...");

		fprintf(fw,"#ifndef %s_H\r\n",toupperString(argv[1]));
		fprintf(fw,"#define %s_H\r\n",toupperString(argv[1]));
		fprintf(fw,"\r\n");
		fprintf(fw,"#include <QObject>\r\n");
		fprintf(fw,"\r\n");
		fprintf(fw,"class %s : public QObject\r\n{\r\n",proper(argv[1]));
		fprintf(fw,"\tQ_OBJECT\r\n");
		// Q_PROPERTY

		
		for(i = 0; i < linesread; i++)
		{
			//split(lines[i]);
			fprintf (fw, "\tQ_PROPERTY(%s %s READ %s WRITE set%s NOTIFY %sChanged)\r\n",dt[i],vn[i],vn[i],proper(vn[i]),vn[i]);
		}
		fprintf(fw,"\r\n");




		// public setters
		fprintf(fw,"public:\r\n");

		fprintf(fw, "\t%s(QObject *parent = 0);\r\n",proper(argv[1]));
		fprintf(fw,"\r\n");
		for(i = 0; i < linesread; i++)
		{
			fprintf (fw, "\tvoid set%s(%s %s){if(%s != m_%s){ m_%s = %s; emit %sChanged();}}\r\n",proper(vn[i]),dt[i], vn[i], vn[i], vn[i], vn[i], vn[i], vn[i]);
		}
		fprintf(fw,"\r\n");

		
		// public getters
		for(i = 0; i < linesread; i++)
		{
			fprintf (fw, "\t%s %s(){return m_%s;}\r\n",dt[i], vn[i], vn[i]);
		}
		fprintf(fw,"\r\n");

		// signals
		fprintf(fw,"signals:\r\n");
		for(i = 0; i < linesread; i++)
		{
			fprintf (fw, "\tvoid %sChanged();\r\n",vn[i]);
		}
		fprintf(fw,"\r\n");

		fprintf(fw,"private:\r\n");
		// private members
		for(i = 0; i < linesread; i++)
		{
			fprintf (fw, "\t%s m_%s;\r\n",dt[i], vn[i]);
		}
		fprintf(fw,"\r\n");
		fprintf(fw,"};\r\n");
		fprintf(fw,"#endif //%s_H\r\n",toupperString(argv[1]));
		printf("done!\r\n");
		fclose(fr);
		fclose(fw);

	}
	return 0;
}

