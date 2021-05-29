#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

FILE *openFile(char *file_name);	// open CSV file to read
FILE *createFile();		// create file for SQL
int getCounting(char *names);	// get value counter
char **initMemory(int x, int count);	// memory allocation
void recordingData(char *names, char **data);		// data recording
void getType(char **types, char *string);	// definition of data types
void createNameTable(char *tableName);		// enter the table name
void prepareNewFile(int count, FILE *in, FILE *out, char *tableName, char *names, char **data, char **types, char *string);	// write data to a file

int main()
{
	char string[1000], Flag[100], names[1000], **data, **types, file_name[20];
	FILE *fp, *result;
	fp = openFile(file_name);
	result = createFile();
	
	int i=0, j, k;
	fgets(names, 999, fp);
	names[strlen(names)-1] = 0;
	
	int count;
	count = getCounting(names);
	
	types = initMemory(25, count);
	data = initMemory(100, count);
	
	recordingData(names, data);
	
	fgets(string, 999, fp);
	string[strlen(string)-1]=0;
	
	getType(types, string);
	char tableName[100];
	createNameTable(tableName);
	
	prepareNewFile(count, fp, result, tableName, names, data, types, string);
	printf("Convert file %s complite", file_name);
	
	fclose(fp);
	fclose(result);
	return 0;
}

FILE *openFile(char *file_name){
	FILE *fp;	
	
	printf("Your file must be in the application folder\n");
	printf("Enter the name of the file to convert: ");

	do{
		scanf("%s", file_name);
		fp = fopen(file_name, "r");
		if(fp==NULL){
			printf("File not found, repeat input\n");
		}
	}
	while(fp==NULL);
	return fp;
}

FILE *createFile(){
	FILE *result;
	result = fopen("Data_Base.sql", "w");
	return result;
}

int getCounting(char *names){
	int count=1, i;
	for(i=0; i < strlen(names); i++){	
		if(names[i] == ',' || names[i] == ';'){
			count++;
		}			
	}
	return count;
}

char **initMemory(int x, int count){
	int i=0;
	
	char** data = (char**)calloc(count, sizeof(char*));
	*data = (char*)calloc(x*count, sizeof(char));
	for(i=1; i<count; i++)
		data[i] = data[i-1]+x;
	return data;
}

void recordingData(char *names, char **data){
	int k=0, j=0, i=0;
	
	for(i; i < strlen(names); i++){	
		if(names[i] == ',' || names[i] == ';'){
			data[k][j] = 0;
			k++;
			j=0;
		}
		else{
			data[k][j] = names[i];
	    	j++;
		}
	}
}

void getType(char **types, char *string){
	char flag[100];
	int i=0, j, k;
	
	k=0;
	i=0;
	while(string[i] != 0)
	{
		j = 0;		
		while(string[i] != ',' && string[i] != ';' && string[i] != 0)
		{
			flag[j] = string[i];
			i++;
			j++;
		}
		flag[j]=0;
		int VARCHAR=0, INTEGER=0, DOUBLE=0;
		j = 0;		
		while(flag[j] != 0){
			if((flag[j] < '0' || flag[j] > '9') && (flag[j] != '-') && (flag[j] != '.'))
			{
				VARCHAR = 1;
				break;
			}		
			if(flag[j] == '.')
			{
				DOUBLE = 1;
				break;
			}
			j++;	
		}
		if((VARCHAR != 1) && (DOUBLE != 1))
		{
			INTEGER = 1;
			strcpy(types[k], "INT");
		}
		else 
		if(VARCHAR == 1)
		{
			DOUBLE = 0;
			strcpy(types[k], "VARCHAR(100)");
		}
		else strcpy(types[k], "DOUBLE");
		k++;
		if(string[i] != 0) i++;
	}
}

void createNameTable(char *tableName){
	printf("Enter table name: ");
	scanf("%s", tableName);
}

void prepareNewFile(int count, FILE *in, FILE *out, char* tableName, char* names, char **data, char ** types, char *string){
	int i;
	
	fprintf(out, "CREATE TABLE %s(\n", tableName);
	for(i=0; i<count; i++){
		fprintf(out, "%s %s", data[i], types[i]);
		if(i<count-1) fprintf(out, ",\n");
		else fprintf(out, ");\n\n");
	}
	
	fprintf(out, "INSERT INTO %s (%s)\n", tableName, names);
	fprintf(out, "VALUES (%s);\n\n", string);
	
	while(fgets(string, 999, in) != NULL){
		string[strlen(string)-1]=0;
		fprintf(out, "INSERT INTO %s (%s)\n", tableName, names);
		fprintf(out, "VALUES (%s);\n\n", string);
	}	
			
}
