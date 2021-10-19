#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bios.h>
#include <dir.h>

char *getcd(void)
{
	char path[MAXPATH];
	strcpy(path, "X:\\");
	path[0] = 'a' + getdisk();
	getcurdir(0, path + 3);
	return path;
}

int main(int argc, char *argv[])
{
	printf("\nFloppyBootWriter V01.A by ali90taz\n\n");

	if(argc > 1)
	{
		if(!strcmp(strlwr(argv[1]), "read"))
		{
			char buffer[512];
			int status;
			FILE *tmp;
			status = biosdisk(2, 0, 0, 0, 1, 1, &buffer);
			status = biosdisk(2, 0, 0, 0, 1, 1, &buffer);
			tmp = fopen("BOOT.BIN", "wb");

			if(setvbuf(tmp, NULL, _IOLBF, 512) != 0)
			{
				printf("Failed to set up buffer for output file...\n");
				status = 0xFFF;
			}
			else
			{
				fwrite(&buffer, 512, 1, tmp);
				if(fclose(tmp) != 0)
				{
					status = 0xFFF;
				}
			}

			switch(status)
			{
			case 0:
				printf("Operation successful, the BOOT.BIN ");
				printf("file was saved in %s\n", strupr(getcd()));
			break;
			default: printf("Error (Ox%X)\n", status);
			}
		}
		else if(!strcmp(strlwr(argv[1]), "write"))
		{
			char buffer[512];
			int status;
			FILE *tmp;
			tmp = fopen("BOOT.BIN", "rb");

			if(setvbuf(tmp, buffer, _IOFBF, 512) != 0)
			{
				printf("Failed to set up buffer for input file...\n");
				status = 0xFFF;
			}
			else
			{
				fread(&buffer, 512, 1, tmp);
				status = biosdisk(3, 0, 0, 0, 1, 1, &buffer);
				status = biosdisk(3, 0, 0, 0, 1, 1, &buffer);

				if(fclose(tmp) != 0)
				{
					status = 0xFFF;
				}
			}

			switch(status)
			{
			case 0:
				printf("Operation successful, the BOOT.BIN ");
				printf("file has been written to the\n");
				printf("boot sector of the floppy.\n");
			break;
			default: printf("Error (Ox%X)\n", status);
			}
		}
		else printf("Unknown command, type fbw /? for help.\n");
	}
	else
	{
		printf("The program was called without arguments,\n");
		printf("type fbw /? for help.\n");
	}
	return 0;
}


