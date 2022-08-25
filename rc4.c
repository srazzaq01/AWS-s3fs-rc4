#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/rc4.h>
#include <openssl/rand.h>
 
//global variables
# define keyNum 16
# define saltNum 8
# define bufSize 4096
# define saltLen 16
 
 
int main(int argc, char* argv[]){
   //create variables for the files, the key, and the buffers
   int iFile;
   int oFile;
   RC4_KEY key;
   unsigned char saltedKey[keyNum];
   char inputBuf[bufSize];
   char outputBuf[bufSize];
 
   //open the input file
   iFile = open(argv[3], O_RDONLY);
   //create output file with the correct flags
   oFile = open(argv[4], O_CREAT | O_WRONLY, 0644);
 
  
 
 
   //if salt option chosen
   if (strcmp(argv[2], "-salt") == 0){
     unsigned char salt[saltNum];
  
 
   //if encryption option chosen
   if (strcmp(argv[1], "-e") == 0){
     char saltBuf[saltLen];
     memset(saltBuf, 0, saltLen); //set saltString to all zeros before reading into it
     RAND_bytes(salt, saltNum); //randomly generate salt bytes
     sprintf(saltBuf, "Salted__%c%c%c%c%c%c%c", //string to store salt in ciphertext
     salt[0], salt[1], salt[2], salt[3],
     salt[4], salt[5], salt[6]);
     saltBuf[15] = salt[7]; //simple solution to fix salt[7] not being printed in properly
     write(oFile, &saltBuf, saltLen); //write saltString to output file
   }
 
   //if decryption option chosen
   else if (strcmp(argv[1], "-d") == 0){
     lseek(iFile, 8, SEEK_SET); //read/write file offset to read saltString
     read(iFile, salt, saltNum); //read salt chars into salt
   }
 
   //create key with salt
   if (! EVP_BytesToKey(EVP_rc4(), EVP_sha256(),
   salt, (unsigned char *)argv[5], strlen(argv[5]), 1, saltedKey, NULL) )
   {
     printf("Error: Could not create encryption key.\n"); //if BytesToKey fails.
     exit(-1);
   }
   RC4_set_key(&key, keyNum, saltedKey); //using saltedKey hash to create RC4_KEY key.
 }
 
 
   else{
   //create key without salt
     if (! EVP_BytesToKey(EVP_rc4(), EVP_sha256(),
       NULL, (unsigned char *)argv[5], (int)strlen(argv[5]), 1, saltedKey, NULL) )
   {
        printf("Error: Could not create encryption key.\n"); //if BytesToKey fails.
        exit(-1);
   }
      RC4_set_key(&key, keyNum, saltedKey); //using saltedKey hash to create RC4_KEY key.
 }
 
 
 
 
 
 
 
 
 
 
 
 
   ssize_t bytes = 0; //counting bytes so program knows what buffer is if buffer < READ_WRITE_SIZE
   while (bytes = read(iFile, &inputBuf, bufSize)){
     RC4(&key, bytes, inputBuf, outputBuf); //encrypt inputBuffer to outputBuffer
     write(oFile, &outputBuf, bytes);
 }
//close files
close (iFile);
close (oFile);
 
 
 
}
 


