#!/bin/bash
( cd ..; make fclean && make test  && clear)
../ircserv $1 $2  &
sleep 1

for i in {1..3}
do
	bash "./err_test/err$i.sh" $1 $2
done

kill $(ps | grep ../ircserv | grep -v grep | cut -d" " -f2)
echo "All done..."
