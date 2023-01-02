// ==============================================================
//
//  Copyright (C) 2004-2016  Alex Vinokur.
//
//  ------------------------------------------------------------
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//  ------------------------------------------------------------
// 
//  mailto:alex DOT vinokur AT gmail DOT com
//  http://sourceforge.net/users/alexvn/
//
// ==============================================================


// ##############################################################
//
//  SOFTWARE : Class BigInt
//  FILE     : main.cpp
//
//  DESCRIPTION :
//          Nain program
//
// ##############################################################


// ===============
#include "bigInt.h"
#include"ElipticCurve.h"
#include"Host.h"
#include<vector>
#include<iostream>
#include<gl/glut.h>
// ===============

// --------------------
#pragma region test_library
static void simpleDemoBigInt(std::ostream& o_stream)
{

	SET_START_TEST_NAME(o_stream);

#define SHOW_BIG_INT(x) o_stream << #x << " : Hex Value = " << x.toStr0xHex() << ", Hex Digits = " << x.getActualHexDigits() << "; Dec Value = " << x.toStrDec() << ", Dec Digits = " << x.getActualDecDigits() << std::endl << std::endl
#define SHOW_DOUBLE(x)  o_stream << #x << " : " << std::fixed << x << std::endl << std::endl

	BigInt::Rossi n1("314159265358979323846264338327950288419716939937510", BigInt::DEC_DIGIT);
	SHOW_BIG_INT(n1);

	BigInt::Rossi n2("224f3e07282886cce82404b6f8", BigInt::HEX_DIGIT);
	SHOW_BIG_INT(n2);

	BigInt::Rossi n3(1123);
	SHOW_BIG_INT(n3);

	BigInt::Rossi n4 = n1 + n2;
	SHOW_BIG_INT(n4);

	BigInt::Rossi n5 = n1 * n3;
	SHOW_BIG_INT(n5);

	BigInt::Rossi n6 = n2 / n3;
	SHOW_BIG_INT(n6);

	BigInt::Rossi n7 = n1.sqrt();
	SHOW_BIG_INT(n7);

	BigInt::Rossi n8 = BigInt::Rossi::pow(n1, n3);
	SHOW_BIG_INT(n8);

	long double d1 = n1.toDouble();
	SHOW_DOUBLE(d1);

	long double d2 = 1123581321345589144233.0;
	SHOW_DOUBLE(d2);

	BigInt::Rossi n9 = BigInt::Rossi::fromDouble(d2);
	SHOW_BIG_INT(n9);

#undef SHOW_BIG_INT
#undef SHOW_DOUBLE

	// --------------------------
	SET_FINISH_TEST_NAME(o_stream);

	o_stream << std::endl;
	o_stream << std::endl;
	o_stream << std::endl;
	o_stream << std::endl;

}
static void demoBigInt(std::ostream& o_stream, const std::string& i_exeFileName)
{
	std::ostringstream oss;

	SET_START_TEST_NAME(oss);

	BigInt::Rossi arg1(100);
	BigInt::Rossi arg2("123456789", BigInt::DEC_DIGIT);
	BigInt::Rossi arg3("123456789ABCDEF0FEDCBA321", BigInt::HEX_DIGIT);

	BigInt::Rossi result;

	// --------------------------------------------
	result = arg1 + arg2;
	oss << "[Hex] " << arg1 << " + " << arg2 << " = " << result << std::endl;
	oss << "[Dec] " << arg1.toStrDec() << " + " << arg2.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg1 + arg3;
	oss << "[Hex] " << arg1 << " + " << arg3 << " = " << result << std::endl;
	oss << "[Dec] " << arg1.toStrDec() << " + " << arg3.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg2 + arg3;
	oss << "[Hex] " << arg2 << " + " << arg3 << " = " << result << std::endl;
	oss << "[Dec] " << arg2.toStrDec() << " + " << arg3.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;


	// --------------------------------------------
	result = arg2 / arg1;
	oss << "[Hex] " << arg2 << " / " << arg1 << " = " << result << std::endl;
	oss << "[Dec] " << arg2.toStrDec() << " / " << arg1.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg3 / arg1;
	oss << "[Hex] " << arg3 << " / " << arg1 << " = " << result << std::endl;
	oss << "[Dec] " << arg3.toStrDec() << " / " << arg1.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg3 / arg2;
	oss << "[Hex] " << arg3 << " / " << arg2 << " = " << result << std::endl;
	oss << "[Dec] " << arg3.toStrDec() << " / " << arg2.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;


	// --------------------------------------------
	result = arg1 * arg2;
	oss << "[Hex] " << arg1 << " * " << arg2 << " = " << result << std::endl;
	oss << "[Dec] " << arg1.toStrDec() << " * " << arg2.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg1 * arg3;
	oss << "[Hex] " << arg1 << " * " << arg3 << " = " << result << std::endl;
	oss << "[Dec] " << arg1.toStrDec() << " * " << arg3.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg2 * arg3;
	oss << "[Hex] " << arg2 << " * " << arg3 << " = " << result << std::endl;
	oss << "[Dec] " << arg2.toStrDec() << " * " << arg3.toStrDec() << " = " << result.toStrDec() << std::endl;
	oss << std::endl;

	// --------------------------------------------
	result = arg1.sqrt();
	oss << "[Hex] " << arg1 << ".sqrt() = " << result << std::endl;
	oss << "[Dec] " << arg1.toStrDec() << ".sqrt() = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg2.sqrt();
	oss << "[Hex] " << arg2 << ".sqrt() = " << result << std::endl;
	oss << "[Dec] " << arg2.toStrDec() << ".sqrt() = " << result.toStrDec() << std::endl;
	oss << std::endl;

	result = arg3.sqrt();
	oss << "[Hex] " << arg3 << ".sqrt() = " << result << std::endl;
	oss << "[Dec] " << arg3.toStrDec() << ".sqrt() = " << result.toStrDec() << std::endl;
	oss << std::endl;


	// --------------------------
	SET_FINISH_TEST_NAME(oss);

	oss << std::endl;
	oss << std::endl;
	oss << std::endl;
	BigInt::Run::showHelp(oss, i_exeFileName);

	o_stream << oss.str() << std::endl;
}
static void showIllegalCommandLineBigInt(std::ostream& o_stream, int argc, char** argv)
{
	std::ostringstream oss;

	oss << ""

		<< std::endl
		<< "\t"
		<< "===> ILLEGAL INPUT <==="
		<< std::endl
		<< std::endl;

	BigInt::Run::showCommandLine(oss, argc, argv);
	BigInt::Run::showHelp(oss, argv[0]);

	o_stream << oss.str() << std::endl;
}
int testBigIntClass(int argc, char** argv) {

	BigInt::Run::showTime(std::cout, "START");
	BigInt::Run::showCommandLine(std::cout, argc, argv);

	std::string flag;

	if (argc == 1)
	{
		flag = BigInt::s_strHelp;
	}
	else
	{
		flag = argv[1];
	}


	if ((argc <= 2) && (flag == BigInt::s_strHelp))
	{
		BigInt::Run::showHelp(std::cout, argv[0]);
		BigInt::Run::showTime(std::cout, "FINISH showHelp()");
		return 0;
	}

	if ((argc == 2) && (flag == BigInt::s_strDemo))
	{
		simpleDemoBigInt(std::cout);
		demoBigInt(std::cout, argv[0]);
		BigInt::Run::showTime(std::cout, "FINISH demoBigInt()");
		return 0;
	}

	if (
		((argc == 2) && (flag == BigInt::s_strTest))
		||
		(argc == 4)
		)
	{
		const int result = BigInt::Run::mainBigInt(argc, argv);
		BigInt::Run::showTime(std::cout, "FINISH testBigInt()");
		return result;
	}

	if ((argc == 2) && (flag == BigInt::s_strTime))
	{
		BigInt::Run::mainBigInt(argc, argv);
		BigInt::Run::showTime(std::cout, "FINISH performanceTestBigInt()");
		return static_cast<int>(BigInt::Test::s_returnValue);
	}



	showIllegalCommandLineBigInt(std::cout, argc, argv);
	BigInt::Run::showTime(std::cout, "FINISH showIllegalCommandLineBigInt()");
	return 0;
}
#pragma endregion test_library
// Clears the current window and draws a triangle.
void display() {

	// Set every pixel in the frame buffer to the current clear color.
	glClear(GL_COLOR_BUFFER_BIT);

	// Drawing is done by specifying a sequence of vertices.  The way these
	// vertices are connected (or not connected) depends on the argument to
	// glBegin.  GL_POLYGON constructs a filled polygon.
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
	glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
	glEnd();

	// Flush drawing command buffer to make drawing happen as soon as possible.
	glFlush();
}

void TestElipticCurve() {
	BigInt::Rossi order(17);
	auto point = std::make_pair(BigInt::Rossi(6), BigInt::Rossi(11));
	ElipticCurveFF f(BigInt::Rossi(0), BigInt::Rossi(7), order,point);
	std::vector < std::pair< BigInt::Rossi, BigInt::Rossi>> vect;
	for (BigInt::Rossi i(0); i < order; i++) {


		auto p = f.FindPointAt(i);
		if (p.first == BigInt::Rossi(order) || p.second == BigInt::Rossi(order))continue;
		vect.push_back(p);
		if (p.second != BigInt::Rossi(0) && p.second != BigInt::Rossi(order))
			vect.push_back(std::make_pair(i, f.FindNegative(p.second)));
	}
	std::cout << "\t";
	for (int i = 0; i < vect.size(); i++) {
		std::cout << vect[i].first.toStrDec() << "," << vect[i].second.toStrDec() << "\t";

	}
	std::cout << "\n";
	for (int i = 0; i < vect.size(); i++) {
		std::cout << vect[i].first.toStrDec() << "," << vect[i].second.toStrDec() << ">>\t";
		for (int j = 0; j < vect.size(); j++) {

			auto r = f.AddPoints(vect[i].first, vect[i].second, vect[j].first, vect[j].second);
			std::cout << r.first.toStrDec() << "," << r.second.toStrDec() << "\t";
		}
		std::cout << "\n";
	}

}
void GlupInit() {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// Position window at (80,80)-(480,380) and give it a title.
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(400, 300);
	glutCreateWindow("A Simple Triangle");

	// Tell GLUT that whenever the main window needs to be repainted that it
	// should call the function display().
	glutDisplayFunc(display);

	glutMainLoop();
}

// ===================
int main(int argc, char** argv)
{
	//testBigIntClass(argc, argv);
	//BigInt::Rossi r("123456789",10);
	// Use a single buffered window in RGB mode (as opposed to a double-buffered
  // window or color-index mode).
	std::pair<BigInt::Rossi, BigInt::Rossi> G = std::make_pair(BigInt::Rossi("602046282375688656758213480587526111916698976636884684818", 10), BigInt::Rossi("174050332293622031404857552280219410364023488927386650641", 10));
	//"spec192k"
	ElipticCurveFF curve(BigInt::Rossi("6277101735386680763835789423207666416083908700390324961276",10), BigInt::Rossi("2455155546008943817740293915197451784769108058161191238065",10), BigInt::Rossi("6277101735386680763835789423207666416083908700390324961279",10),G);
	Host Alice(curve,"Alice"), Bob(curve,"Bob");
	Alice.generateSharedKey(Bob.public_key);
	Bob.generateSharedKey(Alice.public_key);
	Alice.printSharedKey();
	Bob.printSharedKey();


}


