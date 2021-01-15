
#define CGAL_USE_BASIC_VIEWER

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_3.h>
//#include <CGAL/draw_triangulation_2.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Projection_traits_xy_3<K>  Gt;
typedef CGAL::Triangulation_3<K> Triangulation;

typedef K::Point_3          Point;
typedef CGAL::Delaunay_triangulation_2<Gt>                   DT2;

typedef DT2::Vertex_handle Vertex_handle;

constexpr auto READ_NUM = 500000;

int main()
{
	std::vector<Point> V;
	std::ifstream fs("../data/dem.dat");

	float x, y, z;

	for (int i = 0; i < READ_NUM; ++i) {
		fs >> x >> y >> z;
		V.push_back(Point(x, y, z));
	}

	/*
	while (!fs.eof()) {
		fs >> x >> y >> z;
		V.push_back(Point(x, y, z));
	}
	*/
	
	fs.close();

	DT2 dt(V.begin(), V.end());

	std::cout << "The point number:" << std::endl;
	std::cout << dt.number_of_vertices() << std::endl;
	
	int printed = 0;
	std::cout << "First " << READ_NUM / 20 << " points in cdt" << std::endl;
	for (Vertex_handle vh : dt.finite_vertex_handles()) {
		if (printed++ > READ_NUM / 20) {
			break;
		}
		std::cout << vh->point() << std::endl;
	}
	

	
	//CGAL::draw(dt);
	return EXIT_SUCCESS;
}