#include "raylib.h"
#include "DataLoader.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string error;
    std::string path = std::string(DATA_DIR) + "/AAPL.csv";
    std::vector<Candle> candles = DataLoader::loadCSV(path, error);

    if (!error.empty()) std::cout << error << "\n";

    std::cout << "Loaded " << candles.size() << " candles\n";
    for (size_t i = 0; i < candles.size() && i < 5; i++) {
        const Candle& c = candles[i];
        std::cout << c.date << "  O=" << c.open << " H=" << c.high
                  << " L=" << c.low << " C=" << c.close << " V=" << c.volume << "\n";
    }
    if (!candles.empty())
        std::cout << "Last: " << candles.back().date
                  << " close=" << candles.back().close << "\n";

    InitWindow(1280, 720, "MiniChart");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{19, 23, 34, 255});
        DrawText(TextFormat("Loaded %d candles", (int)candles.size()), 40, 30, 32, RAYWHITE);
        DrawFPS(1180, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}