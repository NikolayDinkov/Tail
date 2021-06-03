//------------------------------------------------------------------------
// NAME: Nikolay Dinkov
// CLASS: XIv
// NUMBER: 16
// PROBLEM: tail
// FILE NAME: tail.c (unix file name)
// FILE PURPOSE:
// Имплементация на функцията tail
//------------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//------------------------------------------------------------------------
// FUNCTION: read_write_stdin (име на функцията)
// Функцията служи за четене от стандартния вход и принтиране
// на последни 10 реда от прочетеното
// PARAMETERS:
// Няма параметри
//------------------------------------------------------------------------
void read_write_stdin()
{
	char *buf = calloc(1024, 1024); // заделям динамично 1024 байта с нули за съдъражанието на това, което съм въвел от станандартния вход, което се записва в променливата
	size_t buf_size = 1024; // променливата е числото, което отговаря на бройката на всички байтове
	int buf_space_on = 0; // променливата е числото, което отговаря на бройката на всички записани байтове

	int read_res;
	while((read_res = read(STDIN_FILENO, buf + buf_space_on, 1024)) != 0) // цикъл, с който изчитам целия вход независимо колко е голям
	{
		buf_space_on += read_res;
		if(buf_space_on + 1 > buf_size)
		{
			buf_size += 1024;
			buf = realloc(buf, buf_size);
		}
	}
	char *p_buff = buf; // копие на буфера, с което да се обхожда
	int max_lines = 0; // променлива, която съдържа числото отговарящо за брая на всички редове
	for(int i = 0; p_buff[i] != '\0'; i++) // цикъл за намирането на бройката на всички редове
	{
		if(p_buff[i] == '\n')
		{
			max_lines++;
		}
	}

	int write_res;
	char *p_buf = buf; // 2ро копие за обхождането на буфера
	for(int line = 0; line < max_lines; line++) // цикъл, който служи за принтирането на последните 10 реда от записаното в буфера
	{
		char *new_line; // указател, който сочи към елемента след новия ред и съдържа адреса му
		if(buf[buf_space_on - 1] != '\n' && line == max_lines - 1)
		{
			new_line = strchr(p_buf, '\0');
		}else{
			new_line = strchr(p_buf, '\n') + 1;
		}
		if(buf[buf_space_on - 1] != '\n')
		{
			if(line > max_lines - 10)
			{
				int bytes = new_line - p_buf;
				while(bytes != 0)
				{
					while((write_res = write(STDOUT_FILENO, p_buf, bytes)) != 0 && bytes != 0)
					{	
						if(write_res == -1)
						{
							perror("tail: error writing 'standard output'");
							free(buf);
							return;
						}
						p_buf += write_res;
						bytes -= write_res;
					}
				}
			}else{
				p_buf += (new_line - p_buf);
			}	
		}else{
			if(line >= max_lines - 10)
			{
				int bytes = new_line - p_buf;
				while(bytes != 0)
				{
					while((write_res = write(STDOUT_FILENO, p_buf, bytes)) != 0 && bytes != 0)
					{	
						if(write_res == -1)
						{
							perror("tail: error writing 'standard output'");
							free(buf);
							return;
						}
						p_buf += write_res;
						bytes -= write_res;
					}
				}
			}else{
				p_buf += (new_line - p_buf);
			}	
		}
	}
	free(buf);
}

//------------------------------------------------------------------------
// FUNCTION: read_write_files (име на функцията)
// Функцията служи за прочитане на цялото информация от един файл
// и след това принтиране на последните 10 реда от прочетеното
// PARAMETERS:
// int fd - Файлов дескриптор, служи за да знаем от кой файл да четем.
// char *file_name - Името на файла, който сме отворили,
// служи, за да можем да принтирам изисканите грешки.
//------------------------------------------------------------------------
void read_write_files(int fd, char *file_name)
{
	char *buf = calloc(1024, 1024); // заделям динамично 1024 байта с нули за съдъражанието на файла, което се записва в променливата
	size_t buf_size = 1024; // променливата е числото, което отговаря на бройката на всички байтове
	int buf_space_on = 0; // променливата е числото, което отговаря на бройката на всички записани байтове

	int read_res;
	while((read_res = read(fd, buf + buf_space_on, 1024)) != 0) // цикъл, с който изчитам целия файл независимо колко е голям
	{
		if(read_res == -1)
		{
			char error[512];
			strcpy(error, "tail: error reading '");
			strcat(error, file_name);
			strcat(error, "'");
			perror(error);
			break;
		}
		buf_space_on += read_res;
		if(buf_space_on + 1 > buf_size)
		{
			buf_size += 1024;
			buf = realloc(buf, buf_size);
		}
	}

	char *p_buff = buf; // копие на буфера, с което да се обхожда
	int max_lines = 0; // променлива, която съдържа числото отговарящо за брая на всички редове
	for(int i = 0; p_buff[i] != '\0'; i++) // цикъл за намирането на бройката на всички редове
	{
		if(p_buff[i] == '\n')
		{
			max_lines++;
		}
	}

	int write_res;
	char *p_buf = buf; // 2ро копие за обхождането на буфера
	for(int line = 0; line < max_lines; line++) // цикъл, който служи за принтирането на последните 10 реда от записаното в буфера
	{
		char *new_line = strchr(p_buf, '\n') + 1; // указател, който сочи към елемента след новия ред и съдържа адреса му
		if(line >= max_lines - 10)
		{		
			int bytes = new_line - p_buf;
			while(bytes != 0)
			{
				while((write_res = write(STDOUT_FILENO, p_buf, bytes)) != 0 && bytes != 0)
				{	
					if(write_res == -1)
					{
							perror("tail: error writing 'standard output'");
							free(buf);
							return;
					}
					p_buf += write_res;
					bytes -= write_res;
				}
			}
		}else{
			p_buf += (new_line - p_buf);
		}
	}
	free(buf);
}

//------------------------------------------------------------------------
// FUNCTION: partial_write_headers (име на функцията)
// Функцията служи за да принтира имената на файловете като спазва изискванията от улсовията
// PARAMETERS:
// char name* - Името на файла, което искаме да принтираме
// int bytes - големината на масива, в който е записано името
//------------------------------------------------------------------------
void partial_write_headers(char *name, int bytes)
{
	int write_res;
	while(bytes != 0)
	{
		while((write_res = write(STDOUT_FILENO, name, bytes)) != 0 && bytes != 0)
		{
			if(write_res == -1)
			{
					perror("tail: error writing 'standard output'");
					return;
			}
			name += write_res;
			bytes -= write_res;
		}
	}
}

int main(int argc, char *argv[])
{
	int first_newLine = 0; // служи за принтирането на нов ред ако е по-голямо от нула
	if(argc == 1) // проверка ако няма аргументи при пускането на програмата
	{
		read_write_stdin();
	}
	for(int i = 1; i<argc; i++) // цикъл, за да можем да обходим всички подадени аргументи, i започва от 1, защото винаги нулевия елемент е името на файла
	{
		if(*argv[i] == '-')
		{
			if(first_newLine > 0)
			{
				partial_write_headers("\n", 1);
			}
			if(argc > 2)
			{
				char name[512] = "==> standart input <==\n";
				partial_write_headers(name, strlen(name));
			}
			read_write_stdin();
			first_newLine++;
			continue;
		}
		int fd = open(argv[i], O_RDONLY);
		if(fd == -1)
		{
			char error[512];
			strcpy(error, "tail: cannot open '");
			strcat(error, argv[i]);
			strcat(error, "' for reading");
			perror(error);
			continue;
		}
		if(first_newLine > 0)
		{
			partial_write_headers("\n", 1);
		}
		if(argc > 2)
		{
			char name[512];
			strcpy(name, "==> ");
			strcat(name, argv[i]);
			strcat(name, " <==\n");
			partial_write_headers(name, strlen(name));
		}
		read_write_files(fd, argv[i]);
		first_newLine++;
		int err = close(fd);
		if(err == -1)
		{
			char error[512];
			strcpy(error, "tail: error reading '");
			strcat(error, argv[i]);
			strcat(error, "'");
			perror(error);
			continue;
			
		}
	}
}
