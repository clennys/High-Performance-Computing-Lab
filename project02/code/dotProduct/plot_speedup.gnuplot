set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'speedup_plot.png'

set title 'Speedup vs. Number of Threads'
set xlabel 'Number of Threads'
set ylabel 'Speedup'
set logscale x 2
set grid

# Calculate speedup as serial time / parallel time for each method
plot for [size in "10^5 10^6 10^7 10^8 10^9"]      'dot_product_times.data' using 2:(column(3)/lookup(size, 'serial_execution_times.data')) title sprintf("Size=%s (Reduction)", size) with linespoints lw 2,      '' using 2:(column(4)/lookup(size, 'serial_execution_times.data')) title sprintf("Size=%s (Critical)", size) with linespoints lw 2

# Function to lookup serial time by array size from the serial execution times file
lookup(size, filename) = system(sprintf("grep '^%s' %s | awk '{print $3}'", size, filename))
