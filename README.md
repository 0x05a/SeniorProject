# Senior Project
This is the early works of my senior project. The project will be a chart plotter with an emphasis on navigation for sailboats, specifically sloops.

This will be written in c++ and will use the [gdal](https://gdal.org/index.html) for the vector library which will be reading charts that we can get off of [NOAA](https://www.charts.noaa.gov/InteractiveCatalog/nrnc.shtml)

## Goals:
    - Have good looking, accurate maps
    - Be able to use a GPS dongle with the program
    - Use SAT solving to figure out best course (Subject to change if I can find a better idea)
    - learn more about c++ and nautical charts
    - make writing plugins simple 

## Important Things
- http://www.s-57.com/ 

## Important Layers
- SEAARE - POLYGON sea area
- RESARE - POLYGON restricted area
- COALNE - LINESTRING coastline and water meet
- LNDARE - POLYGON | POINT land area
- LNDRGN - POLYGON land rgn
- SOUNDG - MULTIPOINT MultiPoint of points with depths
- RECTRC - ? Recommended Track (Channels)
- DEPARE - POLYGON Depth Area: Area with range of depth