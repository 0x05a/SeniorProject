#include "ogrsf_frmts.h"

// create a class that handles 3d points
class Point3D
{
    private:
    bool m_merc = false;
    double m_latitude;
    double m_longitude;
    double m_depth;

    public:
        Point3D(double latitude, double longitude, double depth) : m_latitude(latitude), m_longitude(longitude), m_depth(depth) {}
        double z;
        Point3D(OGRMultiPoint::ChildType *multipoint)
        {
            m_latitude = multipoint->getX();
            m_longitude = multipoint->getY();
            m_depth = multipoint->getZ();
        }
    double getLatitude()
    {
        return m_latitude;
    }
    double getLongitude()
    {
        return m_longitude;
    }
    double getDepth()
    {
        return m_depth;
    }
    void setLatitude(double latitude)
    {
        m_latitude = latitude;
    }
    void setLongitude(double longitude)
    {
        m_longitude = longitude;
    }
    void set_Depth(double depth)
    {
        m_depth = depth;
    }
    Point3D operator+(const Point3D& rhs)
    {
        return Point3D(m_latitude + rhs.m_latitude, m_longitude + rhs.m_longitude, m_depth + rhs.m_depth);
    }
    Point3D operator-(const Point3D& rhs)
    {
        return Point3D(m_latitude - rhs.m_latitude, m_longitude - rhs.m_longitude, m_depth - rhs.m_depth);
    }
    std::string to_string()
    {
        // display lat then lon then z
        return "< " + std::to_string(m_latitude) + ", " + std::to_string(m_longitude) + ", " + std::to_string(z) + " >";
    }
    // mercator project on a width by height display.

    Point3D mercator_x_y(double width, double height)
    {
    if (m_merc == true)
    {
        auto ret_point = Point3D(m_latitude, m_longitude, m_depth);
        ret_point.m_merc = true;
        return ret_point;
    }

    const double PI = 3.14159265359;
    double transformed_x = fmod((width*(180+m_longitude)/360), (width +(width/2)));
    double latRad = m_latitude * PI/180;
    double mercN = log(tan((PI/4)+(latRad/2)));
    double transformed_y = (height/2)-(width*mercN/(2*PI));
    m_merc = true;

    return Point3D(transformed_x, transformed_y, m_depth); 
    }
    // haversine formula
    double distanceMiles(Point3D &other)
    {
        // https://en.wikipedia.org/wiki/Haversine_formula
        // using haversine to find distances between lat and lon
        const double PI = 3.14159265359;
        long other_lon = other.m_longitude;
        long other_lat = other.m_latitude;
        double R = 3958.8; // Radius of the Earth in miles
        double dLat = (other_lat - m_latitude) * PI / 180;
        double dLon = (other_lon - m_longitude) * PI / 180;
        double a = sin(dLat / 2) * sin(dLat / 2) +
                cos(m_latitude * PI / 180) * cos(other_lat * PI / 180) *
                sin(dLon / 2) * sin(dLon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        double d = R * c;
        return d;
    }
};

class Point2D
{
    private:
    bool m_merc = false;
    double m_longitude;
    double m_latitude;

    public:
        Point2D(double latitude, double longitude) : m_longitude(longitude), m_latitude(latitude) {}
        Point2D(OGRIteratedPoint &point)
        {
            m_latitude = point.getX();
            m_longitude = point.getY();
        }
    Point2D operator+(const Point2D& rhs)
    {
        return Point2D(m_longitude + rhs.m_longitude, m_latitude + rhs.m_latitude);
    }
    Point2D operator-(const Point2D& rhs)
    {
        return Point2D(m_longitude - rhs.m_longitude, m_latitude - rhs.m_latitude);
    }
    
    double getLatitude()
    {
        return m_latitude;
    }
    double getLongitude()
    {
        return m_longitude;
    }
    void setLatitude(double latitude)
    {
        m_latitude = latitude;
    }
    void setLongitude(double longitude)
    {
        m_longitude = longitude;
    }
    std::string to_string()
    {
        // displam_latitude lat then lon
        return "< " + std::to_string(m_latitude) + ", " + std::to_string(m_longitude) + " >";
    }
    Point2D mercator_x_y(double width, double height)
    {
    if (m_merc == true)
    {
        return Point2D(m_longitude, m_latitude);
    }

    const double PI = 3.14159265359;
    double lon = m_longitude;
    double lat = m_latitude;
    double transformed_longitude = fmod((width*(180+lon)/360), (width +(width/2)));
    double latRad = lat * PI/180;
    double mercN = log(tan((PI/4)+(latRad/2)));
    double transformed_latitude = (height/2)-(width*mercN/(2*PI));
    m_merc = true;

    return Point2D(transformed_longitude, transformed_latitude); 
    }
};

std::vector<Point3D> Get3DPoints(OGRLayer *layer);
std::vector<Point2D> Get2DPointsFromPolyGon(OGRLayer *layer);
std::vector<Point3D> GetDepthPoints(OGRLayer *poLayer, int video_width, int video_height);