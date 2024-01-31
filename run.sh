#!/bin/bash

GIVENARGS=$1
for var in "$@"
do
	if [ -e $var ]
	then
		ARGS+=" ./$var "
	else
		ARGS+=" $var "	
	fi
done

./schedule $ARGS
