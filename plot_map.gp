set terminal gif animate delay 5 size 800,600
set output "P_wave.gif"

set xlabel "x (m)"
set ylabel "Pression P(x,t)"
set grid
set yrange [0:*]

# nombre d'instants à afficher
NT = 50

do for [i=0:NT] {

    # temps correspondant à la frame
    t0 = i * 0.05   # à ajuster selon ton dt

    plot "data/P_xt.csv" using 1:($2==t0 ? $3 : 1/0) with lines lw 2 notitle
}