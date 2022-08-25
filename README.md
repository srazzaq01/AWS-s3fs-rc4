# AWS-s3fs-rc4
To compile this program, run the following command
Gcc rc4.c -o *name* -lcrypto
You have to add -lcrypto because the included libraries need that in order to call the functions. Once the executable is made, then you can run it with the following commands ./executableName -e/d -salt/nosalt input output secretKey
If the execution worked, then you can run diff on the files and see if they are the same or not. Finally the program is also backwards compatible with openSSL.
