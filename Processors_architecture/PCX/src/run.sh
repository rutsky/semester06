#!/bin/sh

./pcx_decode ../data/baboon.pcx     ../data/baboon.ppm     100 10 > baboon.log
./pcx_decode ../data/baboon4.pcx    ../data/baboon4.ppm    100 10 > baboon4.log
./pcx_decode ../data/square_2x2.pcx ../data/square_2x2.ppm 100 10 > square_2x2.log
