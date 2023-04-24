#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <ctime>


using namespace std;

double R = 6371.0;


class Coordinates{

    int x, y, z;
    double lat, lon, alt;
public:
    Coordinates(int x1, int y1){
        x = x1;
        y = y1;
    }
    Coordinates(double lat, double lon, double alt = 0){
        this->lat = lat;
        this->lon = lon;
        x = R * cos(lat) * cos(lon);
        y = R * cos(lat) * sin(lon);
        z = R * sin(lat);
        this->alt = alt;
    }
    int get_x(){
        return x;
    }
    int get_y(){
        return y;
    }
    void set_x(int x1){
        x = x1;
    }
    void set_y(int y1){
        y = y1;
    }
    double get_lat(){
        return lat;
    }
    double get_lon(){
        return lon;
    }
    double get_alt(){
        return alt;
    }
    void set_lat(double lat){
        this->lat = lat;
        z = R * sin(this->lat);
    }
    void set_lon(double lon){
        this->lon = lon;
        x = R * cos(this->lat) * cos(this->lon);
        y = R * cos(this->lat) * sin(this->lon);
    }
    
};  

int dist_point(Coordinates *XY1, Coordinates *XY2){
    return sqrt(pow((XY2->get_x() - XY1->get_x()), 2) + pow((XY2->get_y() - XY1->get_y()), 2));
}
double distance_points_lat_lon(double lat1, double lon1, double lat2, double lon2){
    double sin1 = sin( (lat1-lat2) / 2.0 );
    double sin2 = sin( (lon1-lon2) / 2.0 );
    return 2.0 * R * asin( sqrt( sin1 * sin1 + sin2 * sin2 * cos(lat1) * cos(lat2) ) );
}


class Object{
private:
    int id;
    int path = 0;
    Coordinates *posxy;

    int speed = 26;
protected:
    vector<Coordinates*> coordinates;

public:
    Object(int id, int x, int y){
        this->id = id;
        posxy = new Coordinates(x, y);
        coordinates.push_back(posxy);
        
    }
    Object(int id){
        this->id = id;
        posxy = new Coordinates(0.0, 0.0);
        coordinates.push_back(posxy);
        
    }
    Coordinates *get_position(){
        return posxy;
    }
    int get_id(){
        return id;
    }
    int get_path(){
        return path;
    }
    int get_speed(){
        return speed;
    }
    void set_sum(double path){
        this->path += path;
    }
    int get_count_point(){
        return coordinates.size();
    }
    virtual void randomwalk(int mapx, int mapy){
        int nap = rand() % 8;//направление
        int x = 0, y = 0;
        //Coordinates *newposxy = new Coordinates(posxy->get_x(), posxy->get_y());
        if(nap == 0 ){
            if(posxy->get_x() + speed <= mapx)
                x = posxy->get_x() + speed;
        }
        else if(nap == 1){
            if(posxy->get_x() - speed >= 0)
                x = posxy->get_x() - speed;
        }
        else if(nap == 2){
            if(posxy->get_y() + speed <= mapy)
                y = posxy->get_y() + speed;
        }
        else if(nap == 3){
            if(posxy->get_y() - speed >= 0)
                y = posxy->get_y() - speed;
        }
        if(x != 0 || y != 0){
            Coordinates *newposxy = new Coordinates(x, y);
            coordinates.push_back(newposxy);
            path += dist_point(newposxy, posxy);
            posxy = newposxy;
        }
        
    } 
    int get_path_coord(Coordinates *point){
        return dist_point(posxy, point);
    }
    void set_lat_lon(double lat, double lon){
        posxy->set_lat(lat);
        posxy->set_lon(lon);
    }
    void set_pos(Coordinates *newp){
        posxy = newp;
    }


};

class UserEquipment : public Object{

public:
    UserEquipment(int id, double lat, double lon, double scale) : Object(id){
        
        set_lat_lon(lat, lon);
        this->scale = scale;
        
    }
    void randomwalk(int mapx, int mapy){
        int nap = rand() % 8;//направление
        int x = 0, y = 0;
        //Coordinates *newposxy = new Coordinates(posxy->get_x(), posxy->get_y());
        Coordinates *posxy = get_position();
        int speed = get_speed();
        if(nap == 0 ){
            if(posxy->get_x() + speed <= mapx)
                x = posxy->get_x() + speed;
        }
        else if(nap == 1){
            if(posxy->get_x() - speed >= 0)
                x = posxy->get_x() - speed;
        }
        else if(nap == 2){
            if(posxy->get_y() + speed <= mapy)
                y = posxy->get_y() + speed;
        }
        else if(nap == 3){
            if(posxy->get_y() - speed >= 0)
                y = posxy->get_y() - speed;
        }
        if(x != 0 || y != 0){
            double lat, lon;
            lat = posxy->get_lat() + (double)x * scale;
            lon = posxy->get_lon() + (double)y * scale;
            
            Coordinates *newposxy = new Coordinates(lat, lon, 0.0);

            coordinates.push_back(newposxy);
            //path += dist_point(newposxy, posxy);
            double D = distance_points_lat_lon(posxy->get_lat(), posxy->get_lon(), lat, lon);
            set_sum(D);
            set_pos(newposxy);
            //posxy = newposxy;
        }
        
    } 

private:
    double scale;

};

vector <Object*> Create_UserEquipment(int mapx, int mapy, double latA, double lonA, double scale){
    int count;
    cout<<"Input count objects(1 - 100): ";
    cin>>count;
    vector <Object*> obj;
    Object *newobj;
    int i, i2;
    int id;
    int create_id;
    double pos_lat, pos_lon;
    for(i = 0; i < count; ++i){
        create_id = 0;
        
        while(create_id != 1){
            create_id = 1;
            id = 99 + (rand() % 10000);
            for(i2 = 0; i2 < i; ++i2){
                if(id == obj[i2]->get_id()){
                    create_id = 0;
                    break;
                }
            }

        }
        
        cout<<"id = "<<id<<endl;
        pos_lat = latA + (rand() % mapx) * scale;
        pos_lon = lonA + (rand() % mapy) * scale;
        
        //newobj = new Object(id, rand() % mapx, rand() % mapy);
        newobj = new UserEquipment(id, pos_lat, pos_lon, scale);
        
        obj.push_back(newobj);
    }
    return obj;
}

void randomwalk_objs(vector <Object*> *objs, int mapx, int mapy){
    int count;
    cout<<"Input count iteration move(1 - 1000): ";
    cin>>count;
    int i, i2;
    for(i = 0; i < count; ++i){
        cout<<"iteration: "<<i<<"\n";
        for(i2 = 0; i2 < objs->size(); ++i2){
            objs->at(i2)->randomwalk(mapx, mapy);

            cout<<"\tid = "<<objs->at(i2)->get_id()<<"\tlat = "<<objs->at(i2)->get_position()->get_lat()<<"\tlon = "<< objs->at(i2)->get_position()->get_lon()<<"\n";
        }
    }
    Object *ptr, *ptr2;
    for(i2 = 0; i2 < objs->size(); ++i2){
        ptr = objs->at(i2);

        cout<<"id - "<<ptr->get_id()<<"\n";
        cout<<"\tpath = "<<ptr->get_path()<<"\n";
        cout<<"\tcount point = "<<ptr->get_count_point()<<"\n";
        cout<<"\tpath to \n";
        for(i = 0; i < objs->size(); ++i){
            if(i != i2){
                ptr2 = objs->at(i);
                cout<<"\t\t[id - "<<ptr2->get_id()<<"] = "<<ptr->get_path_coord(ptr2->get_position())<<"\n";
            }
        }
    }
    cout<<"End iteratrion move\n";
}

int main(){
    srand(time(NULL));
    int size_x = 1000, size_y = 1000;
    double latA = 55.084656, lonA = 82.4626;
    double latB = 54.874631, lonB = 82.8410;
    vector <Object*> objs;
    double scale = (lonB - lonA)/(double)size_x;
    objs = Create_UserEquipment(size_x, size_y, latA, latB, scale);
    randomwalk_objs(&objs, size_x, size_y);

    cout<<"End program\n";
    
}
