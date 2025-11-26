# En mode debug vous pouvez faire une sortie en png pour rapidement voir le résultat
set terminal pngcairo size 1000,800 

#Pour gérer les fichiers csv
set datafile separator ","

# Des qu'il vous convient convient faites une sortie en latex plutôt
# set terminal cairolatex pdf size 15cm,10cm

# Deux fichiers seront générés, un pdf et un tex. Il faut inclure les deux dans votre projet puis faire un
# \input{nom_du_fichier} pour inclure le graphique.

SOURCE1 = "data/P_wk_coupled.csv"



# paramètres de la légende
set border back
set key box lt -1 lw 2
set key box opaque
set key width 1 height 1
set key bottom right

# paramètres des axes
set grid back
#set format x "10^{%T}"
#set format y "10^{%T}"
#set logscale xy

# titres des axes (il est possible de faire du latex, les commandes doivent être utilisées avec deux \\)
set xlabel "t"
set ylabel "P(t)"
set autoscale y
set offset graph 0, 0, 0.1, 0.1

# régression linéaire
#f(x)=a*x+b
#fit f(x) SOURCE u (log($1)):(log($2)) via a,b
#titre=sprintf("Ordre de convergence : %3.3f",a)

# premier graphique dans le fichier image.png
set output "Pression_sain.png"
# pour la sortie en latex
# set output "ordre.tex"

plot SOURCE1 using 1:2 with points linecolor rgb "red"       linewidth 1 title "P en fonction du temps", \
     
     

    

     


      