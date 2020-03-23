# Very-Simple-SMTPS
A small library written in c++ that uses libcurl to be able to send emails with binary attachments.

## Installing:
To be able to use this code, you will need to install libcurl and also get the curl header files.

## Compiling:
To compile this on linux do: 
`g++ -o <progam name> <whatever .h .cpp files> email.cpp -lcurl`

## Notes:
Enable either WINDOWS_H or LINUX_H in the header file before compiling to ensure you are using the correct code for your operating system.
