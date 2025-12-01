###############################################
# FICHIER: convergence.plot
# UTILISATION: Trace l'erreur L2 vs Delta x
###############################################

# --- Configuration Générale ---
set terminal pngcairo size 1000,800 font "Arial,12"
set datafile separator ","
set border back
set grid

# --- Configuration des Axes (Log-Log) ---
set logscale xy
set format x "10^{%T}"
set format y "10^{%T}"
set xlabel "Pas d'espace Delta x"
set ylabel "Erreur L2"
set title "Ordre de Convergence"

# --- Légende et Marges ---
set key bottom right box opaque
set offset 0.05, 0.05, 0.05, 0.05

# Sources 
SOURCE1 = "data/Erreur.csv"

# --- Régression Linéaire (Fit) ---
f(x) = a * x + b
fit f(x) SOURCE1 using (log($1)):(log($2)) via a,b

titre_fit = sprintf("Régression (Pente = %3.3f)", a)

# --- Sortie ---
set output "Ordre_Convergence.png"

# --- TRACE FINAL ---

plot SOURCE1 using 1:2 with points pointtype 7 pointsize 1.5 lc rgb "blue" title "Erreur mesurée", \
     exp(f(log(x))) title titre_fit lc rgb "red" lw 2