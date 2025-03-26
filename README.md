#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

// Dimensions de l'image
const int WIDTH = 100;
const int HEIGHT = 100;
const int THETA_MAX = 180;
const int RHO_MAX = sqrt(WIDTH * WIDTH + HEIGHT * HEIGHT);

// Accumulateur pour stocker les votes
vector<vector<int>> accumulator(2 * RHO_MAX, vector<int>(THETA_MAX, 0));

// Fonction pour appliquer la transformée de Hough naïve
void houghTransform(const vector<vector<int>>& image) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Vérifier si le pixel est un bord (valeur non nulle)
            if (image[y][x] > 0) {
                // Parcourir tous les angles possibles
                for (int theta = 0; theta < THETA_MAX; ++theta) {
                    double radians = theta * M_PI / 180.0;
                    int rho = round(x * cos(radians) + y * sin(radians));
                    rho += RHO_MAX; // Décalage pour éviter les indices négatifs
                    accumulator[rho][theta]++;
                }
            }
        }
    }
}

// Fonction pour exporter l'accumulateur en format PPM
void exporterPPM(const string& filename) {
    ofstream fichier(filename);
    if (!fichier) {
        cerr << "Erreur lors de l'ouverture du fichier PPM" << endl;
        return;
    }
    
    fichier << "P3\n" << THETA_MAX << " " << 2 * RHO_MAX << "\n255\n";
    
    for (int rho = 0; rho < 2 * RHO_MAX; ++rho) {
        for (int theta = 0; theta < THETA_MAX; ++theta) {
            int valeur = min(255, accumulator[rho][theta] * 10); // Mise à l'échelle
            fichier << valeur << " " << valeur << " " << valeur << " ";
        }
        fichier << "\n";
    }
    
    fichier.close();
    cout << "Accumulateur exporté sous " << filename << endl;
}

// Fonction principale
int main() {
    // Initialisation d'une image binaire (exemple simple)
    vector<vector<int>> image(HEIGHT, vector<int>(WIDTH, 0));
    
    // Tracer une ligne verticale artificielle
    for (int y = 20; y < 80; ++y) {
        image[y][50] = 255;
    }
    
    // Appliquer la transformée de Hough
    houghTransform(image);
    
    // Exporter l'accumulateur en PPM
    exporterPPM("accumulateur.ppm");
    
    cout << "Transformée de Hough appliquée avec succès." << endl;
    return 0;
}
