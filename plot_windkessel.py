import matplotlib.pyplot as plt
import pandas as pd

# 1. Chargement des données "Couplé" (Le système complet)
try:
    df_P_c = pd.read_csv('data/P_wk_coupled.csv', header=None, names=['t', 'val'])
    df_Q_c = pd.read_csv('data/Q_wk_coupled.csv', header=None, names=['t', 'val'])
    
    # 2. Chargement des données "Seul" (Le test unitaire)
    df_P_a = pd.read_csv('data/P_wk_alone.csv', header=None, names=['t', 'val'])
    df_Q_a = pd.read_csv('data/Q_wk_alone.csv', header=None, names=['t', 'val'])
except FileNotFoundError:
    print("Erreur : Fichiers CSV manquants. Lance './solver parameters.in' d'abord.")
    exit()

plt.figure(figsize=(12, 10))

# --- Graphique Pression ---
plt.subplot(2, 1, 1)
# Courbe Couplée (Venant de l'artère)
plt.plot(df_P_c['t'], df_P_c['val'] / 133.322, label='Modèle Couplé (Artère)', color='blue', linewidth=2)
# Courbe Seule (Test unitaire impulsion)
plt.plot(df_P_a['t'], df_P_a['val'] / 133.322, label='Modèle Seul (Test Unitaire)', color='red', linestyle='--')

plt.ylabel('Pression (mmHg)')
plt.title('Comparaison : Windkessel Couplé vs Windkessel Isolé')
plt.legend()
plt.grid(True, alpha=0.5)

# --- Graphique Débit ---
plt.subplot(2, 1, 2)
# Débit sortant de l'artère
plt.plot(df_Q_c['t'], df_Q_c['val'] * 1e6, label='Débit Couplé (Sortie PDE)', color='blue', linewidth=2)
# Débit imposé artificiellement
plt.plot(df_Q_a['t'], df_Q_a['val'] * 1e6, label='Débit Test (Sinus)', color='red', linestyle='--')

plt.ylabel('Débit (mL/s)')
plt.xlabel('Temps (s)')
plt.legend()
plt.grid(True, alpha=0.5)

plt.tight_layout()
plt.savefig('Comparaison_Windkessel.png', dpi=300)
plt.show()