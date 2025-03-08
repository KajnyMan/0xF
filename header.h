#ifndef header_h
#define header_h

typedef	int Digit;
typedef	unsigned char size_a;
typedef enum { false, true } bool;
enum Direction { up, right, down, left };
enum Color { black,	blue, green, cyan, red,	magenta, brown, lgray,
	 dgray, lblue, lgreen, lcyan, lred,	lmagenta, yellow, white };
enum special_chars {
	lv_line		= 186,
	rv_line		= 186,
	ur_corner	= 187,
	dr_corner	= 188,
	dl_corner	= 200,
	ul_corner	= 201,
	uh_line		= 205,
	dh_line		= 205
};

#define NULL 0

#define BOARD_H 6
#define BOARD_W 6

extern int F_HEADER, F_ERROR;
extern unsigned long power10( int );
extern int power( int, int );

#endif // header_h
