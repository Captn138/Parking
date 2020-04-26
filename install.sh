#!/bin/bash
sudo apt-get install make gcc ttf-ancient-fonts
printf "\n\nTo run the program, execute 'make'.\nWould you like to run the program? "
read -r -p "(yes/no)" response
if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]
 then 
 make
fi