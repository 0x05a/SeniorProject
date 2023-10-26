#include <ogrsf_frmts.h>
#include "../include/depth_layer.h"
#include <iostream>
#include <assert.h>
int main()
{
    GDALAllRegister();
    GDALDataset *poDS;
    const char *filename = "../charts/BYU_CHICO.000";
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
    std::cout << "using layer SOUNDG\n";
    auto point_vector = Get3DPoints(poSOUNDG_layer);
    for (auto& point: point_vector)
    {
        std::cout << point.to_string() << std::endl;
    }
    std::cout << "Wrote " << point_vector.size() << " points\n";
    assert (point_vector.size() == 145);

    return 0;
}