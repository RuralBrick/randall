#!/usr/bin/bash

a=$(./randall 5 | wc -c); if [ $a -eq 5 ]; then echo "success"; else echo "fail"; fi
