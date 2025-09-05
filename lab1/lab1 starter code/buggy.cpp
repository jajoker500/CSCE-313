#include <iostream>
#include <cstring>

struct Point {
    int x, y;

    Point () : x(), y() {}
    Point (int _x, int _y) : x(_x), y(_y) {}
};

class Shape {
    int vertices;
    Point** points;

    public: // need these public to run
    Shape (int _vertices) {
        vertices = _vertices;
        points = new Point*[vertices+1];

        for(int i = 0; i < vertices + 1; ++i) { // needes vertices + 1 because its size is that and needs memory allocated for each 
            points[i] = new Point(); // needed to create this loop to initialize the points with allocated memory
        }
    }

    ~Shape () {
        for(int i = 0; i < vertices + 1; ++i) {
            delete points[i]; // deallocate each new inside array
        }
        delete[] points; // deallocate actual array
    }

    void addPoints (Point pts[]) {
        for (int i = 0; i < vertices; i++) {
            memcpy(points[i], &pts[i%vertices], sizeof(Point)); // needed to include necessary header cstring also stop at vertices
        }
        memcpy(points[vertices], &pts[0], sizeof(Point)); // because shoelace needs wrapping use first point again for final point
    }

    double area () {
        int temp = 0;
        for (int i = 0; i < vertices; i++) {
            // FIXME: there are two methods to access members of pointers
            //        use one to fix lhs and the other to fix rhs
            int lhs = (*points[i]).x * (*points[i+1]).y; // used (*var).x method one to access same as ->
            int rhs = points[i+1]->x * points[i]->y; // method two is -> more common and simple pointer
            temp += (lhs - rhs);
        }
        double area = abs(temp)/2.0;
        return area; // just needed value not address so returning value is fine
    }
};

int main () {
    // FIXME: create the following points using the three different methods
    //        of defining structs:
    //          tri1 = (0, 0)
    //          tri2 = (1, 2)
    //          tri3 = (2, 0)
    Point tri1 = {0, 0}; // method one with generic struct configuration where x = 0 y = 0

    Point tri2; // default
    tri2.x = 1; // now set x = 1
    tri2.y = 2; // and y = 2 seperately this is method 2

    Point tri3(2, 0); // for method 3 use constructor to fill with parameters where x =2 and y = 0

    // adding points to tri
    Point triPts[3] = {tri1, tri2, tri3};
    Shape* tri = new Shape(3);
    tri->addPoints(triPts); // need arrow because  tri is on heap

    // FIXME: create the following points using your preferred struct
    //        definition:
    //          quad1 = (0, 0)
    //          quad2 = (0, 2)
    //          quad3 = (2, 2)
    //          quad4 = (2, 0)
    Point quad1 = {0, 0}; // simply created structs for each of the four points 
    Point quad2 = {0, 2};
    Point quad3 = {2, 2};
    Point quad4 = {2, 0};

    // adding points to quad
    Point quadPts[4] = {quad1, quad2, quad3, quad4};
    Shape* quad = new Shape(4);
    quad->addPoints(quadPts); // need arrow because quad is a Shape type on the heap

    // FIXME: print out area of tri and area of quad
    std::cout << "The area of the tri is: " << tri->area() << std::endl; // output the tri area
    std::cout << "The area of the quad is: " << quad->area() << std::endl; // output the quad area
}
