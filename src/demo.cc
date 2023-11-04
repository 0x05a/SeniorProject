#include <SFML/Graphics.hpp>
#include "ogrsf_frmts.h"
#include <iostream>
#include "include/depth_layer.h"
#include <cassert>

const int VIDEO_WIDTH = 1000;
const int VIDEO_HEIGHT = 1000;

int main()
{
    GDALAllRegister();
    GDALDataset *poDS;
    poDS = (GDALDataset *) GDALOpenEx("charts/BYU_CHICO.000", GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (poDS == NULL)
    {
        printf("Open Failed.\n");
        exit(1);
    }
    printf("Successfully open'd BYU_CHICO.000\n");
    OGRLayer *SOUNDG;
    SOUNDG = poDS->GetLayerByName("SOUNDG");
    OGREnvelope layer_extent;
    auto try_extent = SOUNDG->GetExtent(&layer_extent);
    if (try_extent != OGRERR_NONE)
    {
        std::cout << "Failed to get extent" << std::endl;
    }
    std::cout << "Max Latitude: " << layer_extent.MaxX << std::endl;
    std::cout << "Min Latitude: " << layer_extent.MinX << std::endl;
    std::cout << "Max Longitude: " << layer_extent.MaxY << std::endl;
    std::cout << "Min Longitude: " << layer_extent.MinY << std::endl;

    auto points = GetDepthPoints(SOUNDG, VIDEO_WIDTH, VIDEO_HEIGHT);
    GDALClose(poDS);

    // set up render window
    sf::RenderWindow window(sf::VideoMode(VIDEO_WIDTH, VIDEO_HEIGHT), "Demo Chart");
    window.setFramerateLimit(60);
    // plot points
    for (auto& point : points)
    {
        sf::CircleShape circle(2);
        circle.setPosition(point.getLongitude(), point.getLatitude());
        if (point.getDepth() > 2)
            circle.setFillColor(sf::Color::Green);
        else
            circle.setFillColor(sf::Color::Red);
        window.draw(circle);
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // close window on close
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.display();
    }
    return 0;
}
