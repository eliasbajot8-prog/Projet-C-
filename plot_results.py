import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# ==========================================
# CONFIGURATION
# ==========================================
FILE_DATA = "data/Erreur.csv"
OUTPUT_IMG = "convergence_loglog.png"

def main():
    # 1. Vérification du fichier
    if not os.path.exists(FILE_DATA):
        print(f"[ERREUR] Le fichier {FILE_DATA} est introuvable.")
        print("Assurez-vous d'avoir exécuté ./test_convergence avant.")
        return

    print(f"--- Lecture de {FILE_DATA} ---")
    
    # 2. Chargement des données
    # On suppose que le CSV a deux colonnes : dx, ErreurL2
    # header=None car le C++ ne met pas forcément d'en-tête, on nomme les colonnes manuellement
    try:
        df = pd.read_csv(FILE_DATA, header=None, names=["dx", "error"], sep=",")
    except Exception as e:
        print(f"[ERREUR] Lecture CSV impossible : {e}")
        return

    # Tri des données par dx (pour que la ligne soit tracée proprement)
    df = df.sort_values(by="dx")

    x = df["dx"].values
    y = df["error"].values

    if len(x) < 2:
        print("[ERREUR] Pas assez de points pour calculer une pente.")
        return

    # 3. Calcul de la Pente (Régression Linéaire sur les logs)
    # log(Error) = a * log(dx) + b
    # a est l'ordre de convergence
    log_x = np.log(x)
    log_y = np.log(y)
    
    coeffs = np.polyfit(log_x, log_y, 1)
    slope = coeffs[0]     # La pente (a)
    intercept = coeffs[1] # L'ordonnée à l'origine (b)

    print(f"Pente calculée (Ordre de convergence) : {slope:.4f}")

    # 4. Création du Graphique Log-Log
    plt.figure(figsize=(8, 6))

    # Tracer les points de mesure (Bleu)
    plt.loglog(x, y, 'bo', markersize=8, label='Erreur mesurée', zorder=5)

    # Tracer la droite de régression (Rouge)
    # y_fit = exp(a * log(x) + b) = x^a * exp(b)
    y_fit = (x ** slope) * np.exp(intercept)
    plt.loglog(x, y_fit, 'r--', linewidth=2, label=f'Régression (Pente = {slope:.2f})')

    # Ajout d'une ligne indicative "Ordre 2 théorique" pour comparer (si la pente est proche)
    # On la cale sur le dernier point pour que ce soit joli
    y_theorique = y[-1] * (x / x[-1])**2
    plt.loglog(x, y_theorique, 'g:', linewidth=1.5, label='Pente théorique (Ordre 2)')

    # 5. Esthétique
    plt.xlabel(r'Pas d\'espace $\Delta x$ (m)', fontsize=12)
    plt.ylabel(r'Erreur Globale $L_2$', fontsize=12)
    plt.title(f'Étude de Convergence (Ordre = {slope:.2f})', fontsize=14)
    plt.grid(True, which="both", linestyle="--", alpha=0.6)
    plt.legend()

    # 6. Sauvegarde
    plt.tight_layout()
    plt.savefig(OUTPUT_IMG, dpi=300)
    print(f"Graphique sauvegardé sous : {OUTPUT_IMG}")
    plt.show()

if __name__ == "__main__":
    main()