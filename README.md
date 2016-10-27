#FSND Mathew Kennedy <kennedyl@uwec.edu> & Jeremy Winterberg <winterjd@uwec.edu>

How to compile
    - run the 'make' command

How to run Server side
    use -l flag to listen for incoming connections
        ie. "./fsnd -l <FILENAME>

How to run client side
    use command "./fsnd" appending any necessary flags
   

HOW TO RUN WITH ENCRYPTION

from thing0:
    - start kdc ("./fsnd -k")

from thing2:
    - start server ("./fsnd -l -v file.out")

from thing1:
    - start client ("./fsnd file_to_send.txt")
