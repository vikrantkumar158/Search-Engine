#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#define BASE_URL "http://www.chitkara.edu.in"
void validate(char *str2)
{
	printf("%s",str2);
	int i,j=0;
	for(i=0;str2[i]&&j<1;++i)
	{
		if(BASE_URL[i+7]!=str2[i]&&BASE_URL[i+7])
			break;
		if(str2[i]=='/')
			++j;
	}
	if(j!=1&&str2[i]!='\0')
	{
		printf("Not same as BASE URL!!!\n");
		exit(0);
	}
}
void checkPath(char *str1)
{
	struct stat sb;
	if(stat(str1,&sb)==0 && S_ISDIR(sb.st_mode))
	{
		if((sb.st_mode & S_IWUSR)!=S_IWUSR)
		{
			printf("Invalid directory!!! It isn't writeable!! Exiting now...\n");
			exit(0);
		}
	}
	else
	{
		printf("No such path exists!!! Exiting now...\n");
		exit(0);
	}
}
void retreive(char str1[],char str2[])
{
	static int counter=0;
	++counter;
	char start[]="wget --convert-links -O index.txt -P ";
	char geturl[200],fname[20];
	sprintf(fname,"file%d.txt",counter);
	strcpy(geturl,start);
	strcat(geturl,str1);
	strcat(geturl," ");
	strcat(geturl,str2);
	char links[500]="cat index.txt | grep -Eo \"(http|https)://[a-zA-Z0-9./?=_-]*\" | grep -Ev '(*.jpg|*.css|*.js|*.png|*.php|*.gif|*.xml|*.ico|*noscript*|*.pdf)' | sort | uniq > ";
	//char links[200]="cat index.txt | grep -Eo \"(http|https)://[a-zA-Z0-9./?=_-]*\" | sort | uniq > ";
	strcat(links,fname);
	system(geturl);
	system(links);
	system("rm index.txt");
}
int main(int argc,char *argv[])
{
	int i,j=1,depth,counter=1,x;
	for(i=0;argv[1][i];++i);
	char *str1=(char *)malloc((i+1)*sizeof(char));
	strcpy(str1,argv[1]);
	checkPath(str1);
	sscanf(argv[2],"%d",&depth);
	for(i=0;argv[3][i];++i);
	char *str2=(char *)malloc((i+1)*sizeof(char));
	strcpy(str2,argv[3]);
	validate(str2);
	retreive(str1,str2);
	char str[1000];
	for(i=2;i<=depth;++i)
	{
		x=counter;
		for(;j<=x;++j)
		{
			char str[1000],fname[20];
			sprintf(fname,"file%d.txt",j);
			FILE *fp;
			fp=fopen(fname,"r");
			if(fp==NULL)
				printf("Failed to open");
			else
			{
				while(fgets(str,1000,fp)!=NULL)
				{
					char chk[1000];
					strcpy(chk,"wget -q ");
					strcat(chk,str);
					if(!system(chk))
					{
						++counter;
						retreive(str1,str);
					}
				}
				fclose(fp);
			}
		}
	}
	return 0;
}

