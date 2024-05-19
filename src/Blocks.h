#pragma once

#include "Helpers.h"
#include "matrix.h"

using BlockMatrix = Matrix<5, 5, char>;

BlockMatrix L {{
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 , 'L',  0 ,  0 },
	{ 0 ,  0 , 'L',  0 ,  0 },
	{ 0 ,  0 , 'L', 'L',  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}}, R{{
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 , 'R',  0 ,  0 },
	{ 0 ,  0 , 'R',  0 ,  0 },
	{ 0 , 'R', 'R',  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}}, S{{
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 , 'S', 'S',  0 },
	{ 0 , 'S', 'S',  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}}, Z{{
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 , 'Z', 'Z',  0 ,  0 },
	{ 0 ,  0 , 'Z', 'Z',  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}}, T{{
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 , 'T',  0 ,  0 },
	{ 0 , 'T', 'T', 'T',  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}}, Q{{
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 , 'Q', 'Q',  0 },
	{ 0 ,  0 , 'Q', 'Q',  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}}, I{{
	{ 0 ,  0 , 'I',  0 ,  0 },
	{ 0 ,  0 , 'I',  0 ,  0 },
	{ 0 ,  0 , 'I',  0 ,  0 },
	{ 0 ,  0 , 'I',  0 ,  0 },
	{ 0 ,  0 ,  0 ,  0 ,  0 },
}};

const BlockMatrix& getBlockMatrix (char type) {
	switch (type) {
		case 'L':
			return L;
		case 'R':
			return R;
		case 'S':
			return S;
		case 'Z':
			return Z;
		case 'T':
			return T;
		case 'I':
			return I;
		case 'Q':
			return Q;
		default:
			exit(1);
	}
}

int getXIndex (char type) {
	return 2;
}

int getYIndex (char type) {
	if (type == 'I')
		return -4; //-4
	else
		return -3; //-3
}