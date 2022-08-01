# Project Log

## init commit
Tried to start out the inital project with multiple structs as the data structure to be used. However I prefer just having one
struct with others contained in it within a union. So on to my second commit I removed everything and redesigned the structs and 
removed most of the functions built around the other idea and will try building up the project from there.

## Second log entry 
Technically the third real main log entry but anyways this is the first real version of the second iteration. Managed to get my
program to be able to actually read a single number and determine if its a number then make it into an actual lisp object or to make it an error.

## third log entry
After this I added an eval function which essentially does nothing and refactored the code by removing all uneeded comments 
and the commented out read section.

## fourth log entry
After having a good base on the project I set out to make sure I was getting the proper output on my numbers since I could only 
print out only one number at a time. I also had to fix an infinite loop by making sure after ungetting something to getchar again.

Also fixed the issue where ERROR would display after ever input including numbers. WHich was actually  a \n character.

But a more important bug was that my program was actually going through each indiivual character and printing it out so:

Expected:
INPUT-> 101
OUTPUT-> 101

ACTUAL:
INPUT-> 101
OUTPUT->1
      ->0
      ->1

I initally thought it was just a \n in my printf but it was actually a whole issue with my read routine.
So i copied some code to try and see if I could figure out what was causing the issue.

So basically I delved into the read function to see how it works:

/*	CODE START 	*/

eat_whitespace();				

c = getchar();    

if (isdigit(c)) {
    /* read a fixnum */
    if (c == '-') {
        sign = -1;
    }
    else {
    	ungetc(c, stdin);
    }
   
------------------------------------------- BLOCK A
    while (isdigit(c = getchar())) {
        num = (num * 10) + (c - '0');
        num = (num * 10)  + (c - '0');
    }
   
------------------------------------------  BLOCK B
        if(isdigit(c = getchar())){
        printf("C IS %d", c);
        num = (num * 10) + (c - '0');
    }
------------------------------------------
    num *= sign;
    if (is_delimiter(c)) {
       ungetc(c, stdin);
        return makeNum(num);              
    }


/*	CODE END 	*/

A couple of things to note block a and b should be run seperatly not at the same time.

eat whitespace is at the beginning. I'm assuming so it eats whitespace BEFORE reading from stdin.

then once it has read from stdin it puts it back on stream and evaluates stdin.
In block a It actually runs through the ENTIRE stdin input hence the loop

In block b It only grabs the first not sure what happens to the rest. prolly remains on the stream which should be removed.
This can also be accomplished with a peek routine as well.

Then at the end it checks whetere theree is a space , EOF, ) etc and returns the number.



---
All in all i will attempt to rewrite read() this time and actually produce the correct output.




