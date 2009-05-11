#!/bin/bash

nFiles=$((`ls "gen_populations/" | grep -e "\.dat$" | wc -l` / 2))

for i in `seq 0 $(($nFiles - 1))`; do 
  # TODO: Warnings.
  rm output/gen_selected_points.dat
  rm output/gen_not_selected_points.dat
  
  ln -s gen_populations/gen_selected_points_`printf "%03d" $i`.dat output/gen_selected_points.dat
  ln -s gen_populations/gen_not_selected_points_`printf "%03d" $i`.dat output/gen_not_selected_points.dat
  
  gnuplot gen_draw.gp
  
  sleep 1
done

rm output/gen_selected_points.dat
rm output/gen_not_selected_points.dat
