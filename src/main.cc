#include <SFML/Graphics.hpp>
#include "include/depth_layer.h"
#include <iostream>
#include <stdlib.h>


int main()
{
    GDALAllRegister();
    GDALDataset *poDS;
    const char *filename = "charts/BYU_CHICO.000";
    poDS = (GDALDataset *) GDALOpenEx(filename, GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (poDS == nullptr)
    {
        printf("Open Failed.\n");
        exit(1);
    }
    std::cout << "Opened " << filename << "\n";
    OGRLayer *poSOUNDG_layer;
    poSOUNDG_layer = poDS->GetLayerByName("SOUNDG");
    if (poSOUNDG_layer == NULL)
    {
        perror("Failed to get layer SOUNDG");
        exit(1);
    }
    std::cout << "Got layer SOUNDG\n";
    auto point_vector = Get3DPoints(poSOUNDG_layer);
    for (auto& point: point_vector)
    {
        std::cout << point.to_string() << std::endl;
    }
    std::cout << "Wrote " << point_vector.size() << " points\n";
    return 0;
}

//    sf::RenderWindow window(sf::VideoMode(500, 500), "Chart Plotter");

//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();

//        window.clear(sf::Color::Blue);
//        window.display();
//    }

//    return 0;
// }
