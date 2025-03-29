#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

const int MAX_COLOR = 255;
const int WIDTH_MAX = 1000;
const int HEIGHT_MAX = 1000;
const int M_STEPS = 1000; // nombre de valeurs de m testées
const int B_STEPS = 1000; // nombre de valeurs de b testées
const double M_MIN = -10.0, M_MAX = 10.0;

struct Pixel {
    int r, g, b;
    bool isEdge() const {
        return (r + g + b) > 100; // Seuil simple pour détecter un pixel "non noir"
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./Methodenaive <droite_image.ppm>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output("droite_image.ppm");

    if (!input) {
        std::cerr << "Erreur d'ouverture de l'image : " << argv[1] << std::endl;
        return 1;
    }

    std::string format;
    int width, height, max_val;
    input >> format >> width >> height >> max_val;

    std::vector<std::vector<Pixel>> image(height, std::vector<Pixel>(width));
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            input >> image[i][j].r >> image[i][j].g >> image[i][j].b;

    // Création de l'accumulateur m-b
    std::vector<std::vector<int>> accumulator(M_STEPS, std::vector<int>(B_STEPS, 0));

    // Remplissage de l'accumulateur
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!image[y][x].isEdge()) continue;
            for (int mi = 0; mi < M_STEPS; ++mi) {
                double m = M_MIN + (M_MAX - M_MIN) * mi / M_STEPS;
                double b = y - m * x;
                int bi = static_cast<int>((b + height) * B_STEPS / (2 * height));
                if (bi >= 0 && bi < B_STEPS)
                    accumulator[mi][bi]++;
            }
        }
    }

    // Trouver le maximum de l'accumulateur
    int maxVotes = 0;
    int best_mi = 0, best_bi = 0;
    for (int mi = 0; mi < M_STEPS; ++mi) {
        for (int bi = 0; bi < B_STEPS; ++bi) {
            if (accumulator[mi][bi] > maxVotes) {
                maxVotes = accumulator[mi][bi];
                best_mi = mi;
                best_bi = bi;
            }
        }
    }

    double best_m = M_MIN + (M_MAX - M_MIN) * best_mi / M_STEPS;
    double best_b = (double)best_bi * (2 * height) / B_STEPS - height;

    // Tracer la droite sur l'image
    for (int x = 0; x < width; ++x) {
        int y = static_cast<int>(best_m * x + best_b);
        if (y >= 0 && y < height) {
            image[y][x].r = 255;
            image[y][x].g = 0;
            image[y][x].b = 0;
        }
    }

    // Sauvegarde de l'image
    output << "P3\n" << width << " " << height << "\n" << MAX_COLOR << "\n";
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            output << image[i][j].r << " " << image[i][j].g << " " << image[i][j].b << "\n";

    std::cout << "Droite detectee : y = " << best_m << "x + " << best_b << std::endl;
    return 0;
}
