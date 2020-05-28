#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/// ����������� ������������ ����
typedef struct b
{
    char* name;             ///������� ��������
    char* group;            ///����� ������
    unsigned a0: 1;         ///�����
    unsigned a1: 1;         ///�����
    unsigned a2: 1;         ///�����
    unsigned a3: 1;         ///�����
    char* result;           ///���������
} bits;

///�������, ������������ ����� ������������
int choose();

///�������, ������������� ����������
char* input_line();

///�������, ����������� ��������� � ����������
bits* str_fill();

/// ������� ������� ������ ��� ������������� ������� �����
void clear_str_array(char **str, int n);

/// ������� ���������� ������ �� ��������� �����������
char **simple_split(char *str, int length, char sep);

int main()
{
    bits **st1;             ///������, ���������� ����������� ������
    int flag,               ///����, ������������ ������� �����
        slen,               ///����� ���� ������
        i,                  ///�������� �����
        n,                  ///���������� �����
        limit,              ///����������� ���������� ������� �������
        count,              ///������� ���������� ������� �������
        dolg,               ///���������� ���������
        pass;               ///���������� �������
    char **s2=NULL;         ///������ �����
    char s1[128];           ///�����
    char sep;               ///������-�����������
    FILE *df;               ///��������� �� ����

    setlocale(LC_ALL,"RUS");
    n=0;
    flag=0;
    dolg = 0;
    pass = 0;
    df=fopen("data.csv","r");
    if(df!=NULL)
    {
        /// ������� ����� � �����
        while(fgets(s1,128,df)!= NULL) n++;
        rewind(df); // ������� ��
        st1=(bits**)malloc(n*sizeof(bits*));
        if(st1)
        {
            ///������ ������ �� ����� � ���������� ����� ��������
            sep=';';
            for(i=0,count=0;i<n;i++, count++)
            {
                st1[i]=(bits*)malloc(sizeof(bits));
                if (st1[i])
                {
                    fgets(s1,128,df);
                    slen=strlen(s1);
                    s1[slen-1]='\0';
                    slen=strlen(s1);
                    s2=simple_split(s1,slen,sep);
                    st1[i]->name=(char*)malloc((strlen(s2[0])+1)*sizeof(char));
                    st1[i]->group=(char*)malloc((strlen(s2[1])+1)*sizeof(char));
                    st1[i]->result=(char*)malloc((strlen(s2[6])+1)*sizeof(char));
                    if(s2&&st1[i]->name&&st1[i]->group&&st1[i]->result)
                    {
                        strcpy(st1[i]->name,s2[0]+'\0');
                        strcpy(st1[i]->group,s2[1]+'\0');
                        st1[i]->a0=atoi(s2[2]);
                        st1[i]->a1=atoi(s2[3]);
                        st1[i]->a2=atoi(s2[4]);
                        st1[i]->a3=atoi(s2[5]);
                        strcpy(st1[i]->result,s2[6]+'\0');
                        clear_str_array(s2,7);
                    }
                    else
                    {
                        i=n;
                        puts("Row data not available!");
                        system("pause");
                        system("cls");
                        flag=1;
                    }
                }
                else
                {
                    puts("Error of memory allocation");
                    system("pause");
                    system("cls");
                    i=n;
                    flag=1;
                }
            }
        }
        else
        {
            puts("Out of memory!");
            system("pause");
            system("cls");
        }
        fclose(df);
    }
    else
    {
        perror("Data error!");
        system("pause");
        system("cls");
        st1=(bits**)malloc(n*sizeof(bits*));
        if(st1)
        {
            i=-1;
            do
            {
                i++;
                puts("������� ������ � ����������:");
                st1[i]=str_fill();
                printf("������ ���������� ����? (1)");
                flag=choose();
            }while(flag==1);
        }
    }

    if (flag)
    {
        for(i=0;i<=count;i++)
        {
            free(st1[i]->group);
            free(st1[i]->name);
            free(st1[i]->result);
            free(st1[i]);
        }
        free(st1);
    }

    /// ��������� ������� ��������
    if(st1&&n)
    {
        puts("������ ���������:");
        printf("|%20s |%6s | %22s | %8s|\n",
        "NAME","GROUP","PASSED CREDITS", "RESULT");
         printf("+---------------------+-------+------------------------+---------+\n");
        for(i=0;i<n;i++)
        {
            printf("|%20s |%6s |%5d %5d %5d %5d | %7s |\n",
            st1[i]->name, st1[i]->group, st1[i]->a0, st1[i]->a1, st1[i]->a2, st1[i]->a3, st1[i]->result);
        }
    }
    else
        puts("No data found!");
    system("pause");
    system("cls");

    printf("������� ����������� ����� ������� ������� - ");
    scanf("%d", &limit);

    printf("|%20s |%6s | %22s | %8s|\n",
    "NAME","GROUP","PASSED CREDITS", "RESULT");
    printf("+---------------------+-------+------------------------+---------+\n");
    for(i=0;i<n;i++)
    {
        count=0;
        if(st1[i]->a0) count++;
        if(st1[i]->a1) count++;
        if(st1[i]->a2) count++;
        if(st1[i]->a3) count++;

        if(count < limit)
        {
            strcpy(st1[i]->result, "�������");
            dolg++;
        }
        else
        {
            strcpy(st1[i]->result, "����");
            pass++;
        }
        printf("|%20s |%6s |%5d %5d %5d %5d | %7s |\n",
        st1[i]->name, st1[i]->group, st1[i]->a0, st1[i]->a1, st1[i]->a2, st1[i]->a3, st1[i]->result);
    }
    printf("� ������ ������ %d ������� � %d ���������.", pass, dolg);
    return 0;
}

char **simple_split(char *str, int length, char sep)
{
    char **str_array=NULL;
    int i,j,k,m;
    int key,count;
    for(j=0,m=0;j<length;j++)
    {
        if(str[j]==sep) m++;
    }

    key=0;
    str_array=(char**)malloc((m+1)*sizeof(char*));
    if(str_array!=NULL)
    {
        for(i=0,count=0;i<=m;i++,count++)
        {
            str_array[i]=(char*)malloc(length*sizeof(char));
            if(str_array[i]!=NULL) key=1;
            else
            {
                key=0;
                i=m;
            }
        }
        if(key)
        {
            k=0;
            m=0;
            for(j=0;j<length;j++)
            {
                if(str[j]!=sep) str_array[m][j-k]=str[j];
                else
                {
                    str_array[m][j-k]='\0';
                    k=j+1;
                    m++;
                }
            }
            str_array[m][j-k]='\0';
        }
        else
        {
            clear_str_array(str_array,count);
        }
     }
     return str_array;
}

int choose()
{
    char* line;             ///����� ������, ��������� �������������
    int ans;                ///�������� ���� ������������

    line=(char*)malloc(15*sizeof(char));
    if (line!=NULL)
    {
        fflush(stdin);
        fgets(line, 15, stdin);
        ans=atoi(line);
    }
    else
    {
        puts("������ ��������� ������");
        ans=5;
    }

    free(line);

    return ans;
}

void clear_str_array(char **str, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        free(str[i]);
        str[i]=NULL;
    }
    free(str);
    str=NULL;
}

char* input_line()
{
    char* line_input;          ///����� ������� �������� �������� ������

    line_input=(char*)malloc(80*sizeof(char));
    if (line_input!=NULL)
    {
        fflush(stdin);
        fgets(line_input, 79, stdin);
        line_input[strlen(line_input)-1]='\0';
        line_input=(char*)realloc(line_input, (strlen(line_input)+1)*sizeof(char));
    }
    else
    {
        puts("������ ��������� ������");
        system("pause");
        system("cls");
    }
    return line_input;
}

bits* str_fill()
{
    bits *b;            ///����� ���������, � ������� ���������� ���������� � ���������

    b=(bits*)malloc(sizeof(bits));
    if (b)
    {
        printf("������� ������� �������� - ");
        b->name=input_line();

        printf("������� ����� ������ - ");
        b->group=input_line();

        printf("������� ��������� ����� ������� ������ - ");
        b->a0=choose();

        printf("������� ��������� ����� ������� ������ - ");
        b->a1=choose();

        printf("������� ��������� ����� �������� ������ - ");
        b->a2=choose();

        printf("������� ��������� ����� ���������� ������ - ");
        b->a3=choose();

        printf("� ���� ����� ����� ������� ��������� ����� �������. �����������, ������� �����-������ ������.");
        b->result=input_line();
    }
    else
    {
        puts("������ ��������� ������");
        system("pause");
        system("cls");
    }
    return b;
}
