#define CGAL_USE_BASIC_VIEWER

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
//#include <CGAL/draw_triangulation_2.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <vector>

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

constexpr auto READ_NUM = 50000;

int main()
{
	std::vector<Point> V;
	std::ifstream fs("../data/dem.dat");

	float x, y, z, xmin, xmax, ymin, ymax, xScale, yScale;

	xmin = ymin = FLT_MAX;
	xmax = ymax = FLT_MIN;


	for (int i = 0; i < READ_NUM; ++i) {
		fs >> x >> y >> z;
		V.push_back(Point(x, y, z));
		xmax = x > xmax ? x : xmax;
		xmin = x < xmin ? x : xmin;
		ymax = y > ymax ? y : ymax;
		ymin = y < ymin ? y : ymin;
	}

	fs.close();

	xScale = (xmax - xmin) / 5;
	yScale = (ymax - ymin) / 5;

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

	
	std::cout << "The point number:" << std::endl;
	std::cout << cdt.number_of_vertices() << std::endl;

	CGAL::make_conforming_Delaunay_2(cdt);

	int printed = 0;
	std::cout << "First " << READ_NUM / 20 << " points in cdt" << std::endl;
	for (Vertex_handle vh : cdt.finite_vertex_handles()) {
		if (printed++ > READ_NUM / 20) {
			break;
		}
		std::cout << vh->point() << std::endl;
	}
	
	//CGAL::draw(cdt);
	return EXIT_SUCCESS;
}