#include <ctime>     // for a random seed
#include <fstream>   // for file-reading
#include <iostream>  // for file-reading
#include <sstream>   // for file-reading
#include <vector>
#include <limits.h>

using namespace std;

struct Point {
	double x, y, z;
	int cluster;
	double minDist;

	Point() :
		x(0.0),
		y(0.0),
		z(0.0),
		cluster(-1),
		minDist(DBL_MAX) {

	}
	Point(double x, double y,double z):
		x(x),
		y(y),
		z(z),
		cluster(-1),
		minDist(DBL_MAX){ 
	}
	double distance(Point p) {
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z-z);
	}
};

vector<Point> readcsv() {
	vector<Point> points;
	string line;
	ifstream file("Mall_Customers.csv");

	while (getline(file, line)) {
		stringstream lineStream(line);
		string bit;
		double x, y, z;
		getline(lineStream, bit, ',');
		x = stof(bit);
		getline(lineStream, bit, ';');
		y = stof(bit);
		getline(lineStream, bit, '\n');
		z = stof(bit);
		points.push_back(Point(x, y, z));
	}
	return points;
}

void kMeansClustering(vector<Point>* points, int epochs, int k) {
	int n = points->size();
	
	vector<Point> centroids;
	srand(time(0));
	for (int i = 0; i < k; i++) {
		centroids.push_back(points->at(rand() % n));
	}

	for (vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) {
		
		int clusterId = c - begin(centroids);
		for (vector<Point>::iterator it = points->begin();
			it != points->end(); ++it) {

			Point p = *it;
			double dist = c->distance(p);
			if (dist < p.minDist) {
				p.minDist = dist;
				p.cluster = clusterId;
			}
			*it = p;
		}
	}
	vector<int> nPoints;
	vector<double> sumX, sumY,sumZ;

	for (int j = 0; j < k; ++j) {
		nPoints.push_back(0);
		sumX.push_back(0.0);
		sumY.push_back(0.0);
		sumZ.push_back(0.0);
	}

	for (vector<Point>::iterator it = points->begin();
		it != points->end(); ++it) {
		int clusterId = it->cluster;
		nPoints[clusterId] += 1;
		sumX[clusterId] += it->x;
		sumY[clusterId] += it->y;
		sumZ[clusterId] += it->z;

		it->minDist = DBL_MAX;  
	}

	for (vector<Point>::iterator c = begin(centroids);
		c != end(centroids); ++c) {
		int clusterId = c - begin(centroids);
		c->x = sumX[clusterId] / nPoints[clusterId];
		c->y = sumY[clusterId] / nPoints[clusterId];
		c->z = sumZ[clusterId] / nPoints[clusterId];
	}
	ofstream myfile;
	myfile.open("output.csv");
	myfile << "x,y,z,c" << endl;

	for (vector<Point>::iterator it = points->begin(); it != points->end();
		++it) {
		myfile << it->x << "," << it->y << "," <<it->z<<"," << it->cluster << endl;
	}
	myfile.close();
}

int main() {

	vector<Point> points = readcsv();
	
	kMeansClustering(&points, 100, 5);
}