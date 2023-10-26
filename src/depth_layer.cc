#include <ogrsf_frmts.h>
#include <assert.h>
#include "include/depth_layer.h"
#include <iostream>
/**
 * 
 * @param  {OGRLayer*} poLayer     : 
 * @return {std::vector<Point3D>}  : 
 */
std::vector<Point3D> Get3DPoints(OGRLayer *poLayer)
{
    std::vector<Point3D> points;

    for (auto& poFeature: poLayer)
    {
        auto poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != nullptr && poGeometry->getGeometryType() == wkbMultiPoint25D)
        {
            auto multipoint = (OGRMultiPoint *) poGeometry;
            for (auto&& point: multipoint)
            {
                points.push_back(Point3D(point));
            }
        }
    }
    return points;
}

std::vector<Point2D> Get2DPointsFromPolyGon(OGRLayer *poLayer)
{
    std::vector<Point2D> points;
    for (auto& poFeature: poLayer)
    {
        auto poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != nullptr && poGeometry->getGeometryType() == wkbPolygon)
        {
            auto polygon = (OGRPolygon *) poGeometry;
            auto ring = polygon->getExteriorRing();
            for (auto&& point: ring)
            {
                points.push_back(Point2D(point));
            }
        }
    }
}