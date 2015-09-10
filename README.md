#Conway's Game of Life
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life


---------------------
Author: Dan Wilder
Date: Dec 14 2013
---------------------

** Interactive initialization was never fully implemented. 

~ Below explains the config file.
    Don't change name. Keep filename "config".

------------------------------------------------------------------------

General Description:
  
  The config file is a series of lines with text and lines without.
  Blank lines are (as usual) meaningless and you can put as many 
  as you want in between the lines with text.


  As for lines with text they all follow the format:

       description_string:     int 


  One or more spaces serve as a delimiter between the two components.
  

  The decription string must be a single string (the colon 
    is considered part of that string). It serves as reference to what
    the <int value> will affect.



  Line order is important!!! While you can put blank lines freely, 
  do not alter the order that the content lines appear in. 

-----------------------------------------------------------------------



*** NOTE: The number in [brackets] refer to both the order in which 
          the variables are to appear, and their index in the 
          CONFIG_SETTINGS array located in the source.



 
=============================
Description of each element:
=============================



[0] Show_Prep_Screen:
  ~ Valid Values: 0 or 1
    
    1: Upon program's launch, a screen will pop up with various
       introduction remarks.

    0: No such screen appears.




[1] Auto_Timer:
  ~ Valid Values: 0 or 1

    1. Each generation evolves automatically.
    
    0. Each evolution occurs only when the user hits <enter>




[2] Microsecond_Delay:
  ~ Valid Values: int > 0 
   
  ~ Description:
    This is the number of microseconds to wait before going
    to next generation.

    NOTE: This value does does nothing if Auto_Timer is set to 0.




[3] Number_Rows:
  ~ Valid values: int > 0

  ~ Description: 
    This is the number of rows in the universe. 

    NOTE: Keep in mind the size limitations of YOUR screen's display.
         


 
[4] Number_Cols:  
  ~ Valid values: int > 0

  ~ Description: 
    This is the number of columns in the universe. 

    NOTE: Keep in mind the size limitations of YOUR screen's display.



[5] Enable_Wrap:
  ~ Valid values: 0 or 1

  1. Univere Wrap is enabled. The left most column borders the right 
     most column and vice versa. In addition, the top and bottom
     rows bordering each other.

  0. Universe Wrap is disabled. 




[6] ASCII_INT:
  ~ Valid values: int values representing ASCII characters
  
  ~ Description:
      The int value corresponds to an ASCII character. This character
      will be the character that represents an alive cell.
 
  ~ I plan to add a text file for easy reference to ASCII characters.
      Example:

        '@'  ==> 64
   
        '#'  ==> 35



[7] PREPEND_init_gen:
  ~ Valid values: 0 or 1

    1: Prepend "init_gen/" before all given filenames

    0: Do not prepend "init_gen/" before all given filenames
