set terminal png
set output "graph.png"

set xrange [1000:10000]

set yrange [0:8]

set grid

set ytics 1

set xtics 2500

set ylabel "Time, sec * 10^2"

set xlabel "N"

plot "./TestTable.txt" u 1:2 with linespoints lw 2 pt 2 ps 1 title "double", "./TestTable.txt" u 1:3 with linespoints lw 2 pt 2 ps 1 title "float" , "./TestTable.txt" u 1:4 with linespoints lw 2 pt 2 ps 1 title "int"