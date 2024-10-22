# strong_scaling_plot.gp
set term pngcairo dashed size 1600,600
set title "Strong Scaling: Speedup vs Number of Threads"
set xlabel "Number of Threads"
set ylabel "Speedup"
set grid
set output "speedup.png"

# Define some line and point styles for clarity
set style line 1 lc rgb '#0060ad' lw 2 pt 7
set style line 2 lc rgb '#00FFFF' lw 2 pt 7
set style line 3 lc rgb '#228b22' lw 2 pt 7
set style line 4 lc rgb '#FFA500' lw 2 pt 7
set style line 5 lc rgb '#800080' lw 2 pt 7
set style line 6 lc rgb '#FF0000' lw 2 dt 2

set key top left  # Place the key (legend) outside of the plot

# Plot for multiple array sizes
# Extract times for each array size and plot the speedup dynamically
T1_5 = 0.01306
T1_6 = 0.117111  
T1_7 = 1.3465 
T1_8 = 13.4116
T1_9 = 133.943 

set multiplot layout 1, 2 title "Speedup Comparison: Reduction vs. Critical Method"

# Plot speedup for different array sizes
set title "Speedup (Reduction Method)"
plot "dot_product_times.data" using 2:(T1_5/$3) every ::0::5 title "10^5" with linespoints ls 1, \
     "dot_product_times.data" using 2:(T1_6/$3) every ::6::11 title "10^6" with linespoints ls 2, \
     "dot_product_times.data" using 2:(T1_7/$3) every ::12::17 title "10^7" with linespoints ls 3, \
     "dot_product_times.data" using 2:(T1_8/$3) every ::18::23 title "10^8" with linespoints ls 4, \
     "dot_product_times.data" using 2:(T1_9/$3) every ::24::29 title "10^9" with linespoints ls 5, \
	  x title "Ideal Speedup" with lines ls 6

set title "Speedup (Critical Method)"
plot "dot_product_times.data" using 2:(T1_5/$4) every ::0::5 title "10^5" with linespoints ls 1, \
     "dot_product_times.data" using 2:(T1_6/$4) every ::6::11 title "10^6" with linespoints ls 2, \
     "dot_product_times.data" using 2:(T1_7/$4) every ::12::17 title "10^7" with linespoints ls 3, \
     "dot_product_times.data" using 2:(T1_8/$4) every ::18::23 title "10^8" with linespoints ls 4, \
     "dot_product_times.data" using 2:(T1_9/$4) every ::24::29 title "10^9" with linespoints ls 5, \
	  x title "Ideal Speedup" with lines ls 6
unset multiplot
