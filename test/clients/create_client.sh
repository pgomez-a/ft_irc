#!/bin/bash

for i in {1..127}
do
	echo 'echo "PASS pass"' > "client$i.sh"
	echo 'sleep 1' >> "client$i.sh"
	echo "echo \"NICK c$i\"" >> "client$i.sh"
	echo 'sleep 1' >> "client$i.sh"
	echo 'echo "USER u 0 0 0 0"' >> "client$i.sh"
	echo 'sleep 20' >> "client$i.sh"
done
