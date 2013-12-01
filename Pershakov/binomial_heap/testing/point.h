#ifndef POINT_H

#define POINT_H

class Point {
    public:
        Point(){
            x =  y = 0;
        }

        Point(int new_x, int new_y){
            x = new_x;
            y = new_y;
        }

        int getX() const {
            return x;
        }

        int getY() const {
            return y;
        }

        void setX(int new_x){
            x = new_x;
        }

        void setY(int new_y){
            y = new_y;
        }

        long long sqrDist(const Point &a){
            long long dx = x - a.x;
            long long dy = y - a.y;
            return dx * dx + dy * dy;
        }

    private:
        int x, y;
};

class PointComparator {
    public:
        PointComparator(): o(0, 0) {
        }

        explicit PointComparator(Point new_o): o(new_o) {
        }

        PointComparator(int new_x, int new_y): o(new_x, new_y) {
        }

        bool operator() (const Point &a, const Point &b){
            return o.sqrDist(a) < o.sqrDist(b);
        }

        Point getO(){
            return o;
        }

    private:
        Point o;
};

class RandomPointGenerator {
    public:
        RandomPointGenerator(){
            srand(time(0));
        }

        explicit RandomPointGenerator(const PointComparator &new_cmp){
            cmp = new_cmp;
            srand(time(0));
        }

        Point operator() (){
            int x = rand() % 100;
            int y = rand() % 100;
            return Point(x, y);
        }

        Point decrease(const Point &b){
            if (!cmp(b, cmp.getO()) && !cmp(cmp.getO(), b))
                return b;
            Point c = b;
            c.setY(c.getY() + 1);
            if (cmp(c, b))
                return c;

            c = b;
            c.setY(c.getY() - 1);
            if (cmp(c, b))
                return c;

            c = b;
            c.setX(c.getX() + 1);
            if (cmp(c, b))
                return c;

            c = b;
            c.setX(c.getX() - 1);
            if (cmp(c, b))
                return c;
            
            return c;
        }



    private:
        PointComparator cmp;
};

#endif
