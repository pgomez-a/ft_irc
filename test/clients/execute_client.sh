#!/bin/bash

for i in {1..1023}
do
	bash "client$i.sh" | nc -c localhost 6667&
	sleep 0.1
done
