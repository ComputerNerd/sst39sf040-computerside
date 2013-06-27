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
	for (x=0;x<3;++x){
		do {
			RS232_PollComport(24,&tempC,1);
			if (tempC != Sig[x]){
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
		printf("Error programming byte letter code '%c'\n",datr);
}
void help(void){
	puts("Usage: flash filename [-d]");
	puts("-d is optional");
	puts("If you specify -d that means to dump flash chip instead of write and file will be for output instead of input");
}
int main(int argc,char ** argv)
{
	uint8_t dump=0;
	if (argc!=2&&argc!=3){
		help();
		return 1;
	}
	else if(argc==3){
		if(strcmp("-d",argv[2])==0)
			dump=1;
		else{
			printf("To specify dumping you need to use -d but you did %s instead\nThis program will show help and exit\n",argv[2]);
			help();
			return 1;
		}
	}
	if(RS232_OpenComport(24,500000)){
		puts("Com-port 24 could not be opened");
		return 1;
	}
	waitRDY();
	RS232_SendByte(24,'R');
	if(dump)
		RS232_SendByte(24,'R');
	else
		RS232_SendByte(24,'W');
	waitRDY();
	uint8_t id,manid;
	RS232_PollComport(24,&manid,1);
	printf("Manufacture ID: 0x%X\nDetcted as: ",manid);
	if(manid==0xBF)
			puts("SST/microchip");
	else
		puts("Unkown manufacturer");
	RS232_PollComport(24,&id,1);
	printf("Device ID: 0x%X\n",id);
	uint32_t capcity=524288;
	switch(id){
		case 0xB5:
			puts("SST39SF010A");
			capcity=131072;
		case 0xB6:
			puts("SST39SF020A");
			capcity=262144;
		break;
		case 0xB7:
			puts("SST39SF040");
			capcity=524288;
		break;
		default:
			puts("ERROR: cannot deterim chip capacity defaulting to 524288");
		break;
	}
	FILE* fp;
	uint8_t* dat;
	if(!dump){
		fp=fopen(argv[1],"rb");
		fseek(fp, 0L, SEEK_END);
		size_t size = ftell(fp);
		if (size > capcity){
			puts("Your file is too large");
			fclose(fp);
			return 1;
		}
		rewind(fp);
		dat=calloc(1,capcity);
		if (dat==0){
			puts("Error allocating memory");
			fclose(fp);
			return 1;
		}
		fread(dat,1,size,fp);
		fclose(fp);
		RS232_PollComport(24,&id,1);
		//putchar(id);//should be upercase 'D'
		if(id!='D'){
			puts("An error has occuring exiting...");
			free(dat);
			return 1;
		}
		putchar('\n');
		RS232_PollComport(24,&id,1);
		if (id == 'S')
			puts("Chip is erased");
		else{
			printf("Error erasing chip code %c\n",id);
			free(dat);
			return 1;
		}
	}else
		fp=fopen(argv[1],"wb");	
	//now program the chip
	putchar('\n');
	uint32_t x;
	for (x=0;x<capcity;++x){
		uint8_t data;
		if(dump){
			RS232_PollComport(24,&data,1);
			fputc(data,fp);
		}else{
			programByte(dat[x]);
			RS232_PollComport(24,&data,1);
			if (data!=dat[x])
				printf("Byte %d at address %d should be %d\n\n",data,x,dat[x]);
		}
		printf("Progress : %% %f\r",(float)x/(float)capcity*100.0);
	}
	if(dump)
		fclose(fp);
	free(dat);
	return 0;
}
