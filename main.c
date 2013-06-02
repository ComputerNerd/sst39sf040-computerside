//To compile do: gcc main.c rs232.c -Wall -O2 -o flash
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"
void waitRDY(void)
{
	static const char Sig[]="RDY";
	uint8_t tempC,x;
	uint32_t junkC=0;
	puts("Waiting for RDY command");
	for (x=0;x<3;x++)
	{
		do {
			RS232_PollComport(24,&tempC,1);
			if (tempC != Sig[x])
			{
				junkC++;
				printf("Junk Char %d or %c while waiting for %c so far skiped %d\n",tempC,tempC,Sig[x],junkC);
			}
			putchar(tempC);
		} while (tempC != Sig[x]);
	}
	if (junkC != 0)
		printf("%d junk bytes skipped\n",junkC);
	puts("Ready recived");
}
void programByte(uint8_t dat)
{
	uint8_t datr;
again:
	RS232_SendByte(24,dat);
	RS232_PollComport(24,&datr,1);
	if (datr!='N')
		printf("Error programming byte code %c\n",datr);

}
int main(int argc,char ** argv)
{
	if (argc!=2)
	{
		puts("Usage: flash filename");
	}
	FILE * infile=fopen(argv[1],"rb");
	fseek(infile, 0L, SEEK_END);
	size_t size = ftell(infile);
	if (size > 524288)
	{
		puts("Your file is too large");
		fclose(infile);
		return 1;
	}
	rewind(infile);
	uint8_t * dat=calloc(1,524288);
	if (dat==0)
	{
		fclose(infile);
		return 1;
	}
	fread(dat,1,size,infile);
	fclose(infile);
	if(RS232_OpenComport(24,1000000))
	{
		puts("Com-port 24 could not be opened");
		free(dat);
		return 1;
	}
	waitRDY();
	RS232_SendByte(24,'R');
	RS232_SendByte(24,'D');
	waitRDY();
	uint8_t id;
	RS232_PollComport(24,&id,1);
	printf("Manufacture ID: 0x%X\n",id);
	RS232_PollComport(24,&id,1);
	printf("Device ID: 0x%X\n",id);
	RS232_PollComport(24,&id,1);
	putchar(id);//should be upercase 'D'
	putchar('\n');
	RS232_PollComport(24,&id,1);
	if (id == 'S')
		puts("Chip is erased");
	else
		printf("Error erasing chip code %c\n",id);
	//now program the chip
	putchar('\n');
	uint32_t x;
	for (x=0;x<524288;x++)
	{
		programByte(dat[x]);
		printf("Progress : %% %f\r",(float)x/524288.0*100.0);
	}
	for (x=0;x<524288;x++)
	{
		uint8_t data;
		RS232_PollComport(24,&data,1);
		if (data!=dat[x])
			printf("Byte %d at address %d should be %d\n\n",data,x,dat[x]);
		printf("Progress : %% %f\r",(float)x/524288.0*100.0);
	}
	free(dat);
	return 0;
}
