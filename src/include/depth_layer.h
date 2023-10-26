#include "ogrsf_frmts.h"

// create a class that handles 3d points
class Point3D
{
    public:
        Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
        double x;
        double y;
        double z;
        Point3D(OGRMultiPoint::ChildType *multipoint)
        {
            x = multipoint->getX();
            y = multipoint->getY();
            z = multipoint->getZ();
        }
    Point3D operator+(const Point3D& rhs)
    {
        return Point3D(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Point3D operator-(const Point3D& rhs)
    {
        return Point3D(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    std::string to_string()
    {
        return "< " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " >";
    }
};

class Point2D
{
    public:
        Point2D(double x, double y) : x(x), y(y) {}
        double x;
        double y;
        Point2D(OGRIteratedPoint &point)
        {
            x = point.getX();
            y = point.getY();
        }
    Point2D operator+(const Point2D& rhs)
    {
        return Point2D(x + rhs.x, y + rhs.y);
    }
    Point2D operator-(const Point2D& rhs)
    {
        return Point2D(x - rhs.x, y - rhs.y);
    }
    std::string to_string()
    {
        return "< " + std::to_string(x) + ", " + std::to_string(y) + " >";
    }
};

std::vector<Point3D> Get3DPoints(OGRLayer *layer);
std::vector<Point2D> Get2DPoints(OGRLayer *layer);
