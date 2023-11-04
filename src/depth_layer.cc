#include <ogrsf_frmts.h>
#include <assert.h>
#include "include/depth_layer.h"
#include <cassert>
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
/**
 * 
 * @param  {OGRLayer*} poLayer     : 
 * @return {std::vector<Point2D>}  : 
 */
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
    return points;
}

/** Function to return a list of points that we can chart along with the depth
 * 
 * @param  {OGRLayer*} poLayer     : Layer we are interested in
 * @param  {int} video_width       : video width so we scale properly
 * @param  {int} video_height      : video height so we scale properly
 * @return {std::vector<Point3D>}  : returns a list of points that we can chart
 */
std::vector<Point3D> GetDepthPoints(OGRLayer *poLayer, int video_width, int video_height)
{
    std::vector<Point3D> depth_points;
    auto points = Get3DPoints(poLayer);
   // get extent
    OGREnvelope layer_extent;
    auto extent = poLayer->GetExtent(&layer_extent);
    assert(extent == OGRERR_NONE);
    // get corners
    auto max_lat = layer_extent.MaxX;
    auto max_lon = layer_extent.MaxY;
    auto min_lat = layer_extent.MinX;
    auto min_lon = layer_extent.MinY;
    // get corners
    auto tl_corner = Point2D(max_lat, min_lon);
    auto tr_corner = Point2D(max_lat, max_lon);
    auto bl_corner = Point2D(min_lat, min_lon);
    auto br_corner = Point2D(min_lat, max_lon);

    int const MERC_CONST = 1000;
    // get the mercantor of the corners
    auto merc_tl_corner = tl_corner.mercator_x_y(MERC_CONST, MERC_CONST);
    auto merc_tr_corner = tr_corner.mercator_x_y(MERC_CONST, MERC_CONST);
    auto merc_bl_corner = bl_corner.mercator_x_y(MERC_CONST, MERC_CONST);
    auto merc_br_corner = br_corner.mercator_x_y(MERC_CONST, MERC_CONST);
    // merc transformation min and max lon and lat
    auto merc_max_lat = merc_tr_corner.getLatitude();
    auto merc_max_lon = merc_tr_corner.getLongitude();
    auto merc_min_lat = merc_bl_corner.getLatitude();
    auto merc_min_lon = merc_bl_corner.getLongitude();
    // merc transformation ratio of lat and lon
    auto merc_lat_delta = (merc_max_lat - merc_min_lat);
    auto merc_lon_delta = (merc_max_lon - merc_min_lon);
    // merc transformation of the points to fit the video
    for (auto& point: points)
    {
        //transform it
         auto merc_point = point.mercator_x_y(MERC_CONST, MERC_CONST);
         // scale appropriately, since these are coordinates from the top left, we must subtract the height
         double merc_lat_ratio = (merc_point.getLatitude() - merc_min_lat) / merc_lat_delta;
         double y_lat = video_height - merc_lat_ratio * video_height;
         
         double merc_lon_ratio = (merc_point.getLongitude() - merc_min_lon) / merc_lon_delta;
         double x_long = merc_lon_ratio * video_width;

         // append new point confusing name but our x is latitude and our y is longitude 
         // makes more sense when we render it
         auto new_point = Point3D(y_lat, x_long, point.getDepth());
         depth_points.push_back(new_point);
    } 
    return depth_points;
}
