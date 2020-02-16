/*
OS development - COMP 3500
Project 1 - Pseudo drivers
02-17-2020

Alex Jones
Yeonseo Ju
Ryan McGill
Kegan Vanginkel
Yadi Zhong
*/
#define _CRT_SECURE_NO_WARNINGS

#include "fs.h"
#include <assert.h>
#include <stdlib.h>
#define NUM_SECTORS 20
#define SECTOR_SIZE 65536
#define FNAME "bin"

FILE *pt;
int i,j;
char cur[SECTOR_SIZE]; 
void checkFile(){ //check file for existence, does not check for appropriate size
    pt = fopen(FNAME,"r+");
    if(pt != NULL){
        fclose(pt);
        return;
    }
    else {
        pt = fopen(FNAME,"wb");
        for(j = 0; j < SECTOR_SIZE*NUM_SECTORS; j++){
            fputc(0xFF,pt);
        }
        fclose(pt);
    }
    fclose(pt);
    return;
}

void EraseSector(char n) {
    if(n >= NUM_SECTORS)//validate address
        return;     
    checkFile();                        //check file for existence, if not, create and fill with FF
    pt = fopen(FNAME,"rb+");             //open the file
    fseek(pt,SECTOR_SIZE*n,SEEK_SET);   //seek to sector start
    for (j = 0; j < SECTOR_SIZE; j++){  //iterate over all bytes
        fputc(0xFF,pt);                 //erase byte
    }
    fclose(pt);                         //close file
}
void EraseAllSectors() {                //no need to checkfile as every single sector will check
    for(i = 0; i < NUM_SECTORS; i++){   //iterate over all sectors
        EraseSector(i);                 //erase that sector
    }
}
unsigned short ReadWord(unsigned long long nAddress){
    unsigned short ret;                          //return value
    if ((nAddress & 0x01) != 0 || nAddress >= SECTOR_SIZE*NUM_SECTORS)
        return;
    checkFile();                        //check file for existence, if not, create and fill with FF
    pt = fopen(FNAME,"rb+");             //open file
    fseek(pt,nAddress,SEEK_SET);        //seek to address
    ret = (fgetc(pt) << 8) | (fgetc(pt));
    fclose(pt);
    return ret;

}
void WriteWord(unsigned long long nAddress, unsigned short d){
    unsigned short ret;
    if ((nAddress & 0x01) != 0 || nAddress >= SECTOR_SIZE*NUM_SECTORS)
        return;
    checkFile();                    //check file for existence, if not, create and fill with FF
    pt = fopen(FNAME,"rb+");         //open file
    fseek(pt,nAddress,SEEK_SET);    //seek to address
    ret = (fgetc(pt) << 8) | (fgetc(pt));
    fseek(pt,nAddress,SEEK_SET);    //go back to so that we can write  
    ret &= d;                       //preform necessary bit garbage
    fputc(ret >> 8,pt);                  //put first byte
    fputc(ret,pt);             //put second byte
    fclose(pt);                     //close
}

int main( int argc, const char* argv[] ) // some test script, wont matter when linked as library
{
    checkFile();
    EraseAllSectors();
    for(int k = 0; k < SECTOR_SIZE*NUM_SECTORS; k+=2){
	    WriteWord(k,k);
    }
    for(int k = 0; k < SECTOR_SIZE*NUM_SECTORS; k+=2){
	    assert(((unsigned short)k) == ReadWord(k));
    }
}
