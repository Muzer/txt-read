#include "wii.h"

 
 
using namespace std;

int boxDrawing(){
    char horiz_eq = 205;
    char horiz_eq_to_vert_tl = 213;
    char horiz_eq_to_vert_tr = 184;
    char horiz_eq_to_vert_br = 190;
    char horiz_eq_to_vert_bl = 212;
    char horiz_eq_to_vert_tm = 209;
    char horiz_eq_to_vert_bm = 207;
    char vert_bar = 179;
     cout << "\x1b[0;5H" << endl;
cout << " " << horiz_eq_to_vert_tl << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq_to_vert_tm << horiz_eq_to_vert_tm << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << "\x1b[36;1mHow to use\x1b[37;1m" << horiz_eq << horiz_eq << horiz_eq << horiz_eq_to_vert_tm << horiz_eq_to_vert_tm << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq_to_vert_tr << "\n";
cout << " " << vert_bar << "\x1b[38;1mA\x1b[37;1m to toggle numbering   " << vert_bar << vert_bar << " \x1b[30;1m1\x1b[37;1m to reselect file " << vert_bar << vert_bar << " \x1b[30;1m2\x1b[37;1m to completely restart " << vert_bar << endl;
cout << " " << vert_bar << "\x1b[30;1mDown\x1b[37;1m to scroll down     " << vert_bar << vert_bar << " \x1b[30;1mUp\x1b[37;1m to scroll up    " << vert_bar << vert_bar << " \x1b[30;1mHome\x1b[37;1m to return to loader" << vert_bar << endl;
    cout << " " << horiz_eq_to_vert_bl << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq_to_vert_bm << horiz_eq_to_vert_bm << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq_to_vert_bm << horiz_eq_to_vert_bm << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq << horiz_eq_to_vert_br << "\n";
}