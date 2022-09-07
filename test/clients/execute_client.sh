#!/bin/bash

for i in {1..127}
do
	bash "client$i.sh" | nc -c localhost 6667&
	sleep 0.01
done
