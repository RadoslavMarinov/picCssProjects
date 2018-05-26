move StartButton_main.c .\temp 
move start_button.c .\temp
move start_button.h .\temp
move start_button_conf.h .\temp

del start_button*
del StartButton_main*
del main*

cd temp
move StartButton_main.c ..\
move start_button.c ..\
move start_button.h ..\
move start_button_conf.h ..\