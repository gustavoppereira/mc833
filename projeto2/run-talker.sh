#!/bin/sh

#  run-talkers.sh
#
#
#  Created by julio on 4/10/19.
#

./listener > listener-output.txt &
sleep .5
./talker localhost teste1@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste2@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste3@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste1@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste4@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste5@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste6@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste7@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste8@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste9@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste10@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste11@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste1@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste2@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste3@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste4@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste5@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste7@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste8@email.com
sleep .5
./listener >> listener-output.txt &
sleep .5
./talker localhost teste9@email.com
