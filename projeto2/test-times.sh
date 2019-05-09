gcc server.c -o server
gcc client.c -o client
gcc listener.c -o listener
gcc talker.c -o talker

./server > server-output.txt &
sleep .5
sh run-clients.sh > client-output.txt &

sh run-talker.sh > talker-output.txt &
