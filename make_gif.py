import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# --- PARAMÈTRES DE VITESSE ---
FILENAME = 'data/P_xt.csv'
OUTPUT_GIF = 'pression_lente.gif'

# VITESSE DE LECTURE
FPS = 15           # Images par seconde (vitesse de défilement)

# FLUIDITÉ / DURÉE
# Plus ce chiffre est petit, plus l'animation est lente et fluide.
# STEP = 1  : Utilise toutes les données (Très lent, fichier lourd)
# STEP = 5  : Prend 1 point sur 5 (Bon compromis pour ralentir)
# STEP = 20 : Prend 1 point sur 20 (Rapide)
STEP = 20

def create_slow_animation():
    print("Chargement des données...")
    # Lecture optimisée
    df = pd.read_csv(FILENAME, sep='\s+', header=None, names=['x', 't', 'P'])
    df = df.sort_values(by=['t', 'x'])
    
    # Récupération des temps uniques
    all_times = df['t'].unique()
    
    # Sous-échantillonnage
    selected_times = all_times[::STEP]
    
    # Estimation de la durée
    duree_estimee = len(selected_times) / FPS
    print(f"--- INFO ANIMATION ---")
    print(f"Nombre d'images : {len(selected_times)}")
    print(f"Vitesse lecture : {FPS} images/s")
    print(f"Durée vidéo     : {duree_estimee:.1f} secondes")
    print(f"----------------------")

    # --- Configuration du graphique ---
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # Fixer les axes est CRUCIAL pour que ça ne saute pas
    x_min, x_max = df['x'].min(), df['x'].max()
    p_min, p_max = df['P'].min(), df['P'].max()
    marge = (p_max - p_min) * 0.1
    
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(p_min - marge, p_max + marge)
    
    ax.set_xlabel('Position le long de l\'artère (m)')
    ax.set_ylabel('Pression (Pa)')
    ax.grid(True, linestyle='--', alpha=0.6)
    
    # La courbe
    line, = ax.plot([], [], lw=2.5, color='#d62728', label='Pression sanguine')
    ax.legend(loc='upper right')
    
    # Texte du temps
    time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes)

    def init():
        line.set_data([], [])
        time_text.set_text('')
        return line, time_text

    def update(frame_time):
        # Filtre rapide
        subset = df[df['t'] == frame_time]
        
        line.set_data(subset['x'], subset['P'])
        time_text.set_text(f'Temps : {frame_time:.3f} s')
        ax.set_title(f"Propagation de l'onde de pression (Ralenti)")
        return line, time_text

    print("Génération en cours (patience)...")
    
    # Création de l'animation
    ani = animation.FuncAnimation(
        fig, 
        update, 
        frames=selected_times, 
        init_func=init, 
        blit=True
    )

    # Sauvegarde
    print(f"Sauvegarde du fichier '{OUTPUT_GIF}'...")
    ani.save(OUTPUT_GIF, writer='pillow', fps=FPS)
    print("Terminé ! Vous pouvez ouvrir le GIF.")

if __name__ == "__main__":
    create_slow_animation()