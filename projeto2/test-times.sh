./server > server-out.txt &
sh run-clients.sh > client-output.txt &

./listener > listener-out.txt &
sh run-talker.sh > talker-output.txt &
