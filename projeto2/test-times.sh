./server > server-out.txt &
sleep .5
sh run-clients.sh > client-output.txt &

sh run-talker.sh > talker-output.txt &
