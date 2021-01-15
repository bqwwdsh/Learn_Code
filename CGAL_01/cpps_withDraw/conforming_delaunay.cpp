#define CGAL_USE_BASIC_VIEWER

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <vector>
#include <ctime>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Projection_traits_xy_3<K>  Gt;
typedef K::Point_3   Point;

typedef CGAL::Triangulation_vertex_base_2<Gt> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<Gt> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;

typedef CGAL::Constrained_Delaunay_triangulation_2<Gt, Tds> CDT;

typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CGAL::Delaunay_mesher_2<CDT, Criteria> Mesher;

typedef CDT::Vertex_handle Vertex_handle;

int main()
{
	std::vector<Point> V;
	std::ifstream fs("../../data/dem.dat");

	float x, y, z, xmin, xmax, ymin, ymax, xScale, yScale;

	xmin = ymin = FLT_MAX;
	xmax = ymax = FLT_MIN;

	std::clock_t startTime = clock();
	
	for (int i = 0; i < 50000; ++i) {
		fs >> x >> y >> z;
		V.push_back(Point(x, y, z));
		xmax = x > xmax ? x : xmax;
		xmin = x < xmin ? x : xmin;
		ymax = y > ymax ? y : ymax;
		ymin = y < ymin ? y : ymin;
	}

	/*
	while (!fs.eof()) {
		fs >> x >> y >> z;
		V.push_back(Point(x, y, z));
	}
	for (int i = 0; i < 50000; ++i) {
		fs >> x >> y >> z;
		V.push_back(Point(x, y, z));
		xmax = x > xmax ? x : xmax;
		xmin = x < xmin ? x : xmin;
		ymax = y > ymax ? y : ymax;
		ymin = y < ymin ? y : ymin;
	}
	*/

	fs.close();

	std::clock_t endTime = clock();

	std::cout << "read data cost: " << float(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

	xScale = (xmax - xmin) / 10;
	yScale = (ymax - ymin) / 10;

	startTime = clock();
	
	CDT cdt;
	cdt.insert(V.begin(), V.end());

	Vertex_handle
		vLeftUp = cdt.insert(Point(xmin - xScale, ymax + yScale, 1000.)),
		vLeftDown = cdt.insert(Point(xmin - xScale, ymin - yScale, 1000.)),
		vRightUp = cdt.insert(Point(xmax + xScale, ymax + yScale, 1000.)),
		vRightDown = cdt.insert(Point(xmax + xScale, ymin - yScale, 1000.));
	cdt.insert_constraint(vLeftUp, vLeftDown);
	cdt.insert_constraint(vRightUp, vRightDown);
	cdt.insert_constraint(vLeftUp, vRightUp);
	cdt.insert_constraint(vRightDown, vLeftDown);

	endTime = clock();

	std::cout << "CDT cost: " << double(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
	
	std::cout << cdt.number_of_vertices() << std::endl;

	startTime = clock();
	
	//CGAL::refine_Delaunay_mesh_2(cdt, Criteria(0.125));
	CGAL::make_conforming_Gabriel_2(cdt);

	endTime = clock();

	std::cout << "make_conforming_Delaunay_2 cost: " << double(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
	
	CGAL::draw(cdt);
	return EXIT_SUCCESS;
}