#!/bin/bash
#@Author: Xincheng Han
#@Author: Yuanzhou Xue
#
#Template Declaration
#Adding code before the final 'fi'
#echo "6.xxxxxx"
#elif [ $number -eq 6 ];then  #Do not loss the blank!
##your command;               #Do not forget ';' at the end!
#

usingTarget='Develop'     #Modify it if the other target is used!

echo "******** FUNCTION LIST ***********"
echo "******** 1.MAKE        ***********"
echo "******** 2.COPYFILES   ***********"
echo "******** 22.COPYFILES  Wireless  *"
echo "******** 3.LOGIN       ***********"
echo "******** 33.LOGIN      Wireless  *"
echo "******** 4.SIMROBOT    ***********"
echo "******** 5.GameControl ***********"
echo "******** 6.TeamPosition***********"
echo "******** 7.Bush        ***********"
echo "******** 8.Ping        ***********"
echo "******** 88.Ping       Wireless  *"
echo "******** 9.rm con file ***********"


read number

if [ $number -eq 1 ];then
cd Make/Linux
make CONFIG=$usingTarget -j4
cd;

elif [ $number -eq 2 ];then
cd Make/Linux
echo "Input your IP:192.168.4."
read ip
./copyfiles $usingTarget 192.168.4.$ip -v 50 -b
cd

elif [ $number -eq 22 ];then
cd Make/Linux
echo "Input your IP:10.0.4."
read ip
./copyfiles $usingTarget 10.0.4.$ip -v 50 -b
cd

elif [ $number -eq 3 ];then
cd Make/Linux
echo "Input your IP:192.168.4."
read ip
./login 192.168.4.$ip 
cd

elif [ $number -eq 33 ];then
cd Make/Linux
echo "Input your IP:10.0.4."
read ip
./login 10.0.4.$ip 
cd

elif [ $number -eq 4 ];then
cd Build/Linux/SimRobot/$usingTarget
./SimRobot;

elif [ $number -eq 5 ];then
cd GameController
java -jar GameController.jar

elif [ $number -eq 6 ];then
cd GameController
java -jar TeamCommunicationMonitor.jar

elif [ $number -eq 7 ];then
cd Build/Linux/bush/$usingTarget
./bush;

elif [ $number -eq 8 ];then
echo "Input your IP:192.168.4."
read ip
ping -c 10 192.168.4.$ip

elif [ $number -eq 88 ];then
echo "Input your IP:10.0.4."
read ip
ping -c 10 10.0.4.$ip

elif [ $number -eq 9 ];then
rm Config/Scenes/Includes/connect.con

fi
